FROM centos:7
    
RUN cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime && \
    echo "Asia/Shanghai" > /etc/timezone
#RUN localedef -v -c -i en_US -f UTF-8 en_US.UTF-8
#RUN yum install -y locales && locale-gen en_US.UTF-8 && update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
#ENV LC_ALL en_US.UTF-8
#RUN localectl set-locale LANG=en_US.UTF-8
#ENV LANGUAGE en_US.UTF-8

RUN curl https://mirrors.aliyun.com/repo/Centos-7.repo -o /etc/yum.repos.d/CentOS-Base.repo
RUN yum clean all
RUN yum makecache

RUN yum -y update &&\
	yum -y groupinstall development && \
	yum install -y gcc gcc-c++ cmake git wget make yasm.x86_64 \
		pkg-config autoconf automake libtool vim curl \
		zlib-devel bzip2-devel openssl-devel ncurses-devel sqlite-devel \
		readline-devel tk-devel

RUN mkdir /build
WORKDIR /build

# cc Setup
RUN echo $'export CC=gcc \n\
export CXX=g++ \n\
export LDFLAGS="-L/opt/local/lib" \n\
export CFLAGS="-Wall" \n\
export CXXFLAGS="-Wall" \n\
export CMAKE_C_FLAGS_RELEASE="-s" \n\
export CMAKE_CXX_FLAGS_RELEASE="-s"' > /opt/centos-base.env
RUN echo source /opt/centos-base.env >> ~/.bashrc

RUN mkdir /root/.ssh &&\
    ssh-keygen -t rsa -f /root/.ssh/id_rsa -q -P ""

#cmake Setup
COPY cmake.Makefile /build/
RUN ["/bin/bash", "-c", "source ~/.bashrc; make -C /build/ -f cmake.Makefile install clean"]

#yasm Setup
COPY yasm.Makefile /build/
#ADD yasm /build/yasm
RUN ["/bin/bash", "-c", "source ~/.bashrc; make -C /build/ -f yasm.Makefile install clean"]

# go1.13 Setup
COPY go113.Makefile /build/
RUN ["/bin/bash", "-c", "source ~/.bashrc; make -C /build/ -f go113.Makefile install clean"]
RUN echo $'export GOROOT="/usr/local/go113" \n\
export GOPATH=~/go \n\
export PATH="$GOROOT/bin:$GOPATH/bin:$PATH" \n\
export GOPROXY="https://goproxy.io,direct" \n\
export GO111MODULE=on' > /opt/go113-base.env
RUN echo source /opt/go113-base.env >> ~/.bashrc

# python3 Setup
COPY py3.Makefile /build/
RUN ["/bin/bash", "-c", "source ~/.bashrc; make -C /build/ -f py3.Makefile install clean"]

# entry Setup
WORKDIR /build

ADD pipeline /opt/pipeline

ENTRYPOINT ["/opt/pipeline"]
