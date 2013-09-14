#! /usr/bin/env python
# -*- coding:utf-8 -*-
# Author : wenxueliu
# Date   : 20130830
# Comment: abstract from openstack swift
# License: Apache 2.0

from __future__ import with_statement
import functools
import errno
import os
import resource
import signal
import time
import subprocess
import re
from gettext import gettext as _

from swift.common.utils import search_tree, remove_file, write_file


SWIFT_DIR = '/etc/swift'
RUN_DIR = '/var/run/swift'

KILL_WAIT = 15  # seconds to wait for servers to die (by default)
WARNING_WAIT = 3  # seconds to wait after message that may just be a warning

MAX_DESCRIPTORS = 32768
MAX_MEMORY = (1024 * 1024 * 1024) * 2  # 2 GB


#add the server you want to run
ALL_SERVERS = ['server1', 'server2', 'server3']
MAIN_SERVERS = ['server1', 'server2']
REST_SERVERS = [s for s in ALL_SERVERS if s not in MAIN_SERVERS]

GRACEFUL_SHUTDOWN_SERVERS = MAIN_SERVERS + ['auth-server']
START_ONCE_SERVERS = REST_SERVERS
# These are servers that match a type (account-*, container-*, object-*) but
# don't use that type-server.conf file and instead use their own.
STANDALONE_SERVERS = ['object-expirer']

def setup_env():
    """Try to increase resource limits of the OS. Move PYTHON_EGG_CACHE to /tmp
    """
    try:
        resource.setrlimit(resource.RLIMIT_NOFILE,
                           (MAX_DESCRIPTORS, MAX_DESCRIPTORS))
        resource.setrlimit(resource.RLIMIT_DATA,
                           (MAX_MEMORY, MAX_MEMORY))
    except ValueError:
        print _("WARNING: Unable to increase file descriptor limit.  "
                "Running as non-root?")

    os.environ['PYTHON_EGG_CACHE'] = '/tmp'

class UnknownCommandError(Exception):
    pass


class Manager():
    """Main class for performing commands on groups of servers.

    :param servers: list of server names as strings

    """

    def __init__(self, servers, run_dir=RUN_DIR):
        server_names = set()
        for server in servers:
            if server == 'all':
                server_names.update(ALL_SERVERS)
            elif server == 'main':
                server_names.update(MAIN_SERVERS)
            elif server == 'rest':
                server_names.update(REST_SERVERS)
            elif '*' in server:
                server_names.update([s for s in ALL_SERVERS if
                                     re.match(server.replace('*', '.*'), s)])
            else:
                server_names.add(server)

        self.servers = set()
        for name in server_names:
            self.servers.add(Server(name, run_dir))

    @command
    def status(self, **kwargs):
        """display status of tracked pids for server
        """
        status = 0
        for server in self.servers:
            status += server.status(**kwargs)
        return status

    @command
    def start(self, **kwargs):
        """starts a server
        """
        setup_env()
        status = 0

        for server in self.servers:
            status = server.launch(**kwargs)
        #add the kwargs you want to do, for example
        if not kwargs.get('daemon', True):
            print "not daemon"
        return status


    @command
    def stop(self, **kwargs):
        """stops a server
        """
        server_pids = {}
        for server in self.servers:
            signaled_pids = server.stop(**kwargs)
            if not signaled_pids:
                print _('No %s running') % server
            else:
                server_pids[server] = signaled_pids

        return 1


    def get_command(self, cmd):
        """Find and return the decorated method named like cmd

        :param cmd: the command to get, a string, if not found raises
                    UnknownCommandError

        """
        try:
            f = getattr(self, cmd)
        except AttributeError:
            raise UnknownCommandError(cmd)
        if not hasattr(f, 'publicly_accessible'):
            raise UnknownCommandError(cmd)
        return f

    @classmethod
    def list_commands(cls):
        """Get all publicly accessible commands

        :returns: a list of string tuples (cmd, help), the method names who are
                  decorated as commands
        """
        get_method = lambda cmd: getattr(cls, cmd)

        #the sorted() isn't needed currently
        return sorted([(x, get_method(x).__doc__.strip())
                       for x in dir(cls) if
                       getattr(get_method(x), 'publicly_accessible', False)])

    def run_command(self, cmd, **kwargs):
        """Find the named command and run it

        :param cmd: the command name to run

        """
        f = self.get_command(cmd)
        return f(**kwargs)



