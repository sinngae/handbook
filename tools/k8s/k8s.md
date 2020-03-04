# k8s
## 构成
### master节点
+ kube-apiserver
+ kube-scheduler
+ kube-controller-manager
+ etcd

键值数据库；参考zk来做分布式一致性和服务发现；
### node节点
+ kubelet

    管理生命Pod周期
+ kube-proxy

    网络齿轮/管道/网线
### Container Runtime
k8s 支持docker containerd cri-o rktlet及任何实现了kubernetes CRI的容器

+ CRI CNI - Container Network Interface
    + CRI - container runtime interface
    + CNI - Container Network Interface
    + CSI - 

    Kubelet 通过CRI与容器运行时交互，以管理镜像和容器。

    CNI(容器网络接口)
+ OCI - Open Container Initiative

    linux基金会进行管理，OCI规范可以看做是Container Runtime的一个标准

## 部署
### 1.为什么要关闭swap
为了性能，当然如果运行容器较多，可以添加kubelet参数--fail-swap-on=false来解决
```
free	// 查看swap是否使用
vim /etc/fstab swap 那行注释掉，永久关闭
swapoff -a // 关闭
swapon -a // 打开
sysctl --system // 从所有的系统目录读取值
```