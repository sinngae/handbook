# minikue探坑记录
minikube是k8s的单点运行模型，用于开发用户本地使用

## win10环境
host：win10

vm driver：hyper-v
win10需要开启嵌入虚拟功能，即在虚拟机上虚拟操作系统

guest：centos7

centos7 vm driver：kvm2

目标：minikube安装在centos7上

+ 1.BISO设置虚拟化
+ 2.Hyper-V设置guest可以嵌套虚拟化：（坑1）
powershell运行：
Set-VMProcessor -VMName <VMName> -ExposeVirtualizationExtensions $true
+ 3.centos安装虚拟化驱动

试过在win10上用minikue，结果卡在
```sh
.\minikube.exe start --image-mirror-country cn --iso-url=https://kubernetes.oss-cn-hangzhou.aliyuncs.com/minikube/iso/minikube-v1.5.0.iso --registry-mirror=https://registry.docker-cn.com --vm-driver="hyperv" --hyperv-virtual-switch="MinikubeSwitch" --memory=2048 --cpus=2 --force
# 报错：creating host ...?
```
没能成功

### 安装
详见：

[阿里云教程](https://yq.aliyun.com/articles/221687)

[英文教程](https://www.howtoforge.com/learning-kubernetes-locally-via-minikube-on-linux-manjaro-archlinux/)

[github上教程](https://github.com/jaywcjlove/handbook/blob/master/CentOS/CentOS7%E5%AE%89%E8%A3%85KVM%E8%99%9A%E6%8B%9F%E6%9C%BA%E8%AF%A6%E8%A7%A3.md)


### 安全策略
```sh
groupadd libvirtd
sudo usermod -a -G libvirtd [user]

vi /etc/libvirt/libvirtd.conf
# 使能unix_sock_group改为libvirtd

vi /etc/polkit-1/localauthority/50-org.libvirtd-group-access.pkla
#   [libvirtd group Management Access]
#   Identity=unix-group:libvirtd
#   Action=org.libvirt.unix.manage
#   ResultAny=yes
#   ResultInactive=yes
#   ResultActive=yes

service libvirtd restart
```

### 运行
```sh
minikube start --vm-driver=kvm2

minikube dashboard
## 创建dashboard
#   访问http://127.0.0.1:46658/api/v1/namespaces/kubernetes-dashboard/services/http:kubernetes-dashboard:/proxy/

minikube start
minikube stop
```

## manjaro环境
```sh
# pacman安装docker/minikube/kubectl/libvirtd
# 因为平时不用启动他们，所以就没有设置开机启动
# 需要的时候，先启动docker/libvirtd服务
systemctl start docker.service
systemctl start libvirtd.service

# 不要minikube start --vm-driver=kvm2，会占用整个电脑的cpu和内存
minikube start --vm-driver=kvm2 --memory=2048 --cpus=2
# 没错，就这样就可以了，manjaro一直惯着它的用户
```

## 使用
### 验证
```sh
# 查看集群状态
kubectl cluster-info
# 查看节点信息
kubectl get nodes
# 查看deploy状态
kubectl get pods
```
### 发布
```sh
# 旧发布方式，已不推荐
kubectl run httpd --image=centos/httpd --port=80
kubectl expose deployment httpd --port 80 --type=NodePort
# 因为centos没有安装UI桌面，本测试使用nginx配置转发访问httpd，在host用浏览器访问的。

# 访问httpd服务
minikube service httpd
# 获取访问url
minikube service httpd --url
```