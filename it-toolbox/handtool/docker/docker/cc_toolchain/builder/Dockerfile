FROM centos-base:master

# include lib Setup
RUN mkdir -p /opt/local/include
RUN mkdir -p /opt/local/lib
RUN mkdir -p /opt/local/static_lib
#ADD include /opt/local/include
#ADD lib /opt/local/lib
#ADD static_lib /opt/local/static_lib

RUN yum install -y \
	json-c-devel.x86_64 \
	lua-devel.x86_64 \
    openssl-devel.x86_64 \
	libcurl-devel.x86_64 \
    libev-devel.x86_64

# cc Setup
RUN echo $'export CFLAGS="-I/usr/local/include $CFLAGS "\n\
	    export CXXFLAGS="-I/usr/local/include $CXXFLAGS "\n\
	    export LDFLAGS="-L/opt/local/lib $LDFLAGS" \n\
		export LD_LIBRARY_PATH="/usr/local/lib:/usr/local/lib64"' > /opt/cc-builder.env
RUN echo source /opt/cc-builder.env >> ~/.bashrc


# zlog
COPY zlog.Makefile /build/
RUN ["/bin/bash", "-c", "source /opt/build.env; make -C /build/ -f zlog.Makefile install clean"]


# entry Setup
WORKDIR /build

ADD pipeline /opt/pipeline

ENTRYPOINT ["/opt/pipeline"]
