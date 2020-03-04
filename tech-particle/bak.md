
# log4cplus/openssl/protobuf编译
log4cplus库处理
编辑Makefile文件
CFLAGS+=-fPIC
CXXFLAGS+=-fPIC
./configure--prefix=/usr/local/log4cplus/--with-pic

openssl库处理：
./config--prefix=/usr/local/openssl-fPIC
make
makeinstall

protobuf库处理：
编辑Makefile文件
CFLAGS+=-fPIC
CXXFLAGS+=-fPIC
CPPFLAGS+=-fPIC

