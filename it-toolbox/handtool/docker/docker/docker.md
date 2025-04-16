# Docker

```sh
# 1.centos 安装docker
#   docker要求linux内核版本在3.10以上
uname -r    # 查看内核版本

# 2.安装docker
yum -y install docker-io

# 3.启动docker服务
service docker start

# 4.测试hello-world
docker run hello-world

# 5.docker使用
docker run centos:7 /bin/echo "hi,world"
docker run -i -t centos:7 /bin/bash
#   -i 允许与容器内的标准输入交互
#   -t 在新容器内指定一个伪终端或终端
#   可使用exit命令或CTRL+D退出容器
docker run -d centos:7 /bin/sh -c "while true; do echo hello world; sleep 1; done" 

# 6.返回一串长字符，是容器ID，用来查看容器内发生了什么
docker ps                           # 查看容器在运行，-l 查询最后一次创建的容器
docker logs [容器id|容器名]         # 查看容器的标准输出
# -f 像tail -f 一样来输出容器的标准输出

docker stop [容器id|容器名]         # 停止容器
docker start [容器id|容器名]        # 启动容器
docker rm [容器id|容器名]           # 删除容器

### 1.容器使用

docker ps -a                    # 查看所有命令
docker stats                    # 查看虚拟cpu使用io使用等
docker top [容器id|容器名]       # 查看容器内运行情况
docker pull training/webapp     # 载入镜像
docker run -d -P training/webapp python app.py
# -d 让容器在后台运行<br>
# -P 将容器内部使用到的网络端口映射到我们使用的主机上
# docker ps可以查看到端口映射信息
docker run -d -p 5000:5000 training/webapp python app.py
# -p 设置不一样的端口
docker port [容器id|容器名称]   # 查看端口映射情况
docker inspect [容器id|容器名]
# 查看docker底层信息，返回一个json记录着docker容器配置和状态
docker attach [容器id|容器名称] # 进入一个正在运行的容器
docker exec -it [容器id|容器名称] /bin/bash # 进入一个正在运行的容器
docker inspect -f '{{.ID}}' [容器id|容器名称]
docker ps [local path] [上一命令的结果]:[container path]
# 从本地拷贝文件到容器
docker cp src dest


# 由image获取镜像制作的一些信息
docker history [image url]
docker history [image url]  --no-trunc
```

### 2.使用镜像
运行容器时，如果本地不存在，则从仓库中下载，默认从Docker Hub下载

2.1 管理和使用本地Docker主机镜像  
```sh
docker images                       # 列出本地主机上的镜像 TAG列，代表这个仓库的不同的版本
docker pull [镜像名]:[版本号]        # 拉取镜像
docker search [镜像名]              # 搜索镜像
```

2.2 创建镜像  
两种方式：  
+ 1.从已创建的容器中更新镜像，并且提交这个镜像
+ 2.使用Dockerfile指令创建一个新的镜像
```sh
# docker commit -m="commit log" -a="runoob" [容器id|容器名] runoob/[镜像名]:[版本号]
# -m 提交描述<
# -a 指定镜像作者
docker build -t
```

### nginx
docker pull nginx:latest


### redis 
docker pull redis

redis 实例  
docker run --name some-redis -d redis

redis 持久化 实例  
docker run --name some-redis -d redis redis-server --appendonly yes
