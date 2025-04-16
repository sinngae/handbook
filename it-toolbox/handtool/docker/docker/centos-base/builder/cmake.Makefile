.PHONY: all install clean

all: install

cmake-3.14.1.tar.gz:
	wget https://github.com/Kitware/CMake/releases/download/v3.14.1/cmake-3.14.1.tar.gz

cmake-3.14.1: cmake-3.14.1.tar.gz
	tar xzf cmake-3.14.1.tar.gz && \
    touch cmake-3.14.1

/usr/local/bin/cmake: cmake-3.14.1
	cd cmake-3.14.1 && \
	./bootstrap && \
	gmake && gmake install

install: /usr/local/bin/cmake

clean:
	rm -rf cmake-3.14.1 cmake-3.14.1.tar.gz
