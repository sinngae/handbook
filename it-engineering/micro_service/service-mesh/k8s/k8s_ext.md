来源：https://www.cnblogs.com/lfl17718347843/p/14480495.html
----
k8s集群从一千节点增加到五千台节点遇到的瓶颈
Kubernetes 自从1.6起便号称可以承载 5000 个以上的节点，但是从数十到 5000 的路上，难免会遇到问题。
在 kubernetes 5000 之路上的经验，包括遇到的问题、尝试解决问题以及找到真正的问题。
1、
问题一：1 ~ 500 个节点之后
问题：kubectl 有时会出现 timeout （ p.s. kubectl -v=6 可以显示所有 API 细节指令）

尝试解决：
一开始以为是 kube-apiserver 服务器负载的问题，尝试增加 proxy 做 replica 协助进行负载均衡
但是超过 10 个备份 master 的时候，发现问题不是因为 kube-apiserver 无法承受负载，GKE 通过一台 32-core VM 就可以承载 500 个节点

原因：
排除以上原因，开始排查 master 上剩下的几个服务(etcd、kube-proxy)
开始尝试调整 etcd
通过使用datadog查看 etcd 吞吐量，发现有异常延迟(latency spiking ~100 ms)
通过Fio工具做性能评估，发现只用到 10%的 IOPS(Input/Output Per Second)，由于写入延迟(write latency 2ms)降低了性能
尝试把 SSD 从网络硬盘变为每台机器有个 local temp drive （ SSD ）
结果从~100ms —> 200us

2、
问题二：~1000 个节点的时候
问题：发现 kube-apiserver 每秒从 etcd 上读取 500mb

尝试解决：
通过Prometheus查看 container 之间的网络流量

原因：
发现Fluentd和 Datadog 抓取每个节点上资料过于频繁
调低两个服务的抓取频率，网络性能从 500mb/s 降低到几乎没有
etcd 小技巧：通过--etcd-servers-overrides可以将 Kubernetes Event 的资料写入作为切割，分不同机器处理，如下所示
--etcd-servers-overrides=/events#https://0.example.com:2381;https://1.example.com:2381;https://2.example.com:2381

3、
问题三：1000 ～ 2000 个节点
问题：无法再写入数据，报错 cascading failure
kubernetes-ec2-autoscaler 在全部的 etcd 都停掉以后才回传问题，并且关闭所有的 etcd

尝试解决：
猜测是 etcd 硬盘满了，但是检查 SSD 依旧有很多空间
检查是否有预设的空间限制，发现有 2GB 大小限制

解決方法:
在 etcd 启动参数中加入--quota-backend-bytes
修改 kubernetes-ec2-autoscaler 逻辑——如果超过 50%出现问题，关闭集群
各种服务的优化
Kube masters 的高可用
一般来说，我们的架构是一个 kube-master （主要的 Kubernetes 服务提供组件，上面有 kube-apiserver、kube-scheduler 和 kube-control-manager ）加上多個 slave。但是要达到高可用，要参考一下方式实现：

kube-apiserver 要设置多个服务，并且通过参数--apiserver-count重启并且设定
kubernetes-ec2-autoscaler 可以帮助我们自动关闭 idle 的资源，但是这跟 Kubernetes scheduler 的原则相悖，不过通过这些设定，可以帮助我们尽量集中资源。
{
"kind" : "Policy",
"apiVersion" : "v1",
"predicates" : [
{"name" : "GeneralPredicates"},
{"name" : "MatchInterPodAffinity"},
{"name" : "NoDiskConflict"},
{"name" : "NoVolumeZoneConflict"},
{"name" : "PodToleratesNodeTaints"}
],
"priorities" : [
{"name" : "MostRequestedPriority", "weight" : 1},
{"name" : "InterPodAffinityPriority", "weight" : 2}
]
}
以上为调整 kubernetes scheduler 范例，通过调高 InterPodAffinityPriority 的权重，达到我们的目的。更多示范参考范例．

需要注意的是，目前 Kubernetes Scheduler Policy 并不支持动态切换，需要重启 kube-apiserver(issue: 41600)

4、
调整 scheduler policy 造成的影响
OpenAI 使用了KubeDNS ，但不久后发现

问题：
经常出现 DNS 查询不到的情况（随机发生）
超过 ~200QPS domain lookup

尝试解决：
尝试查看为何有这种状态，发现有些 node 上跑了超过 10 个 KuberDNS

解决方法：
由于 scheduler policy 造成了许多 POD 的集中
KubeDNS很轻量，容易被分配到同一节点上，造成 domain lookup 的集中
需要修改 POD affinity （相关介绍），尽量让KubeDNS分配到不同的 node 之上
affinity:
podAntiAffinity:
requiredDuringSchedulingIgnoredDuringExecution:

weight: 100
labelSelector:
matchExpressions:
key: k8s-app
operator: In
values:
kube-dns
topologyKey: kubernetes.io/hostname
5、
新建节点时 Docker image pulls 缓慢的问题

问题：
每次新节点建立起来，docker image pull 都要花 30 分钟

尝试解决：
有一个很大的 container image Dota，差不多 17GB，影响了整个节点的 image pulling
开始检查 kubelet 是否有其他 image pull 选项

解决方法：
在 kubelet 增加选项--serialize-image-pulls=false来启动 image pulling，让其他服务可以更早地 pull （参考：kubelet启动选项）
这个选项需要 docker storgae 切换到 overlay2 （可以参考docker 教学文章）
并且把 docker image 存放到 SSD，可以让 image pull 更快一些
补充：source trace
// serializeImagePulls when enabled, tells the Kubelet to pull images one
// at a time. We recommend not changing the default value on nodes that
// run docker daemon with version < 1.9 or an Aufs storage backend.
// Issue #10959 has more details.
SerializeImagePulls *bool json:"serializeImagePulls"
提高 docker image pull 的速度
此外，还可以通过以下方式来提高 pull 的速度

kubelet 参数--image-pull-progress-deadline要提高到 30mins docker daemon 参数max-concurrent-download调整到 10 才能多线程下载

6、
网络性能提升
Flannel 性能限制

问题：
OpenAI 节点间的网络流量，可以达到 10-15GBit/s，但是由于 Flannel 所以导致流量会降到 ～ 2GBit/s

解决方式是拿掉 Flannel，使用实际的网络
hostNetwork: true
dnsPolicy: ClusterFirstWithHostNet