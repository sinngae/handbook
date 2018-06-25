#!/usr/bin/env python
# coding=utf-8

n = 0
s = 0
t = 1
for n in range(1, 21):
    t *= n
    s += t
print s
