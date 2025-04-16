FROM centos-base:master


RUN source ~/.bashrc

WORKDIR /build

ADD pipeline /opt/pipeline

ENTRYPOINT ["/opt/pipeline"]
