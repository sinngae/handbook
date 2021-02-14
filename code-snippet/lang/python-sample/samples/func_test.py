#!/usr/bin/env python
# coding=utf-8

def hi_wd():
    print 'hi, world'

def three_hiwd():
    for i in range(3):
        hi_wd()

if __name__ == '__main__':
    three_hiwd()
