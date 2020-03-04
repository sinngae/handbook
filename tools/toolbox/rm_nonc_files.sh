#!/bin/bash 
# 删除同级文件夹下非.c文件 慎用
#rm -f $(ls -l |grep "^-" |awk '{print $9}'|grep -v ".c"|grep -v "clean")
