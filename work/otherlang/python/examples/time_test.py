#!/usr/bin/env python
# coding=utf-8

import time, datetime

print time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time()))

time.sleep(1)

print time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time()))

time.sleep(0.5)

TIME = datetime.datetime.now()
print time.strftime("%Y-%m-%d %H:%M:%S")

print time.ctime(time.time())
print time.asctime(time.localtime(time.time()))
print time.asctime(time.gmtime(time.time()))

start = time.time()
sum = 0
for i in range(3000):
    sum += i
end = time.time()
print end - start

start = time.clock()
sum = 0
for i in range(1000000):
    sum += i
end = time.clock()
print '%6.3f' % (end - start)
