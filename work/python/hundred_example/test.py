#!/usr/bin/env python
#coding=utf-8
'''
file test.py
'''

"""
this is a test
"""
print("hi, python")
print("你好，蛇蛇")
print "hi,another"

print "",
print "\n",

import sys
print sys.argv[0]
if len(sys.argv) > 1 :
    print sys.argv[1]


print "","hi"
print ord('1')
print ord('9')
print ord('a')
print ord('A')

try:
    raise ValueError
except Exception as e:
    raise IndexError from e
