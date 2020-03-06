# stun
simple traversal of user datagram protocol through NetworkAddressTranslators 
##NAT的UDP简单穿越
是一种网络协议，允许于多个NAT后的客户端找出自己的公网地址，查出自己位于哪种类型的NAT之后，以及NAT为某一个本地端口所绑定的Internet端端口。（这些信息用于另个同时处于NAT路由器之后的主机之间建立UPD通信）该协议由RFC3489定义（目前已被RFC5389取代，stun被定义为协助穿越NAT的工具，并不独立提供穿越解决方案，后续还有RFC7350）

# turn
traversal using relay NAT 通过Relay方式穿越NAT


# 基于UDP的P2P应用需要考虑NAT类型，不同NAT组合的穿透方式不一致
NAT分为四种
+ 全锥型 Full Cone
+ 受限锥型 Restricted Cone 

  ip受限
+ 端口受限锥形 Port Restricted Cone 

  ip+port受限
+ 对称型 Symmetric

NAT路由器为NAT内同一主机C的同一ip+port访问不同的公网ip+port组合S1和S2，分配不同的端口即为对称型，否则为锥型。
如果S在和S1建立连接后，收到S2发来的数据，则有四种情况：
+ 1.无条件转发给C。为上文1
+ 2.如果C之前给S2的ip发过数据，则转发。为上文2
+ 3.如果C之前给S2的ip+port发过数据，则转发。为上文3
+ 4.丢弃报文，拒绝转发，即对称型NAT

所以：
+ 1.安全性系数：对称型>端口受限锥型>受限锥型>全锥型
+ 2.两个终端共有4+3+2+1=10中组合
+ 3.其中，NAT是端口受限锥型、对称型 与NAT是对称型的之间无法打通

NAT缓解了IPV4地址不足的问题，也带来了限制
+ 1.NAT外部主机无法主动跟位于NAT内部的主机通信
+ 2.NAT内部主机想要通信，必须和公网的一个IP通信
+ 3.必须经路由器建立映射关系，从而实现数据转发

