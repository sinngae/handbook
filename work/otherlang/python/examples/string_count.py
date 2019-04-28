#!/usr/bin/env python
# coding=utf-8

import string
s = raw_input('input a string:\n')
letters = 0
space = 0
digit = 0
others = 0
i = 0
#for c in s:
while i < len(s):
    c = s[i]
    i += 1
    if c.isalpha():
        letters += 1
    elif c.isspace():
        space += 1
    elif c.isdigit():
        digit += 1
    else:
        others += 1
print 'char = %d, space = %d, digit = %d, others = %d' % (letters, space, digit, others)
