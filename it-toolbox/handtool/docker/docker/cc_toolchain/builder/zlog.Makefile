.PHONY: all install clean

all: install

zlog-1.2.12.tar.gz:
	wget https://github.com/HardySimpson/zlog/archive/1.2.12.tar.gz -O zlog-1.2.12.tar.gz

zlog-1.2.12: zlog-1.2.12.tar.gz
	tar xzvf zlog-1.2.12.tar.gz && \
    touch zlog-1.2.12

/opt/local/lib/libzlog.a: zlog-1.2.12
	make CFLAGS=-D_DEFAULT_SOURCE=1 -C zlog-1.2.12 PREFIX=/opt/local install

install: /opt/local/lib/libzlog.a

clean:
	rm -rf zlog-1.2.12 zlog-1.2.12.tar.gz
