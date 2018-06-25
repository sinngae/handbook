#!/usr/bin/env python
# coding=utf-8

def fib(n):
    a,b = 1,1
    for i in range(n-1):
        a,b = b,a+b
    return a

def fib1(n):
    if n == 1 or n == 2:
        return 1
    return fib1(n - 1) + fib1(n - 2)

def fib2(n):
    if n == 1:
        return [1]
    if n == 2:
        return [1, 1]
    fibs = [1, 1]
    for i in range(2, n):
        fibs.append(fibs[-1] + fibs[-2])
    return fibs

n = int(raw_input('n:\n'))
print fib2(n)
