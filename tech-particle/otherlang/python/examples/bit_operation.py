#!/usr/bin/env python
# coding=utf-8

if __name__ == '__main__':
    a = 077
    b = a & 3
    print b
    b &= 7
    print b
    b = a | 3
    print b
    b |= 7
    print b
    b = a ^ 3
    print b
    b ^= 7
    print b

    print 

    a = int(raw_input('input a number:\n'))
    b = a >> 4
    # ~按位取反
    c = ~(~0 << 4)
    d = b & c
    print '%o\t%o' % (a,d)

    a = 234
    b = ~a
    print b
    a = ~a
    print a
