#! /usr/bin/env python
# -*- coding:utf-8 -*-

#Author : Wenxueliu
#Date: 2013-07-09
#From: dive into python

"""
This module will search for scripts in the same directory named 
XYZtest.py. Each such script should be a test suite that tests 
a module through PyUnit. This script will aggregate all found test
suites into one big test suite and run them all at once.
"""

import sys, os, re, unittest

def regressionTest():

    path = os.path.abspath(os.path.dirname(sys.argv[0]))
    files = os.listdir(path)
    test = re.compile("test\.py", re.IGNORECASE)
    files = filter(test.search, files)
    file2Module = lambda f: os.path.splitext(f)[0]
    moduleNames = map(file2Module, files)
    modules = map(__import__, moduleNames)
    load = unittest.defaultTestLoader.loadTestsFromModule
    return unittest.TestSuite(map(load, modules))

if __name__ == "__main__":
    print 'sys.argv[0]', sys.argv[0]
    pathname = os.path.dirname(sys.argv[0])
    print 'path = ' , pathname
    pathname = os.path.abspath(sys.argv[0])
    print 'abspath = ' , pathname  
    path = os.getcwd()
    print os.listdir(path)
