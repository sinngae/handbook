FROM centos-base:master

# supervisor Setup
RUN yum -y install epel-release
RUN yum -y install python-pip supervisor


WORKDIR /workspace

ADD pipeline /opt/pipeline

ENTRYPOINT ["/opt/pipeline"]
