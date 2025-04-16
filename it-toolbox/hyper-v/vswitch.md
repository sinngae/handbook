# vswitch 虚拟交换机
## centos配置
双交换机：默认交换机 Default Switch + 外部交换机。
1. 安装Hyper-V，并新建centos-7虚拟机；
2. 新增外部交换机，并在宿主机下执行powershell>ipconfig查看新建外部交换机IP配置；
3. 配置centos-7，新增外部交换机对应的网卡；
4. 启动centos-7，`vim /etc/sysconfig/network-scripts/ifcfg-eth1`，配置外部交换机对应的网卡为静态ip。
