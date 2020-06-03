#!/usr/bin/env python
# coding=utf-8

Tn = 0
Sn = []
n = int(raw_input('n = '))
a = int(raw_input('a = '))
for count in range(n):
    Tn = Tn + a
    a = a * 10
    Sn.append(Tn)
    print Tn

Sn = reduce(lambda x,y : x + y, Sn)
print "sum = ", Sn


maximum = lambda x,y : (x > y)*x + (x < y)*y
minimum = lambda x,y : (x > y)*y + (x < y)*x

if __name__ == '__main__':
    a = 10
    b = 20
    print maximum(a,b)
    print minimum(a,b)
