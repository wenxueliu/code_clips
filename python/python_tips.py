#!/usr/bin/env python
# -*- utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4

# From OpenStack Project Mostly
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

import os
import sys
from posixpath import curdir, sep, pardir, join, abspath, commonprefix
import locale

enc = locale.getpreferredencoding()

def unicode_abspath(path):
    global enc
    assert type(path) is unicode
    # shouldn't pass unicode to this craphead, it appends with os.getcwd() which is always a str
    return os.path.abspath(path.encode(sys.getfilesystemencoding())).decode(sys.getfilesystemencoding())


def relpath(path, start=curdir):
    """Return a relative version of a path"""

    if not path:
        raise ValueError("no path specified")

    if type(start) is unicode:
        start_list = unicode_abspath(start).split(sep)
    else:
        start_list = abspath(start).split(sep)

    if type(path) is unicode:
        path_list = unicode_abspath(path).split(sep)
    else:
        path_list = abspath(path).split(sep)

    # Work out how much of the filepath is shared by start and path.
    i = len(commonprefix([start_list, path_list]))

    rel_list = [pardir] * (len(start_list)-i) + path_list[i:]
    if not rel_list:
        return curdir
    return join(*rel_list)


def console_print(st=u"", f=sys.stdout, linebreak=True):
    global enc
    assert type(st) is unicode
    f.write(st.encode(enc))
    if linebreak: f.write(os.linesep)

def console_flush(f=sys.stdout):
    f.flush()

#@parm question: you want to question which need replay yes/no; 
def yes_no_question(question):
    """a chose of y/n"""
    while True:
        console_print(question, linebreak=False)
        console_print(u" [y/n] ", linebreak=False)
        console_flush()
        text = raw_input()
        if text.lower().startswith("y"):
            return True
        elif text.lower().startswith("n"):
            return False
        else:
            console_print(u"Sorry, I didn't understand that. Please type yes or no.")


def plat():
    """get the platform x86 or x86_64"""
    if sys.platform.lower().startswith('linux'):
        arch = platform.machine()
        if (arch[0] == 'i' and
            arch[1].isdigit() and
            arch[2:4] == '86'):
            plat = "x86"
        elif arch == 'x86_64':
            plat = arch
        else:
            FatalVisibleError("Platform not supported")
        return "lnx.%s" % plat
    else:
        FatalVisibleError("Platform not supported")


def mkdirs(path, overwrite=False):
    from os import makedirs
    from errno import EEXIST
    try:
        makedirs(path)
    except OSError as err:
        if err.errno == EEXIST:
            if not overwrite:
                print "path '%s' already exists" % path
        else:
            raise

####################################################
from optparse import OptionParser, SUPPRESS_HELP
def CmdLineParse():   
    """
    命令行程序： from python-swiftclient
    """
    parser = OptionParser(version='%%prog %s' % version,
                          usage = '''
Usage: %%prog <command> [options] [args]
.....
'''.strip('\n') )
    parser.add_option('-v', '--verbose', action='count', dest='verbose',
            default=1, help='print more info')
    parser.disable_interspersed_args()
    (options, args) = parse_args(parser, argv[1:], enforce_requires=False)
    parser.enable_interspersed_args()

def encode_utf8(value):
    if isinstance(value, unicode):
        value = value.encode('utf8')
    return value


TRUE_VALUES = set(('true', '1', 'yes', 'on', 't', 'y'))
def config_true_value(value):
    """
    Returns True if the value is either True or a string in TRUE_VALUES.
    Returns False otherwise.
    This function come from swift.common.utils.config_true_value()
    """
    return value is True or \
        (isinstance(value, basestring) and value.lower() in TRUE_VALUES)


def prt_bytes(bytes, human_flag):
    """
    convert a number > 1024 to printable format, either in 4 char -h format as
    with ls -lh or return as 12 char right justified string
    """

    if human_flag:
        suffix = ''
        mods = 'KMGTPEZY'
        temp = float(bytes)
        if temp > 0:
            while (temp > 1023):
                temp /= 1024.0
                suffix = mods[0]
                mods = mods[1:]
            if suffix != '':
                if temp >= 10:
                    bytes = '%3d%s' % (temp, suffix)
                else:
                    bytes = '%.1f%s' % (temp, suffix)
        if suffix == '':    # must be < 1024
            bytes = '%4s' % bytes
    else:
        bytes = '%12s' % bytes

    return(bytes)


