#!/usr/bin/env python
# coding=utf-8

'''
premute and combine numbers 1,2,3,4 into three non-repetitive order
'''

for i in range(1, 5):
    for j in range(1, 5):
        for k in range(1, 5):
            if (i != k) and (i != j) and (j!= k):
                print i,j,k
