## 1.确认内核支持memory and swap accounting
cat /boot/config-3.10.0-957.5.1.el7.x86_64 |grep CONFIG_MEMCG
如何开启？

## 2.运行etcd
docker run --net=host -d gcr.io/google_containers/etcd:2.0.12 /usr/local/bin/etcd --addr=127.0.0.1:4001 --bind-addr=0.0.0.0:4001 --data-dir=/var/etcd/data

## 3.启动master，即运行kubelet并启动一个包含其他master组件的[pod](../userguide/pods.md)
docker run \
--volume=/:/rootfs:ro \
--volume=/sys:/sys:ro \
--volume=/dev:/dev \
--volume=/var/lib/docker/:/var/lib/docker:ro \
--volume=/var/lib/kubelet/:/var/lib/kubelet:rw \
--volume=/var/run:/var/run:rw \
--net=host \
--pid=host \
--privileged=true \
-d \
gcr.io/google_containers/hyperkube:v1.0.1 \
/hyperkube kubelet --containerized --hostname-override="127.0.0.1"
--address="0.0.0.0" --api-servers=http://localhost:8080 --config=/etc/kubernetes/manifests

## 4.运行service proxy
docker run -d --net=host --privileged gcr.io/google_containers/hyperkube:v1.0.1 /hyperkube proxy --master=http://127.0.0.1:8080 --v=2

## 测试
安装kubernetes_client

## centos部署k8s
1.设置时间同步
ntpdate -u cn.pool.ntp.org
2.node结点安装redhat-ca.crt
yum install *rhsm* -y

