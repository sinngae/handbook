#!/usr/bin/env python
# coding=utf-8

true = 1
false = 0

def sq(x):
    return x * x

print 'if input a number smaller than 50, then exit'

again = 1
while again:
    num = int(raw_input('input a number: '))
    print 'result: %d' % (sq(num))
    if sq(num) >= 50:
        again = true
    else:
        again = false
