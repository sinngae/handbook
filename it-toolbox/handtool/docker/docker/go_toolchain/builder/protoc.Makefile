.PHONY: all install clean

all: install

v3.14.0.tar.gz:
	wget https://github.com/protocolbuffers/protobuf/archive/v3.14.0.tar.gz

install:v3.14.0.tar.gz
	tar zxf v3.14.0.tar.gz && \
	cd protobuf-3.14.0 && \
	./autogen.sh && \
	./configure && \
	make && make install

clean:
	-rm -rf v3.14.0.tar.gz
