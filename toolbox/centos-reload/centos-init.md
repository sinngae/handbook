# centos mininal
```sh
# 无UI安装，/目录50G，/home 100G，
#   /usr/ 安装centos的yum库软体
#   /usr/local/用于安装git库软体等
#   /usr/local/src 用于存放源码

# 时区修改为上海时间
mv /etc/localtime /etc/localtime.bk
ln -s /usr/share/zoneinfo/Asia/Hong_Kong /etc/localtime

# 更换更新镜像源为阿里云镜像源
mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup
wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo
yum clean all
yum makecache

# 安装第三方源
yum install epel-release

# 编译python3
wget https://www.python.org/ftp/python/3.7.1/Python-3.7.1.tgz
yum groupinstall "Development Tools"
yum -y install zlib-devel  libpcap-devel xz-devel wget openssl-devel bzip2-devel expat-devel ncurses-devel gdbm-devel readline-devel sqlite-devel tk-devel libffi libffi-devel e2fsprogs-devel libuuid-devel tcl
# ./configure --prefix=/usr/local --enable-optimizations --with-ssl-default-suites=openssl --enable-shared --with-openssl=/usr --with-cxx-main=g++
./configure --prefix=/usr/local --with-ssl-default-suites=openssl --enable-shared --with-openssl=/usr

# core pattern
sysctl -w kernel.core_pattern=/tmp/core-%e-%p
sysctl -a|grep kernel.core

# 安装资源监控工具
#   iotop iftop
yum install iotop iftop
#   network
yum install -y bind-utils
#   performance
yum install -y sysstat


# 安装Clang检测工具cppcheck
wget https://github.com/danmar/cppcheck/archive/1.87.tar.gz
tar zxf 1.87.tar.gz && cd 1.87
mkdir /root/cppcheck_cfg
cp -r cfg/* /root/cppcheck_cfg/
make SRCDIR=build CFGDIR=/root/cppcheck_cfg HAVE_RULES=yes
make install CFGDIR=/root/cppcheck_cfg
#   使用示例<br>
cppcheck [file|dir] --enable=warning > a.out 2>&1
cppcheck [file|dir] --enable=all> a.out 2>&1

# 安装代码统计工具cloc
#   下载：git clone https://github.com/AlDanial/cloc.git
#   加入环境变量path：export PATH=$PATH:/...<br>
#   使用示例：<br>
cloc [filename|dirpath]<br>
```