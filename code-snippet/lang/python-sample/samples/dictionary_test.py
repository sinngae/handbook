#!/usr/bin/env python
# coding=utf-8

persons = {'li':18, 'wang':50, 'zhang':20, 'sum':22}
m = 'li'
for key in persons.keys():
    if persons[m] < persons[key]:
        m = key

print m,persons[m]
