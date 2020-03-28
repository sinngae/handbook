# 概述
此文件夹用于记录重装CentOS后安装一些软件或组件。包括开发工具、第三方库、编程语言等等。
其中，大部分都是可以迁移到docker镜像中的，但为了更敏捷地开发，还是在本地也安装一下。

## install nginx
```sh
wget http://nginx.org/download/nginx-1.15.7.tar.gz

tar zxf nginx-1.15.7.tar.gz
cd nginx-1.15.7
## 生产环境编译
./configure --user=nginx --group=nginx --prefix=/etc/nginx --sbin-path=/usr/sbin/nginx --conf-path=/etc/nginx/nginx.conf
--pid-path=/var/run/nginx.pid --lock-path=/var/run/nginx.lock --error-log-path=/var/log/nginx/error.log 
--http-log-path=/var/log/nginx/access.log --with-http_gzip_static_module --with-http_stub_status_module 
--with-http_ssl_module --with-pcre --with-file-aio --with-http_realip_module --without-http_scgi_module 
--without-http_uwsgi_module --without-http_fastcgi_module
## 开发环境编译
./configure --prefix=/opt/local --with-http_gzip_static_module --with-http_stub_status_module --with-http_ssl_module --with-pcre --with-file-aio --with-http_realip_module --without-http_scgi_module --without-http_uwsgi_module --without-http_fastcgi_module
```

## install rust
`curl https://sh.rustup.rs -sSf|sh`

## install yasm
```sh
wget http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz
tar zxvf yasm-1.3.0.tar.gz
cd yasm-1.3.0
./configure
make && make install
```