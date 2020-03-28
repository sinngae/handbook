# NTP

## ntp协议

## 相关命令
```sh
# 安装
yum -y install ntp

# 手工发起同步
ntpdate ntp1.aliyun.com
# 启动NTP服务
service ntpd start
# 设置开机启动
chkconfig ntpd on
# 检查设置
chkconfig --list ntpd

# 时间自动同步
#   crontab -e
#   添加：*/10 * * * * ntpdate time.nist.gov    # 每隔十分钟同步一次
#   添加：30 5 * * * ntpdate time.nist.gov      # 每天5点30分同步一次

# 相关命令
hwclock --show      # CMOS时间，硬件时间
date -R             # 系统时间
clock --systohc     # 硬件时间同步系统时间
```

### 几个时间服务器
+ Windows系统上自带的两个：time.windows.com 和 time.nist.gov
+ MacOS上自带的两个：time.apple.com 和 time.asia.apple.com
+ NTP授时快速域名服务：cn.ntp.org.cn 中国授时
+ NTP授时快速域名服务：cn.ntp.org.cn 中国授时

#### 阿里云 NTP服务器
+ time.pool.aliyun.com
+ ntp.aliyun.com             
+ ntp1.aliyun.com
+ ntp2.aliyun.com
+ ntp3.aliyun.com
+ ntp4.aliyun.com
+ ntp5.aliyun.com
+ ntp6.aliyun.com
+ ntp7.aliyun.com

**Time服务器**
+ time1.aliyun.com
+ time2.aliyun.com
+ time3.aliyun.com
+ time4.aliyun.com
+ time5.aliyun.com
+ time6.aliyun.com
+ time7.aliyun.com

#### 大学授时
+ s1c.time.edu.cn       北京大学 
+ s2m.time.edu.cn       北京大学
+ s1b.time.edu.cn       清华大学
+ s1e.time.edu.cn       清华大学
+ s2a.time.edu.cn       清华大学
+ s2b.time.edu.cn       清华大学

#### 苹果提供的授时服务器   
+ time1.apple.com
+ time2.apple.com
+ time3.apple.com
+ time4.apple.com
+ time5.apple.com
+ time6.apple.com
+ time7.apple.com

#### Google提供的授时服务器   
+ time1.google.com
+ time2.google.com
+ time3.google.com
+ time4.google.com