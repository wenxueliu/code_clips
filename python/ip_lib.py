# vim: tabstop=4 shiftwidth=4 softtabstop=4
#change from openstack sightly
# All Rights Reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License"); you may
#    not use this file except in compliance with the License. You may obtain
#    a copy of the License at
#
#         http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
#    WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
#    License for the specific language governing permissions and limitations
#    under the License.

"""
  you need prepare for it before you start:
    pip install netaddr
    install oslo.config of openStack
"""

import netaddr
from oslo.config import cfg


OPTS = [
    cfg.BoolOpt('ip_lib_force_root',
                default=False,
                help=_('Force ip_lib calls to use the root helper')),
]


LOOPBACK_DEVNAME = 'lo'
# NOTE(ethuleau): depend of the version of iproute2, the vlan
# interface details vary.
VLAN_INTERFACE_DETAIL = ['vlan protocol 802.1q',
                         'vlan protocol 802.1Q',
                         'vlan id']


class SubProcessBase(object):
    def __init__(self, root_helper=None, namespace=None):
        self.root_helper = root_helper
        self.namespace = namespace
        try:
            self.force_root = cfg.CONF.ip_lib_force_root
        except cfg.NoSuchOptError:
            # Only callers that need to force use of the root helper
            # need to register the option.
            self.force_root = False

    def _run(self, options, command, args):
        if self.namespace:
            return self._as_root(options, command, args)
        elif self.force_root:
            # Force use of the root helper to ensure that commands
            # will execute in dom0 when running under XenServer/XCP.
            return self._execute(options, command, args, self.root_helper)
        else:
            return self._execute(options, command, args)

    def _as_root(self, options, command, args, use_root_namespace=False):
        """
         if namespace isn't None, the root_helper must be set.
         if use_root_namespace = True the namespace is None
        """
        if not self.root_helper:
            raise SudoRequired()

        namespace = self.namespace if not use_root_namespace else None

        return self._execute(options,
                             command,
                             args,
                             self.root_helper,
                             namespace)

    @classmethod
    def _execute(cls, options, command, args, root_helper=None,
                 namespace=None):
        opt_list = ['-%s' % o for o in options]
        if namespace:
            ip_cmd = ['ip', 'netns', 'exec', namespace, 'ip']
        else:
            ip_cmd = ['ip']
        return execute(ip_cmd + opt_list + [command] + list(args),
                             root_helper=root_helper)


