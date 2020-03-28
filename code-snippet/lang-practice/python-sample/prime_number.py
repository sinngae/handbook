#!/usr/bin/env python
# coding=utf-8

h = 0
leap = 1
from math import sqrt
from sys import stdout
for m in range(101, 201):
    k = int(sqrt(m+1))
    for i in range(2, k+1):
        if m % i == 0:
            leap = 0
            break
    if leap == 1:
        print '%-4d' % m
        h += 1
        if h % 10 == 0:
            print ''
    leap = 1
print 'the total is %d' % h

print

lower = int(raw_input('input the min value: '))
upper = int(raw_input('imput the max value: '))

for num in range(lower, upper + 1):
    if num > 1:
        for i in range(2, num):
            if (num % i) == 0:
                break
        else:
            print(num)
