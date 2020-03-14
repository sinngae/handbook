#!/bin/bash
# 手动同步
#   yum install -y ntp
ntpdate time.nist.gov

# 时间自动同步
#   crontab -e
#   添加：*/10 * * * * ntpdate time.nist.gov    # 每隔十分钟同步一次
#   添加：30 5 * * * ntpdate time.nist.gov      # 每天5点30分同步一次

# 几个时间服务器
#   time.nist.gov
#

# 相关命令
#   hwclock --show      # CMOS时间，硬件时间
#   date -R             # 系统时间
#   clock --systohc     # 硬件时间同步系统时间