class IPWrapper(SubProcessBase):
    def __init__(self, root_helper=None, namespace=None):
        super(IPWrapper, self).__init__(root_helper=root_helper,
                                        namespace=namespace)
        self.netns = IpNetnsCommand(self)

    def device(self, name):
        return IPDevice(name, self.root_helper, self.namespace)

    def get_devices(self, exclude_loopback=False):
        retval = []
        output = self._execute(['o', 'd'], 'link', ('list',),
                               self.root_helper, self.namespace)
        for line in output.split('\n'):
            if '<' not in line:
                continue
            tokens = line.split(' ', 2)
            if len(tokens) == 3:
                if any(v in tokens[2] for v in VLAN_INTERFACE_DETAIL):
                    delimiter = '@'
                else:
                    delimiter = ':'
                name = tokens[1].rpartition(delimiter)[0].strip()

                if exclude_loopback and name == LOOPBACK_DEVNAME:
                    continue

                retval.append(IPDevice(name,
                                       self.root_helper,
                                       self.namespace))
        return retval

    def add_tuntap(self, name, mode='tap'):
        self._as_root('', 'tuntap', ('add', name, 'mode', mode))
        return IPDevice(name, self.root_helper, self.namespace)

    def add_veth(self, name1, name2, namespace2=None):
        """
        it can be another namespace, default is self.namespace
        if the namespace2 doesn't exist, add the namespace2 
        """
        args = ['add', name1, 'type', 'veth', 'peer', 'name', name2]

        if namespace2 is None:
            namespace2 = self.namespace
        else:
            self.ensure_namespace(namespace2)
            args += ['netns', namespace2]

        self._as_root('', 'link', tuple(args))

        return (IPDevice(name1, self.root_helper, self.namespace),
                IPDevice(name2, self.root_helper, namespace2))

    def ensure_namespace(self, name):
        if not self.netns.exists(name):
            ip = self.netns.add(name)
            lo = ip.device(LOOPBACK_DEVNAME)
            lo.link.set_up()
        else:
            ip = IPWrapper(self.root_helper, name)
        return ip

    def namespace_is_empty(self):
        return not self.get_devices(exclude_loopback=True)

    def garbage_collect_namespace(self):
        """Conditionally destroy the namespace if it is empty."""
        if self.namespace and self.netns.exists(self.namespace):
            if self.namespace_is_empty():
                self.netns.delete(self.namespace)
                return True
        return False

    def add_device_to_namespace(self, device):
        if self.namespace:
            device.link.set_netns(self.namespace)

    def add_vxlan(self, name, vni, group=None, dev=None, ttl=None, tos=None,
                  local=None, port=None, proxy=False):
        cmd = ['add', name, 'type', 'vxlan', 'id', vni]
        if group:
                cmd.extend(['group', group])
        if dev:
                cmd.extend(['dev', dev])
        if ttl:
                cmd.extend(['ttl', ttl])
        if tos:
                cmd.extend(['tos', tos])
        if local:
                cmd.extend(['local', local])
        if proxy:
                cmd.append('proxy')
        # tuple: min,max
        if port and len(port) == 2:
                cmd.extend(['port', port[0], port[1]])
        elif port:
            raise NetworkVxlanPortRangeError(vxlan_range=port)
        self._as_root('', 'link', cmd)
        return (IPDevice(name, self.root_helper, self.namespace))

    @classmethod
    def get_namespaces(cls, root_helper):
        output = cls._execute('', 'netns', ('list',), root_helper=root_helper)
        return [l.strip() for l in output.split('\n')]


class IPDevice(SubProcessBase):
    def __init__(self, name, root_helper=None, namespace=None):
        super(IPDevice, self).__init__(root_helper=root_helper,
                                       namespace=namespace)
        self.name = name
        self.link = IpLinkCommand(self)
        self.addr = IpAddrCommand(self)
        self.route = IpRouteCommand(self)

    def __eq__(self, other):
        return (other is not None and self.name == other.name
                and self.namespace == other.namespace)

    def __str__(self):
        return self.name


class IpCommandBase(object):
    COMMAND = ''

    def __init__(self, parent):
        self._parent = parent

    def _run(self, *args, **kwargs):
        return self._parent._run(kwargs.get('options', []), self.COMMAND, args)

    def _as_root(self, *args, **kwargs):
        return self._parent._as_root(kwargs.get('options', []),
                                     self.COMMAND,
                                     args,
                                     kwargs.get('use_root_namespace', False))


class IpDeviceCommandBase(IpCommandBase):
    @property
    def name(self):
        return self._parent.name


class IpLinkCommand(IpDeviceCommandBase):
    COMMAND = 'link'

    def set_address(self, mac_address):
        self._as_root('set', self.name, 'address', mac_address)

    def set_mtu(self, mtu_size):
        self._as_root('set', self.name, 'mtu', mtu_size)

    def set_up(self):
        self._as_root('set', self.name, 'up')

    def set_down(self):
        self._as_root('set', self.name, 'down')

    def set_netns(self, namespace):
        self._as_root('set', self.name, 'netns', namespace)
        self._parent.namespace = namespace

    def set_name(self, name):
        self._as_root('set', self.name, 'name', name)
        self._parent.name = name

    def set_alias(self, alias_name):
        self._as_root('set', self.name, 'alias', alias_name)

    def delete(self):
        self._as_root('delete', self.name)

    @property
    def address(self):
        return self.attributes.get('link/ether')

    @property
    def state(self):
        return self.attributes.get('state')

    @property
    def mtu(self):
        return self.attributes.get('mtu')

    @property
    def qdisc(self):
        return self.attributes.get('qdisc')

    @property
    def qlen(self):
        return self.attributes.get('qlen')

    @property
    def alias(self):
        return self.attributes.get('alias')

    @property
    def attributes(self):
        return self._parse_line(self._run('show', self.name, options='o'))

    def _parse_line(self, value):
        if not value:
            return {}

        device_name, settings = value.replace("\\", '').split('>', 1)
        tokens = settings.split()
        keys = tokens[::2]
        values = [int(v) if v.isdigit() else v for v in tokens[1::2]]

        retval = dict(zip(keys, values))
        return retval