#========================================
import os
import random
import shlex
import signal
import logging
from eventlet.green import subprocess
from eventlet import greenthread

LOG = logging.getLogger(__name__)

class InvalidArgumentError(Exception):
    def __init__(self, message=None):
        super(InvalidArgumentError, self).__init__(message)


class UnknownArgumentError(Exception):
    def __init__(self, message=None):
        super(UnknownArgumentError, self).__init__(message)

class ProcessExecutionError(Exception):
    def __init__(self, stdout=None, stderr=None, exit_code=None, cmd=None,
                 description=None):
        self.exit_code = exit_code
        self.stderr = stderr
        self.stdout = stdout
        self.cmd = cmd
        self.description = description

        if description is None:
            description = "Unexpected error while running command."
        if exit_code is None:
            exit_code = '-'
        message = ("%s\nCommand: %s\nExit code: %s\nStdout: %r\nStderr: %r"
                   % (description, cmd, exit_code, stdout, stderr))
        super(ProcessExecutionError, self).__init__(message)

class NoRootWrapSpecified(Exception):
    def __init__(self, message=None):
        super(NoRootWrapSpecified, self).__init__(message)


def _subprocess_setup():
    # Python installs a SIGPIPE handler by default. This is usually not what
    # non-Python subprocesses expect.
    signal.signal(signal.SIGPIPE, signal.SIG_DFL)


def execute(*cmd, **kwargs):
    """Helper method to shell out and execute a command through subprocess.

    Allows optional retry.

    :param cmd:             Passed to subprocess.Popen.
    :type cmd:              string
    :param process_input:   Send to opened process.
    :type proces_input:     string
    :param check_exit_code: Single bool, int, or list of allowed exit
                            codes.  Defaults to [0].  Raise
                            :class:`ProcessExecutionError` unless
                            program exits with one of these code.
    :type check_exit_code:  boolean, int, or [int]
    :param delay_on_retry:  True | False. Defaults to True. If set to True,
                            wait a short amount of time before retrying.
    :type delay_on_retry:   boolean
    :param attempts:        How many times to retry cmd.
    :type attempts:         int
    :param run_as_root:     True | False. Defaults to False. If set to True,
                            the command is prefixed by the command specified
                            in the root_helper kwarg.
    :type run_as_root:      boolean
    :param root_helper:     command to prefix to commands called with
                            run_as_root=True
    :type root_helper:      string
    :param shell:           whether or not there should be a shell used to
                            execute this command. Defaults to false.
    :type shell:            boolean
    :returns:               (stdout, stderr) from process execution
    :raises:                :class:`UnknownArgumentError` on
                            receiving unknown arguments
    :raises:                :class:`ProcessExecutionError`
    """

    process_input = kwargs.pop('process_input', None)
    check_exit_code = kwargs.pop('check_exit_code', [0])
    ignore_exit_code = False
    delay_on_retry = kwargs.pop('delay_on_retry', True)
    attempts = kwargs.pop('attempts', 1)
    run_as_root = kwargs.pop('run_as_root', False)
    root_helper = kwargs.pop('root_helper', '')
    shell = kwargs.pop('shell', False)

    if isinstance(check_exit_code, bool):
        ignore_exit_code = not check_exit_code
        check_exit_code = [0]
    elif isinstance(check_exit_code, int):
        check_exit_code = [check_exit_code]

    if kwargs:
        raise UnknownArgumentError('Got unknown keyword args '
                                     'to utils.execute: %r' % kwargs)

    if run_as_root and os.geteuid() != 0:
        if not root_helper:
            raise NoRootWrapSpecified(
                message=('Command requested root, but did not specify a root '
                         'helper.'))
        cmd = shlex.split(root_helper) + list(cmd)

    cmd = map(str, cmd)

    while attempts > 0:
        attempts -= 1
        try:
            LOG.debug(_('Running cmd (subprocess): %s'), ' '.join(cmd))
            _PIPE = subprocess.PIPE  # pylint: disable=E1101

            if os.name == 'nt':
                preexec_fn = None
                close_fds = False
            else:
                preexec_fn = _subprocess_setup
                close_fds = True

            obj = subprocess.Popen(cmd,
                                   stdin=_PIPE,
                                   stdout=_PIPE,
                                   stderr=_PIPE,
                                   close_fds=close_fds,
                                   preexec_fn=preexec_fn,
                                   shell=shell)
            result = None
            if process_input is not None:
                result = obj.communicate(process_input)
            else:
                result = obj.communicate()
            obj.stdin.close()  # pylint: disable=E1101
            _returncode = obj.returncode  # pylint: disable=E1101
            if _returncode:
                LOG.debug(_('Result was %s') % _returncode)
                if not ignore_exit_code and _returncode not in check_exit_code:
                    (stdout, stderr) = result
                    raise ProcessExecutionError(exit_code=_returncode,
                                                stdout=stdout,
                                                stderr=stderr,
                                                cmd=' '.join(cmd))
            return result
        except ProcessExecutionError:
            if not attempts:
                raise
            else:
                LOG.debug(_('%r failed. Retrying.'), cmd)
                if delay_on_retry:
                    greenthread.sleep(random.randint(20, 200) / 100.0)
        finally:
            # NOTE(termie): this appears to be necessary to let the subprocess
            #               call clean something up in between calls, without
            #               it two execute calls in a row hangs the second one
            greenthread.sleep(0)


