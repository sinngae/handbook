#!/usr/bin/env python
# coding=utf-8

def fact(j):
    sum = 0
    if j == 0:
        sum = 1
    else:
        sum = j * fact(j - 1)
    return sum
print fact(5)


def output(s, l):
    if l == 0:
        return 
    print s[l - 1]
    output(s, l - 1)

s = raw_input('input a string: ')
l = len(s)
output(s, l)

def age(n):
    if n == 1: c = 10
    else: c = age(n - 1) + 2
    return c
print age(5)
