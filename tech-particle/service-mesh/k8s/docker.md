# docker

## 安装docker-ce
docker-ce是社区版，还有企业版等
```sh
# 安装
yum install -y yum-utils device-mapper-persistent-data lvm2
yum-config-manager --add-repo https://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
yum-config-manager --add-repo https://packages.docker.com/1.10/yum/repo/main/centos/7
yum makecache fast
yum install docker-ce
# 安装docker命令补全
yum install bash_completion
curl -L https://raw.githubusercontent.com/docker/docker/v$(docker version -f "{{.Client.Version}}")/contrib/completion/bash/docker -o /etc/bash_completion.d/docker
vim ~/.bash_profile
# 添加如下内容
#   [ -f /etc/bash_completion.d ] && . /etc/bash_completion.d
```

## docker 命令
### 启动
```sh
docker run --entrypoint "" -it --rm --name [container name] -v/data/disk:/data -v/data/disk/sdx:/media/sdx --privileged=true [image name]:master /bin/bash

# 连上测试平台
# --privileged=true --add-host [hostname1] --add-host [hostname2] 
# 导出
docker save [image id] > [xxx.tar]
# 导入
docker load < [xxx.tar]
```

+ docker退出容器，而不关闭容器：
     ctrl+p+q
+ docker退出容器，并且关闭容器：
     exit

### 上传镜像
```sh
# 1.在Linux下，先执行：
docker login [registry.host]
# 登录registry，输入账号、密码。

# 2.修改镜像的tag
docker tag [image name]:master [registry.host.remote.image]:master
# 3.然后执行docker push命令，将镜像推到registry中
docker push registry.xunlei.cn/xl_yy/virtual_onecloud
# 4.使用镜像
docker pull [registry.host]/[remoe.image.name]:master
# 输入docker hub账号密码
# 5.docker 清理
docker system prune -a
# 6.调试
docker exec -it f83d0af9af19 /bin/bash # 在docker里执行