## centos 安装docker
1.docker要求linux内核版本在3.10以上<br>
uname -r查看内核版本<br>
2.安装docker<br>
yum -y install docker-io<br>
3.启动docker服务<br>
service docker start<br>
4.测试hello-world<br>
docker run hello-world<br>

## docker使用
docker run centos:7 /bin/echo "hi,world"<br>
docker run -i -t centos:7 /bin/bash <br>
// -i 允许与容器内的标准输入交互<br>
// -t 在新容器内指定一个伪终端或终端<br>
// 可使用exit命令或CTRL+D退出容器<br>
docker run -d centos:7 /bin/sh -c "while true; do echo hello world; sleep 1;<br>
done" //返回一串长字符，是容器ID，用来查看容器内发生了什么<br>
docker ps //查看容器在运行，-l 查询最后一次创建的容器<br>
docker logs [容器id|容器名] //查看容器的标准输出 <br>
// -f 像tail -f 一样来输出容器的标准输出<br>
docker stop [容器id|容器名] //停止容器<br>
docker start [容器id|容器名] //启动容器<br>
docker rm [容器id|容器名] //删除容器<br>

### 1.容器使用
docker ps -a//查看所有命令<br>
docker stats //查看虚拟cpu使用io使用等<br>
docker top [容器id|容器名] //查看容器内运行情况<br>
docker pull training/webapp //载入镜像<br>
docker run -d -P training/webapp python app.py<br>
// -d 让容器在后台运行<br>
// -P 将容器内部使用到的网络端口映射到我们使用的主机上 <br>
// docker ps可以查看到端口映射信息<br>
docker run -d -p 5000:5000 training/webapp python app.py<br>
// -p 设置不一样的端口<br>
docker port [容器id|容器名称] // 查看端口映射情况<br>
docker inspect [容器id|容器名] <br>
//查看docker底层信息，返回一个json记录着docker容器配置和状态<br>
docker attach [容器id|容器名称] // 进入一个正在运行的容器<br>
docker inspect -f '{{.ID}}' [容器id|容器名称]<br>
docker ps [local path] [上一命令的结果]:[container path]<br>
//从本地拷贝文件到容器<br>
docker cp src dest<br>

### 2.使用镜像
运行容器时，如果本地不存在，则从仓库中下载，默认从Docker Hub下载<br>
2.1 管理和使用本地Docker主机镜像<br>
docker images //列出本地主机上的镜像 TAG列，代表这个仓库的不同的版本<br>
docker pull [镜像名]:[版本号] //拉取镜像<br>
docker search [镜像名] //搜索镜像<br>
2.2 创建镜像<br>
两种方式：1.从已创建的容器中更新镜像，并且提交这个镜像；2.使用Dockerfile指令创建一个新的镜像<br>
docker commit -m="commit log" -a="runoob" [容器id|容器名] runoob/[镜像名]:[版本号]<br>
//-m 提交描述<br>
//-a 指定镜像作者<br>
docker build -t <br>