def trycmd(*args, **kwargs):
    """A wrapper around execute() to more easily handle warnings and errors.

    Returns an (out, err) tuple of strings containing the output of
    the command's stdout and stderr.  If 'err' is not empty then the
    command can be considered to have failed.

    :discard_warnings   True | False. Defaults to False. If set to True,
                        then for succeeding commands, stderr is cleared

    """
    discard_warnings = kwargs.pop('discard_warnings', False)

    try:
        out, err = execute(*args, **kwargs)
        failed = False
    except ProcessExecutionError as exn:
        out, err = '', str(exn)
        failed = True

    if not failed and discard_warnings and err:
        # Handle commands that output to stderr but otherwise succeed
        err = ''

    return out, err

def _run_shell_command(cmd, throw_on_error=False, buffer=True, env=None):
    if buffer:
        out_location = subprocess.PIPE
        err_location = subprocess.PIPE
    else:
        out_location = None
        err_location = None

    newenv = os.environ.copy()
    if env:
        newenv.update(env)

    output = subprocess.Popen(cmd,
                              stdout=out_location,
                              stderr=err_location,
                              env=newenv)
    out = output.communicate()
    if output.returncode and throw_on_error:
        print "%s returned %d" % (cmd, output.returncode)
    if len(out) == 0 or not out[0] or not out[0].strip():
        return ''
    return out[0].strip().decode('utf-8')

#=======================================
import datetime
def get_datetime(date, now=False):
    if now:
        now = time.utcnow()
        current_time = time.time(now.hour, now.minute, now.second)
    else:
        current_time = time.time()
    return datetime.datetime.combine(data, current_time)

def today(self):
    return time.today()
#=======================================

#=======================================
""" period task execute"""

def is_soon(dt, window):
    """Determines if time is going to happen in the next window seconds.

    :params dt: the time
    :params window: minimum seconds to remain to consider the time not soon

    :return: True if expiration is within the given duration
    """
    soon = (utcnow() + datetime.timedelta(seconds=window))
    return normalize_time(dt) <= soon

def delta_seconds(before, after):
    """Return the difference between two timing objects.

    Compute the difference in seconds between two date, time, or
    datetime objects (as a float, to microsecond resolution).
    """
    delta = after - before
    try:
        return delta.total_seconds()
    except AttributeError:
        return ((delta.days * 24 * 3600) + delta.seconds +
                float(delta.microseconds) / (10 ** 6))