class IpAddrCommand(IpDeviceCommandBase):
    COMMAND = 'addr'

    def add(self, ip_version, cidr, broadcast, scope='global'):
        self._as_root('add',
                      cidr,
                      'brd',
                      broadcast,
                      'scope',
                      scope,
                      'dev',
                      self.name,
                      options=[ip_version])

    def delete(self, ip_version, cidr):
        self._as_root('del',
                      cidr,
                      'dev',
                      self.name,
                      options=[ip_version])

    def flush(self):
        self._as_root('flush', self.name)

    def list(self, scope=None, to=None, filters=None):
        if filters is None:
            filters = []

        retval = []

        if scope:
            filters += ['scope', scope]
        if to:
            filters += ['to', to]

        for line in self._run('show', self.name, *filters).split('\n'):
            line = line.strip()
            if not line.startswith('inet'):
                continue
            parts = line.split()
            if parts[0] == 'inet6':
                version = 6
                scope = parts[3]
                broadcast = '::'
            else:
                version = 4
                if parts[2] == 'brd':
                    broadcast = parts[3]
                    scope = parts[5]
                else:
                    # sometimes output of 'ip a' might look like:
                    # inet 192.168.100.100/24 scope global eth0
                    # and broadcast needs to be calculated from CIDR
                    broadcast = str(netaddr.IPNetwork(parts[1]).broadcast)
                    scope = parts[3]

            retval.append(dict(cidr=parts[1],
                               broadcast=broadcast,
                               scope=scope,
                               ip_version=version,
                               dynamic=('dynamic' == parts[-1])))
        return retval


class IpRouteCommand(IpDeviceCommandBase):
    COMMAND = 'route'

    def add_gateway(self, gateway, metric=None):
        args = ['replace', 'default', 'via', gateway]
        if metric:
            args += ['metric', metric]
        args += ['dev', self.name]
        self._as_root(*args)

    def delete_gateway(self, gateway):
        self._as_root('del',
                      'default',
                      'via',
                      gateway,
                      'dev',
                      self.name)

    def get_gateway(self, scope=None, filters=None):
        if filters is None:
            filters = []

        retval = None

        if scope:
            filters += ['scope', scope]

        route_list_lines = self._run('list', 'dev', self.name,
                                     *filters).split('\n')
        default_route_line = next((x.strip() for x in
                                   route_list_lines if
                                   x.strip().startswith('default')), None)
        if default_route_line:
            gateway_index = 2
            parts = default_route_line.split()
            retval = dict(gateway=parts[gateway_index])
            metric_index = 4
            parts_has_metric = (len(parts) > metric_index)
            if parts_has_metric:
                retval.update(metric=int(parts[metric_index]))

        return retval

    def pullup_route(self, interface_name):
        """Ensures that the route entry for the interface is before all
        others on the same subnet.
        """
        device_list = []
        device_route_list_lines = self._run('list', 'proto', 'kernel',
                                            'dev', interface_name).split('\n')
        for device_route_line in device_route_list_lines:
            try:
                subnet = device_route_line.split()[0]
            except Exception:
                continue
            subnet_route_list_lines = self._run('list', 'proto', 'kernel',
                                                'match', subnet).split('\n')
            for subnet_route_line in subnet_route_list_lines:
                i = iter(subnet_route_line.split())
                while(i.next() != 'dev'):
                    pass
                device = i.next()
                try:
                    while(i.next() != 'src'):
                        pass
                    src = i.next()
                except Exception:
                    src = ''
                if device != interface_name:
                    device_list.append((device, src))
                else:
                    break

            for (device, src) in device_list:
                self._as_root('del', subnet, 'dev', device)
                if (src != ''):
                    self._as_root('append', subnet, 'proto', 'kernel',
                                  'src', src, 'dev', device)
                else:
                    self._as_root('append', subnet, 'proto', 'kernel',
                                  'dev', device)


