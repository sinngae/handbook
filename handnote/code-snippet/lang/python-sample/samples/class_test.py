#!/usr/bin/env python
# coding=utf-8

class bcolors:
    header = '\033[95m'
    okblue = '\033[94m'
    okgreen = '\033[92m'
    warning = '\033[93m'
    fail = '\033[91m'
    endc = '\033[0m'
    bold = '\033[1m'
    underline = '\033[4m'

print bcolors.endc
print bcolors.warning + 'waring font?' + bcolors.endc
print bcolors.underline  + 'adff' + bcolors.endc