class _PeriodicTasksMeta(type):
    def __init__(cls, names, bases, dict_):
        """Metaclass that allows us to collect decorated periodic tasks."""
        super(_PeriodicTasksMeta, cls).__init__(names, bases, dict_)

        # NOTE(sirp): if the attribute is not present then we must be the base
        # class, so, go ahead an initialize it. If the attribute is present,
        # then we're a subclass so make a copy of it so we don't step on our
        # parent's toes.
        try:
            cls._periodic_tasks = cls._periodic_tasks[:]
        except AttributeError:
            cls._periodic_tasks = []

        try:
            cls._periodic_last_run = cls._periodic_last_run.copy()
        except AttributeError:
            cls._periodic_last_run = {}

        try:
            cls._periodic_spacing = cls._periodic_spacing.copy()
        except AttributeError:
            cls._periodic_spacing = {}

        for value in cls.__dict__.values():
            if getattr(value, '_periodic_task', False):
                task = value
                name = task.__name__

                if task._periodic_spacing < 0:
                    LOG.info(_('Skipping periodic task %(task)s because '
                               'its interval is negative'),
                             {'task': name})
                    continue
                if not task._periodic_enabled:
                    LOG.info(_('Skipping periodic task %(task)s because '
                               'it is disabled'),
                             {'task': name})
                    continue

                # A periodic spacing of zero indicates that this task should
                # be run every pass
                if task._periodic_spacing == 0:
                    task._periodic_spacing = None

                cls._periodic_tasks.append((name, task))
                cls._periodic_spacing[name] = task._periodic_spacing
                cls._periodic_last_run[name] = task._periodic_last_run

DEFAULT_INTERVAL = 60.0

class PeriodicTasks(object):
    __metaclass__ = _PeriodicTasksMeta

    def run_periodic_tasks(self, context, raise_on_error=False):
        """Tasks to be run at a periodic interval.
		: _periodic_tasks: (task_name, task)		
		: context : task(context)
		: question : what does the idle_for mean to?
		: if there are a lots of task and the spaceing short, the task 
		: will not be expected which run the tast next time will delay a lot 
		: besides the task running time as much as short.
		"""
        idle_for = DEFAULT_INTERVAL
        for task_name, task in self._periodic_tasks:
            full_task_name = '.'.join([self.__class__.__name__, task_name])

            now = datetime.datetime.utcnow()
            spacing = self._periodic_spacing[task_name]
            last_run = self._periodic_last_run[task_name]

            # If a periodic task is _nearly_ due, then we'll run it early
            if spacing is not None and last_run is not None:
                due = last_run + datetime.timedelta(seconds=spacing)
                if not is_soon(due, 0.2):
                    idle_for = min(idle_for, delta_seconds(now, due))
                    continue

            if spacing is not None:
                idle_for = min(idle_for, spacing)

            LOG.debug(_("Running periodic task %(full_task_name)s"), locals())
            self._periodic_last_run[task_name] = datetime.datetime.utcnow()

            try:
                task(self, context)
            except Exception as e:
                if raise_on_error:
                    raise
                LOG.exception(_("Error during %(full_task_name)s: %(e)s"),
                              locals())
            time.sleep(0)

        return idle_for

#=======================================
"""
Import related utilities and helper functions.
"""

import sys
import traceback


def import_class(import_str):
    """Returns a class from a string including module and class."""
    mod_str, _sep, class_str = import_str.rpartition('.')
    try:
        __import__(mod_str)
        return getattr(sys.modules[mod_str], class_str)
    except (ValueError, AttributeError):
        raise ImportError('Class %s cannot be found (%s)' %
                          (class_str,
                           traceback.format_exception(*sys.exc_info())))


def import_object(import_str, *args, **kwargs):
    """Import a class and return an instance of it."""
    return import_class(import_str)(*args, **kwargs)


def import_object_ns(name_space, import_str, *args, **kwargs):
    """Tries to import object from default namespace.

    Imports a class and return an instance of it, first by trying
    to find the class in a default namespace, then failing back to
    a full path if not found in the default namespace.
    """
    import_value = "%s.%s" % (name_space, import_str)
    try:
        return import_class(import_value)(*args, **kwargs)
    except ImportError:
        return import_class(import_str)(*args, **kwargs)


def import_module(import_str):
    """Import a module."""
    __import__(import_str)
    return sys.modules[import_str]


def try_import(import_str, default=None):
    """Try to import a module and if it fails return default."""
    try:
        return import_module(import_str)
    except ImportError:
        return default


#=======================================
