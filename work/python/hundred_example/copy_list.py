#!/usr/bin/env python
# coding=utf-8

a = [1, 2, 3]
b = a
a.append(4)
c = a[:]
a.append(5)
print b
print c
