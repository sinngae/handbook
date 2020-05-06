# 虚拟化技术
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

## 虚拟机
VMWare Vbox Hyper-V kvm(libvirtd)

**vmware转Hyper-V**
windows最新自带Hyper-V管理工具，之前vmware和vbox基于Hyper-V提供虚拟机功能。
那怎么从vmware/vbox转hyper-v，如果是windows虚拟机可以使用MVMC(windows提供，把运行中的虚拟机转移)和StartWind V2V Convertor（提供磁盘文件的转换），但对于linux虚拟机两者支持都不好，最后只能关闭Hyper-V管理，然后使用vmware。
+ services.msc禁止Hyper服务自动启动；
+ 打开控制面板，windows功能禁用Hyper服务；
+ 管理员启动cmd，执行`bcdedit /set hypervisorlaunchtype off`;

逆操作：
+ services.msc配置Hyper被禁止的服务自动启动(其他保留手动触发即可)；
+ 打开控制面板，windows功能禁用Hyper服务；
+ 管理员启动cmd，执行`bcdedit /set hypervisorlaunchtype auto`;

**bcdedit**

## 容器
容器的核心技术：Cgroup + Namespace

容器 = cgroup + namespace + rootfs + 容器引擎
+ cgroup：资源控制
+ namespace：访问隔离
+ rootfs：文件系统隔离，镜像的本质就是一个rootfs文件
+ 容器引擎：生命周期控制

### docker
docker在同一主机的操作系统上虚拟出不同的运行时环境，支持不同环境的进程运行。比虚拟出整个操作系统的vmware和vbox更轻量级。容器完全符合沙箱机制，相互之间不会有接口。