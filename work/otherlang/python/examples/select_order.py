#!/usr/bin/env python
# coding=utf-8

if __name__ == "__main__":
    N = 10
    print 'input ten numbers:\n'
    l = []
    for i in range(N):
        l.append(int(raw_input('input a number:\n')))
    print 
    for i in range(N):
        print l[i]
    print

    for i in range(N - 1):
        min = i
        for j in range(i + 1, N):
            if l[min] > l[j]: min = j
        l[i],l[min] = l[min],l[i]

    print 'after order:'
    for i in range(N):
        print l[i]
