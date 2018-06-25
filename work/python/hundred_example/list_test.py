#!/usr/bin/env python
# coding=utf-8

a = ['one', 'two', 'three']
for i in a[::-1]:
    print i

print

test_list = ['10086', 'zgyd', [1,2,3,4,5]]
print len(test_list)
print test_list[1:]
test_list.append('i\'m new here')
print len(test_list)
print test_list[-1]
print test_list.pop(1)
print len(test_list)
print test_list

print 

matrix = [[1, 2, 3],
        [4, 5, 6],
        [7, 8, 9]]
print matrix
print matrix[1]
col2 = [row[1] for row in matrix]
print col2
col2even = [row[1] for row in matrix if row[1]%2 == 0]
print col2even

print

L = [1,2,3,4,5]
s1 = ','.join(str(n) for n in L)
print s1

data = []
data.append(['', '', []])
data[0][0] = 'lilei'
data[0][1] = 'boy'
data[0][2].append(90)
data[0][2].append(93)
data[0][2].append(89)

data.append(['', '', []])
data[1][0] = 'hanmeimei'
data[1][1] = 'girl'
data[1][2].append(100)
data[1][2].append(103)
data[1][2].append(124)
print data
data.reverse()
print data
for i in range(len(data)):
    print data[i]

a = [1, 3, 2]
b = [4, 5, 6]
a.sort()
print a

print a + b

a.extend(b)
print a
