#!/usr/bin/python
# -*- utf-8 -*-

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

