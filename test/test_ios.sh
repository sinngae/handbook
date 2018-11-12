#!/bin/bash

echo "kernel versio:"
#cat /proc/version
uname -a
# "program info"
#file /bin/ls
# getconf LONG_BIT

echo ""

echo "system version"
lsb_release -a
# cat /etc/os-release
#cat /etc/redhat-release
#rpm -q centos-release

echo "cpu info"
cat /proc/cpuinfo |grep name 
