.PHONY: all install clean

all: install

go1.13.15.linux-amd64.tar.gz:
	wget https://dl.google.com/go/go1.13.15.linux-amd64.tar.gz

install:go1.13.15.linux-amd64.tar.gz
	tar zxf go1.13.15.linux-amd64.tar.gz && \
	mv go /usr/local/go113

clean:
	-rm -rf go1.13.15.linux-amd64.tar.gz
