.PHONY: all install clean

all: install

yasm-1.3.0.tar.gz:
	wget http://www.tortall.net/projects/yasm/releases/yasm-1.3.0.tar.gz
#	git clone https://github.com/yasm/yasm.git

install:yasm-1.3.0.tar.gz
	tar zxf yasm-1.3.0.tar.gz && \
	cd yasm-1.3.0 && mkdir build && cd build && \
	cmake .. && \
	make && make install

clean:
	-rm -rf yasm-1.3.0
	-rm -rf yasm-1.3.0.tar.gz 
	-rm -rf build
