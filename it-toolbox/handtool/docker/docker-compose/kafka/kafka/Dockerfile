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
	yum install -y gcc git wget vim curl

# kafka Setup
# 参考https://www.digitalocean.com/community/tutorials/how-to-install-apache-kafka-on-centos-7

# entry Setup
RUN mkdir /work
WORKDIR /work

ADD pipeline /opt/pipeline

ENTRYPOINT ["/opt/pipeline"]