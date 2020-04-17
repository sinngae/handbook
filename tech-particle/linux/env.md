# env

+ 全部用户配置：/etc/profile
+ 当前用户配置：~/.bashrc ？ ~/.bash_profile

可使用echo $变量名 查看

printenv 打印变量名

常用变量
PATH              |     bin目录
------------------|--------------
LD_LIBRARY_PATH   |     动态库目录
LIBRARY_PATH      |     静态库目录
C_INCLUDE_PATH    |     C头文件目录
CPLUS_INCLUDE_PATH|     C++头文件目录

在/etc/ld.so.conf後添加lib路徑，然後執行ldconfig生效

env             |   含义
----------------|-----------
HOME            |
SHELL           | shell类型
HISTSIZE        | history大小
MAIL            | 邮箱
LANG            | 语言
RANDOM          | 随机数

# locale

# sys
```sh
# 查看 "kernel version"
cat /proc/version
# 查看 "system version"
lsb_release -a
cat /etc/os-release
cat /etc/redhat-release
rpm -q centos-release
uname -a

# 以ls为例查看"program info"
file /bin/ls
# 查看机器字长
getconf LONG_BIT

# 查看 "cpu info"
cat /proc/cpuinfo |grep name 
```