class Server():
    """Manage operations on a server or group of servers of similar type

    :param server: name of server
    """

    def __init__(self, server, run_dir=RUN_DIR):
        if '-' not in server:
            server = '%s-server' % server
        self.server = server.lower()
        self.type = server.rsplit('-', 1)[0]
        self.cmd = 'swift-%s' % server
        self.procs = []
        self.run_dir = run_dir

    def __str__(self):
        return self.server

    def __repr__(self):
        return "%s(%s)" % (self.__class__.__name__, repr(str(self)))

    def __hash__(self):
        return hash(str(self))

    def __eq__(self, other):
        try:
            return self.server == other.server
        except AttributeError:
            return False

    def get_pid_file_name(self, conf_file):
        """Translate conf_file to a corresponding pid_file

        :param conf_file: an conf_file for this server, a string

        :returns: the pid_file for this conf_file

        """
        return conf_file.replace(
            os.path.normpath(SWIFT_DIR), self.run_dir, 1).replace(
                '%s-server' % self.type, self.server, 1).replace(
                    '.conf', '.pid', 1)

    def get_conf_file_name(self, pid_file):
        """Translate pid_file to a corresponding conf_file

        :param pid_file: a pid_file for this server, a string

        :returns: the conf_file for this pid_file

        """
        if self.server in STANDALONE_SERVERS:
            return pid_file.replace(
                os.path.normpath(self.run_dir), SWIFT_DIR, 1).replace(
                    '.pid', '.conf', 1)
        else:
            return pid_file.replace(
                os.path.normpath(self.run_dir), SWIFT_DIR, 1).replace(
                    self.server, '%s-server' % self.type, 1).replace(
                        '.pid', '.conf', 1)

    def conf_files(self, **kwargs):
        """Get conf files for this server

        :param: number, if supplied will only lookup the nth server

        :returns: list of conf files
        """
        if self.server in STANDALONE_SERVERS:
            found_conf_files = search_tree(SWIFT_DIR, self.server + '*',
                                           '.conf', dir_ext='.conf.d')
        else:
            found_conf_files = search_tree(SWIFT_DIR, '%s-server*' % self.type,
                                           '.conf', dir_ext='.conf.d')
        number = kwargs.get('number')
        if number:
            try:
                conf_files = [found_conf_files[number - 1]]
            except IndexError:
                conf_files = []
        else:
            conf_files = found_conf_files
        if not conf_files:
            # maybe there's a config file(s) out there, but I couldn't find it!
            if not kwargs.get('quiet'):
                print _('Unable to locate config %sfor %s') % (
                    ('number %s ' % number if number else ''), self.server)
            if kwargs.get('verbose') and not kwargs.get('quiet'):
                if found_conf_files:
                    print _('Found configs:')
                for i, conf_file in enumerate(found_conf_files):
                    print '  %d) %s' % (i + 1, conf_file)

        return conf_files

    def pid_files(self, **kwargs):
        """Get pid files for this server

        :param: number, if supplied will only lookup the nth server

        :returns: list of pid files
        """
        pid_files = search_tree(self.run_dir, '%s*' % self.server)
        if kwargs.get('number', 0):
            conf_files = self.conf_files(**kwargs)
            # filter pid_files to match the index of numbered conf_file
            pid_files = [pid_file for pid_file in pid_files if
                         self.get_conf_file_name(pid_file) in conf_files]
        return pid_files

    def iter_pid_files(self, **kwargs):
        """Generator, yields (pid_file, pids)
        """
        for pid_file in self.pid_files(**kwargs):
            yield pid_file, int(open(pid_file).read().strip())

    def signal_pids(self, sig, **kwargs):
        """Send a signal to pids for this server

        :param sig: signal to send

        :returns: a dict mapping pids (ints) to pid_files (paths)

        """
        pids = {}
        for pid_file, pid in self.iter_pid_files(**kwargs):
            try:
                if sig != signal.SIG_DFL:
                    print _('Signal %s  pid: %s  signal: %s') % (self.server,
                                                                 pid, sig)
                os.kill(pid, sig)
            except OSError, e:
                if e.errno == errno.ESRCH:
                    # pid does not exist
                    if kwargs.get('verbose'):
                        print _("Removing stale pid file %s") % pid_file
                    remove_file(pid_file)
                elif e.errno == errno.EPERM:
                    print _("No permission to signal PID %d") % pid
            else:
                # process exists
                pids[pid] = pid_file
        return pids

    def get_running_pids(self, **kwargs):
        """Get running pids

        :returns: a dict mapping pids (ints) to pid_files (paths)

        """
        return self.signal_pids(signal.SIG_DFL, **kwargs)  # send noop

    def kill_running_pids(self, **kwargs):
        """Kill running pids

        :param graceful: if True, attempt SIGHUP on supporting servers

        :returns: a dict mapping pids (ints) to pid_files (paths)

        """
        graceful = kwargs.get('graceful')
        if graceful and self.server in GRACEFUL_SHUTDOWN_SERVERS:
            sig = signal.SIGHUP
        else:
            sig = signal.SIGTERM
        return self.signal_pids(sig, **kwargs)

    def status(self, pids=None, **kwargs):
        """Display status of server

        :param: pids, if not supplied pids will be populated automatically
        :param: number, if supplied will only lookup the nth server

        :returns: 1 if server is not running, 0 otherwise
        """
        if pids is None:
            pids = self.get_running_pids(**kwargs)
        if not pids:
            number = kwargs.get('number', 0)
            if number:
                kwargs['quiet'] = True
                conf_files = self.conf_files(**kwargs)
                if conf_files:
                    print _("%s #%d not running (%s)") % (self.server, number,
                                                          conf_files[0])
            else:
                print _("No %s running") % self.server
            return 1
        for pid, pid_file in pids.items():
            conf_file = self.get_conf_file_name(pid_file)
            print _("%s running (%s - %s)") % (self.server, pid, conf_file)
        return 0

    def spawn(self, conf_file, once=False, wait=True, daemon=True, **kwargs):
        """Launch a subprocess for this server.

        :param conf_file: path to conf_file to use as first arg
        :param once: boolean, add once argument to command
        :param wait: boolean, if true capture stdout with a pipe
        :param daemon: boolean, if true ask server to log to console

        :returns : the pid of the spawned process
        """
        args = [self.cmd, conf_file]
        if once:
            args.append('once')
        if not daemon:
            # ask the server to log to console
            args.append('verbose')

        # figure out what we're going to do with stdio
        if not daemon:
            # do nothing, this process is open until the spawns close anyway
            re_out = None
            re_err = None
        else:
            re_err = subprocess.STDOUT
            if wait:
                # we're going to need to block on this...
                re_out = subprocess.PIPE
            else:
                re_out = open(os.devnull, 'w+b')
        proc = subprocess.Popen(args, stdout=re_out, stderr=re_err)
        pid_file = self.get_pid_file_name(conf_file)
        write_file(pid_file, proc.pid)
        self.procs.append(proc)
        return proc.pid

    def wait(self, **kwargs):
        """
        wait on spawned procs to start
        """
        status = 0
        for proc in self.procs:
            # wait for process to close its stdout
            output = proc.stdout.read()
            if output:
                print output
                start = time.time()
                # wait for process to die (output may just be a warning)
                while time.time() - start < WARNING_WAIT:
                    time.sleep(0.1)
                    if proc.poll() is not None:
                        status += proc.returncode
                        break
        return status

    def interact(self, **kwargs):
        """
        wait on spawned procs to terminate
        """
        status = 0
        for proc in self.procs:
            # wait for process to terminate
            proc.communicate()
            if proc.returncode:
                status += 1
        return status

    def launch(self, **kwargs):
        """
        Collect conf files and attempt to spawn the processes for this server
        """
        conf_files = self.conf_files(**kwargs)
        if not conf_files:
            return []

        pids = self.get_running_pids(**kwargs)

        already_started = False
        for pid, pid_file in pids.items():
            conf_file = self.get_conf_file_name(pid_file)
            # for legacy compat you can't start other servers if one server is
            # already running (unless -n specifies which one you want), this
            # restriction could potentially be lifted, and launch could start
            # any unstarted instances
            if conf_file in conf_files:
                already_started = True
                print _("%s running (%s - %s)") % (self.server, pid, conf_file)
            elif not kwargs.get('number', 0):
                already_started = True
                print _("%s running (%s - %s)") % (self.server, pid, pid_file)

        if already_started:
            print _("%s already started...") % self.server
            return []

        if self.server not in START_ONCE_SERVERS:
            kwargs['once'] = False

        pids = {}
        for conf_file in conf_files:
            if kwargs.get('once'):
                msg = _('Running %s once') % self.server
            else:
                msg = _('Starting %s') % self.server
            print '%s...(%s)' % (msg, conf_file)
            try:
                pid = self.spawn(conf_file, **kwargs)
            except OSError, e:
                if e.errno == errno.ENOENT:
                    #TODO(clayg): should I check if self.cmd exists earlier?
                    print _("%s does not exist") % self.cmd
                    break
            pids[pid] = conf_file

        return pids

    def stop(self, **kwargs):
        """Send stop signals to pids for this server

        :returns: a dict mapping pids (ints) to pid_files (paths)

        """
        return self.kill_running_pids(**kwargs)

def main():
    manager = Manager(servers, run_dir=options.run_dir)
    try:
        status = manager.run_command(command, **options.__dict__)
    except UnknownCommandError:
        parser.print_help()
        print 'ERROR: unknown command, %s' % command
        status = 1

    return 1 if status else 0
