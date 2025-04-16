# openvpn
vpn，virtual private network，虚拟专用网络，建立安全数据传输隧道。
openvpn是linux上开源的vpn软件，提供了服务端和客户端。

## server端配置
```sh
## 1.安装openvpn
yum install -y epel-release
yum install -y install openvpn easy-rsa net-tools bridge-utils

## 2.创建CA和证书
cd /usr/share/easy-rsa/3
./easyrsa init-pki``
./easyrsa build-ca
./easyrsa build-server-full server1 nopass
./easyrsa build-client-full client1 nopass
./easyrsa gen-dh ## 生成DH需要等待一段时间

#./easyrsa gen-dh
#Generating DH parameters, 2048 bit long safe prime, generator 2
#This is going to take a long time
#..................................................................................+..........
#..................................................................................+........++*++*
#DH parameters of size 2048 created at /usr/share/easy-rsa/3/pki/dh.pem

## 3.创建TLS-Auth Key
openvpn --genkey --secret ./pki/ta.key
cp -pR /usr/share/easy-rsa/3/pki/{issued,private,ca.crt,dh.pem,ta.key} /etc/openvpn/server/
## 4.内核参数中开启ipv4 forwarding
cd /etc/sysctl.d/
vi 99-sysctl.conf
#追加net.ipv4.ip_forward = 1
sysctl --system

## 5.配置openvpn Server
cp /usr/share/doc/openvpn-2.4.8/sample/sample-config-files/server.conf /etc/openvpn/server/
vi /etc/openvpn/server/server.conf
#需要修改的地方如下
#   # line 32: change if need (listening port of openvpn)
#   port 1194
#   # line 35: change if need
#   ;proto tcp
#   proto udp
#   # line 78: specify certificates
#   ca ca.crt
#   cert issued/server1.crt
#   key private/server1.key
#   # line 85: specify DH file
#   dh dh.pem
#   # line 101: specify network to be used on V**
#   # any network are OK except your local network
#   server 10.8.0.0 255.255.255.0
#   # line 143: uncomment and change to your local network
#   push "route 192.168.60.0 255.255.255.0"
#   # line 231: keepalive settings
#   keepalive 10 120
#   # line 244: specify TLS-Auth key
#   tls-auth ta.key
#   # line 263: uncomment (enable compress)
#   comp-lzo
#   # line 281: enable persist options
#   persist-key
#   persist-tun
#   # line 287: change log path
#   status /var/log/openvpn-status.log
#   # line 296: change log path
#   log /var/log/openvpn.log
#   log-append /var/log/openvpn.log
#   # line 306: specify log level (0 - 9, 9 means debug lebel)
#   verb 3

# 6.启动openvpn-server服务并设置开机自启动
systemctl start openvpn-server@server
systemctl enable openvpn-server@server
```
## Client端安装
1. 下载安装  
manjaro pacman可安装openvpn

2. 拷贝证书
+ /etc/openvpn/server/ca.crt
+ /etc/openvpn/server/ta.key
+ /etc/openvpn/server/issued/client1.crt
+ /etc/openvpn/server/private/client1.key

上述四个文件拷贝到manjaro的/etc/openvpn/client/下

3. 编辑client1.config
```sh
cp /usr/share/doc/openvpn-2.4.8/sample/sample-config-files/client.conf /etc/openvpn/client/client1.config

#   #添加及修改如下字段
#   remote [server公网ip] 1194
#   ca ca.crt
#   cert client1.crt
#   key client1.key
#   tls-auth ta.key 1
#   comp-lzo
```
4. 打开openvpn GUI，选择client1，连接vpn
