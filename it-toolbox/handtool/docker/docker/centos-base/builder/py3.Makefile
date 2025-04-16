.PHONY: all install clean

all: install

Python-3.8.7.tgz:
	wget https://www.python.org/ftp/python/3.8.7/Python-3.8.7.tgz

install:Python-3.8.7.tgz
	tar zxf Python-3.8.7.tgz && \
    cd Python-3.8.7 && \
	./configure --prefix=/usr/local/python3 && \
	make && make install

clean:
	-rm -rf Python-3.8.7 Python-3.8.7.tgz