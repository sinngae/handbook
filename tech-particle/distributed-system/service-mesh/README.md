# Service Mesh

## 综合
1.Pod Node app redis
亲和性和反亲和性

2.Stateful app（relate to stateless）
DNS/proxy/mq/...

3.运维Devops重新定义
监管控 IS/IIS
SRE/devops/AIOps
站点可靠工程师

运维无状态服务，docker不是dev/ops的界限

4.knative/cloudnative/istio/k8s
世界不需要程序员
k8s下一代技术

Knative:  
build->serving(k8s支撑)->eventing

Istio(route支持)

build自带rate信息->自动化扩容服务

openFaas <--> Serverless

Knative更广阔
api兼容 跨云协作 开放协议

* Dapper阿里

5.docker containerd本质
linux namespace

clone()函数 pid命名空间/网络命名空间/mount命名空间（文件系统)

exec()函数装入程序

nc命令 nc -l 80
cgroup -> cpu/memory

6.边缘计算：分化vs癌化

## 虚拟化技术
虚拟化本质是资源管理技术，把计算机实体资源如CPU/内存/网卡/磁盘抽象后呈现出来，打破把实体不可分开使用的障碍，使用户可以用更好的组织型态使用实体资源。
+ 硬件虚拟化 SR-IOV
+ 软件虚拟化
    + 应用虚拟化 Wine
    + 平台虚拟化
        + 完全虚拟化 vmware vbox qemu
        + 硬件负载虚拟化 vmware xen kvm
        + 部分虚拟化
        + 超虚拟化
        + 操作系统虚拟化 即容器技术

docker在同一主机的操作系统上虚拟出不同的运行时环境，支持不同环境的进程运行。比虚拟出整个操作系统的vmware和vbox更轻量级。容器完全符合沙箱机制，相互之间不会有接口。