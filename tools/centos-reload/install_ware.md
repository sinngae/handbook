## install nginx
`wget http://nginx.org/download/nginx-1.15.7.tar.gz`<br>
`tar zxf nginx-1.15.7.tar.gz`<br>
`cd nginx-1.`<br>
`./configure --user=nginx --group=nginx --prefix=/etc/nginx --sbin-path=/usr/sbin/nginx --conf-path=/etc/nginx/nginx.conf`<br>
`--pid-path=/var/run/nginx.pid --lock-path=/var/run/nginx.lock --error-log-path=/var/log/nginx/error.log `<br>
`--http-log-path=/var/log/nginx/access.log --with-http_gzip_static_module --with-http_stub_status_module `<br>
`--with-http_ssl_module --with-pcre --with-file-aio --with-http_realip_module --without-http_scgi_module `<br>
`--without-http_uwsgi_module --without-http_fastcgi_module`<br>

`./configure --prefix=/opt/local --with-http_gzip_static_module --with-http_stub_status_module --with-http_ssl_module --with-pcre --with-file-aio --with-http_realip_module --without-http_scgi_module --without-http_uwsgi_module --without-http_fastcgi_module`<br>

## install rust
`curl https://sh.rustup.rs -sSf|sh`<br>

## install yasm
`wget http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz`<br>
`tar zxvf yasm-1.3.0.tar.gz`<br>
`cd yasm-1.3.0`<br>
`./configure`<br>
`make && make install`<br>
