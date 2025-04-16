# docker-machine
docker-machine是支持在主机/虚拟机上安装docker的工具，用于管理所有docker，类似于kubernutes。
原理：远程连接一组机器（可以是主机，也可以是虚拟机），批量操作。

```shell script
brew search docker-machine
# docker-machine                          docker-machine-driver-hyperkit          docker-machine-driver-vultr             docker-machine-nfs
# docker-machine-completion               docker-machine-driver-vmware            docker-machine-driver-xhyve             docker-machine-parallels

# 查看已有的docker machine
docker-machine ls
# create docker-machine
docker-machine create --driver vmware test
# start/stop
docker-machine [start|stop] test
```