# docker-swarm
参考：http://c.biancheng.net/view/3168.html

Docker Swarm 是 Docker 的集群管理工具.


## 搭建
```shell
# 创建swarm管理节点
docker-machine create -d virtualbox swarm-manager
#   初始化
docker-machine ssh swarm-manager # 进入管理节点
docker swarm init --advertise-addr 192.168.99.107
#   创建工作节点（...）
#   增加工作节点
docker swarm join --token SWMTKN-1-4oogo9qziq768dma0uh3j0z0m5twlm10iynvz7ixza96k6jh9p-ajkb6w7qd06y1e33yrgko64sk 192.168.99.107:2377
```

## 管理