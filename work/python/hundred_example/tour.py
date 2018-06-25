#!/usr/bin/env python
# coding=utf-8

tour = []
height = []

hei = 100.0
tim = 10 

for i in range(1, tim + 1):
    if i == 1:
        tour.append(hei)
    else:
        tour.append(2*hei)
    hei /= 2
    height.append(hei)

print('sum_height: tour = {0}'.format(sum(tour)))
print('10th height = {0}'.format(height[-1]))