class IpNetnsCommand(IpCommandBase):
    COMMAND = 'netns'

    def add(self, name):
        self._as_root('add', name, use_root_namespace=True)
        return IPWrapper(self._parent.root_helper, name)

    def delete(self, name):
        self._as_root('delete', name, use_root_namespace=True)

    def _execute(self, cmds, addl_env={}, check_exit_code=True):
        if not self._parent.root_helper:
            raise SudoRequired()
        elif not self._parent.namespace:
            raise Exception('No namespace defined for parent')
        else:
            env_params = []
            if addl_env:
                env_params = (['env'] +
                              ['%s=%s' % pair for pair in addl_env.items()])
            return execute(
                ['ip', 'netns', 'exec', self._parent.namespace] +
                env_params + list(cmds),
                root_helper=self._parent.root_helper,
                check_exit_code=check_exit_code)

    def exists(self, name):
        output = self._as_root('list', options='o', use_root_namespace=True)

        for line in output.split('\n'):
            if name == line.strip():
                return True
        return False


def device_exists(device_name, root_helper=None, namespace=None):
    try:
        address = IPDevice(device_name, root_helper, namespace).link.address
    except RuntimeError:
        return False
    return bool(address)


def iproute_arg_supported(command, arg, root_helper=None):
    command += ['help']
    stdout, stderr = execute(command, root_helper=root_helper,
                                   check_exit_code=False, return_stderr=True)
    return any(arg in line for line in stderr.split('\n'))


#=======================================================================
import signal
import shlex
from eventlet.green import subprocess
from eventlet import greenthread

def _subprocess_setup():
    # Python installs a SIGPIPE handler by default. This is usually not what
    # non-Python subprocesses expect.
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)

def subprocess_popen(args, stdin=None, stdout=None, stderr=None, shell=False,
                     env=None):
    return subprocess.Popen(args, shell=shell, stdin=stdin, stdout=stdout,
                            stderr=stderr, preexec_fn=_subprocess_setup,
                            close_fds=True, env=env)

def create_process(cmd, root_helper=None, addl_env=None):
    """Create a process object for the given command.

    The return value will be a tuple of the process object and the
    list of command arguments used to create it.
    """
    if root_helper:
        cmd = shlex.split(root_helper) + cmd
    cmd = map(str, cmd)

    #LOG.debug(_("Running command: %s"), cmd)
    print "Running command: %s",cmd
    env = os.environ.copy()
    if addl_env:
        env.update(addl_env)

    obj = subprocess_popen(cmd, shell=False,
                                 stdin=subprocess.PIPE,
                                 stdout=subprocess.PIPE,
                                 stderr=subprocess.PIPE,
                                 env=env)

    return obj, cmd


def execute(cmd, root_helper=None, process_input=None, addl_env=None,
            check_exit_code=True, return_stderr=False):
    try:
        obj, cmd = create_process(cmd, root_helper=root_helper,
                                  addl_env=addl_env)
        _stdout, _stderr = (process_input and
                            obj.communicate(process_input) or
                            obj.communicate())
        obj.stdin.close()
        m = "\nCommand: %(cmd)s\nExit code: %(code)s\nStdout: %(stdout)r\n"
              "Stderr: %(stderr)r" % {'cmd': cmd, 'code': obj.returncode,
                                       'stdout': _stdout, 'stderr': _stderr}
        #LOG.debug(m)
        print m
        if obj.returncode and check_exit_code:
            raise RuntimeError(m)
    finally:
        # NOTE(termie): this appears to be necessary to let the subprocess
        #               call clean something up in between calls, without
        #               it two execute calls in a row hangs the second one
        greenthread.sleep(0)

    return return_stderr and (_stdout, _stderr) or _stdout

#========================================================================
class SudoRequired(NeutronException):
    message = "Sudo privilege is required to run this command."

class NetworkVxlanPortRangeError(NeutronException):
    message = "Invalid network VXLAN port range: '%(vxlan_range)s'"


if __name__ == "__main__":
    ip = IpAddrCommand()
    ip.list()
