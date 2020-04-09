# Service Mesh

## openstack
美国NASA牵头，各IT巨头参与的OpenStack，私有云框架。
现在美国NASA已停止Openstack开发，并迁移到aws上。
美国国内云计算厂商前三是亚马逊AWS、微软Azure、谷歌GCP无一不是走自主研发路线，OpenStack已经势微。OpenStack是拼凑出来的，现在也面临烂尾。
有意思的是主流云计算都是互联网公司研发，而OpenStack都是传统IT厂家如IBM/Intel/华为/思科/惠普。Openstack对传统IT也许就是换个方式卖硬件，而互联网的迭代升级和快速响应，且本身就是做大规模业务服务的，更多地是把自身积累的理念和经验外部化。

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

