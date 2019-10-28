## centos mininal
无UI安装，/目录50G，/home 100G，<br>
/usr/ 安装centos的yum库软体<br>
/usr/local/用于安装git库软体等<br>
/usr/local/src 用于存放源码<br>

## 时区修改为上海时间
`mv /etc/localtime /etc/localtime.bk`<br>
`ln -s /usr/share/zoneinfo/Asia/Hong_Kong /etc/localtime`<br>

## 更换更新镜像源为阿里云镜像源
`mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup`<br>
`wget -O /etc/yum.repos.d/CentOS-Base.repo http://mirrors.aliyun.com/repo/Centos-7.repo`<br>
`yum clean all`<br>
`yum makecache`<br>

## 编译python
`wget https://www.python.org/ftp/python/3.7.1/Python-3.7.1.tgz`<br>
`yum groupinstall "Development Tools"`<br>
`yum -y install zlib-devel  libpcap-devel xz-devel wget openssl-devel bzip2-devel expat-devel ncurses-devel gdbm-devel readline-devel sqlite-devel tk-devel libffi libffi-devel e2fsprogs-devel libuuid-devel tcl`<br>
<del>./configure --prefix=/usr/local --enable-optimizations --with-ssl-default-suites=openssl --enable-shared --with-openssl=/usr --with-cxx-main=g++</del><br>
`./configure --prefix=/usr/local --with-ssl-default-suites=openssl --enable-shared --with-openssl=/usr`<br>


## 编译vim
`git clone https://github.com/vim/vim.git`<br>
`./configure --enable-multibyte --enable-python3interp=yes --with-python-command=/usr/bin/python --with-python-config-dir=/usr/lib64/python2.7/config  --with-python3-command=/usr/local/bin/python3  --with-python3-config-dir=/usr/local/lib/python3.7/config-3.7m-x86_64-linux-gnu --prefix=/usr/local`<br>

## 编译YouCompleteMe
`git clone https://github.com/VundleVim/Vundle.vim.git`<br>
`git clone https://github.com/Valloric/YouCompleteMe.git`<br>
`install libncurses5-dev libgnome2-dev libgnomeui-dev libgtk2.0-dev libatk1.0-dev libbonoboui2-dev libcairo2-dev libx11-dev libxpm-dev libxt-dev python-dev python3-dev ruby-dev lua5.1 liblua5.1-dev libperl-dev git`<br>
`git submodule update --init --recursive`<br>
`python3 install.py --clang-completer`<br>

## clang llvm

## core pattern
`sysctl -w kernel.core_pattern=/tmp/core-%e-%p`<br>
`sysctl -a|grep kernel.core`<br>


## 安装第三方源
`yum install epel-release`<br>
## 安装资源监控工具
`yum install iotop iftop`<br>

## 安装cppcheck
`wget https://github.com/danmar/cppcheck/archive/1.87.tar.gz`<br>
`tar zxf 1.87.tar.gz && cd 1.87`<br>
`mkdir /root/cppcheck_cfg`<br>
`cp -r cfg/* /root/cppcheck_cfg/`<br>
`make SRCDIR=build CFGDIR=/root/cppcheck_cfg HAVE_RULES=yes`<br>
`make install CFGDIR=/root/cppcheck_cfg`<br>
使用示例<br>
`cppcheck [file|dir] --enable=warning > a.out 2>&1`<br>
