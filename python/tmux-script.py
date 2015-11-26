#! /usr/bin/env python
import subprocess
import os

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

left_commands = [
    "vim testl1",
    "vim testl2",
    "vim testl3",
]
right_commands = [
    "vim testr1",
    "vim testr2",
]

def tmux_panel(left_commands, right_commands):

    session = ''
    if right_commands:
        session += 'tmux selectp -t 0; tmux splitw -hd -p 35 \"%s\"; ' % right_commands[-1]
    for index, command in enumerate(right_commands[:-1]):
        session += 'tmux selectp -t 1; tmux splitw -d -p %i \"%s\"; ' % (
            100 / (len(right_commands) - index),
            command
        )

    for index, command in enumerate(left_commands[1:]):
        session += 'tmux selectp -t 0; tmux splitw -d -p %i \"%s\"; ' % (
            100 / (len(left_commands) - index),
            command
        )
    if left_commands:
        session += left_commands[0]

    args = [
        'tmux',
        'new-session',
        session,
    ]

    print 'Running ', args
    _run_shell_command(args)

if __name__== "__main__":
    tmux_panel(left_commands, right_commands)
