# Wireshark 抓包 + 抓包结果分析
## 一、基础概念
Wireshark：开源网络抓包分析工具，捕获网卡上经过的数据包；底层依赖 `libpcap`（Linux/macOS）、`WinPcap/Npcap`（Windows）。
> 重要提醒：**仅可抓取自己拥有权限的设备/网络流量；未经授权抓取他人网络数据包属于违法行为。**

## 二、抓包操作步骤
### 1. 准备
- Windows：安装时勾选 Npcap；
- Linux：`sudo apt install wireshark`，普通用户抓包需要把用户加入 `wireshark` 用户组，否则只能root运行；
- 选择网卡：打开Wireshark首页，列表会列出本机网卡（Wi‑Fi、以太网、虚拟网卡）。

### 2. 开始抓包
1. 双击目标网卡，进入抓包界面，数据包开始滚动；
2. **过滤（最常用）**：在上方 `Display filter` 输入过滤表达式，回车生效
    | 过滤需求 | 表达式 |
    | ---- | ---- |
    | 指定IP | `ip.addr == 192.168.1.100` |
    | 只看TCP | `tcp` |
    | 只看UDP | `udp` |
    | 指定端口 | `tcp.port == 443` |
    | HTTP | `http` |
    | HTTPS TLS | `tls` |
    | IP源地址 | `ip.src == 10.0.0.5` |
    | IP目的地址 | `ip.dst == 223.5.5.5` |

> 区分：**显示过滤器（display filter）** 只是筛选已经抓到的包；**捕获过滤器（capture filter）** 在抓包前设置，只抓取符合条件的包，减少文件体积，语法不一样（基于bpf）。
> 捕获过滤器示例：`host 192.168.1.100 and port 80`

3. 停止抓包：点击红色停止按钮；
4. 保存：`File -> Save`，保存为 `.pcapng`（推荐，新版格式）。

## 三、Wireshark界面4个核心面板
1. **数据包列表面板（上）**：每行1个包
    - No：序号
    - Time：抓包相对时间（从抓包开始计时）
    - Source：源地址
    - Destination：目的地址
    - Protocol：协议
    - Length：包总字节长度
    - Info：协议简要信息（比如TCP `SYN`、`GET /`）
2. **数据包详情面板（中）**：选中单个包，分层展开协议栈
    ```
    Frame: 物理层，整个数据包信息
    ├─ Ethernet II：二层，源/目的MAC
    ├─ IPv4：三层，源IP、目的IP、TTL
    └─ TCP/UDP：四层，端口、标记位
        └─ HTTP/TLS：应用层
    ```
3. **字节原始面板（下）**：数据包原始十六进制+ASCII内容。

## 四、典型数据包分析案例
### 案例1：TCP三次握手（建立连接）
TCP标记位：`SYN`、`SYN+ACK`、`ACK`
1. 客户端 → 服务端：`SYN`，客户端随机序列号seq=x
    > 我想建立连接，我的初始序列号是x
2. 服务端 → 客户端：`SYN+ACK`，seq=y，ack=x+1
    > 同意连接，你的下一个包序列号应该是x+1；我的初始序列号y
3. 客户端 → 服务端：`ACK`，ack=y+1
    > 收到，连接正式建立

> 抓包关键词：`Flags: SYN` / `Flags: SYN, ACK` / `Flags: ACK`

### 案例2：HTTP请求（明文）
协议栈：Ethernet → IP → TCP → HTTP
- TCP已经建立连接后，客户端发送 `GET /index.html HTTP/1.1`
- 服务端返回 `HTTP/1.1 200 OK` + 响应体
> 可以直接在详情面板看到请求头、响应头。

### 案例3：HTTPS（TLS）
流量是加密的，**默认抓包看不到HTTP明文**。
想要解密HTTPS：需要配置浏览器SSLKEYLOG文件，在Wireshark指定密钥文件，才能还原TLS明文。

### 案例4：TCP四次挥手（断开连接）
1. 客户端 → 服务端：`FIN`：我这边不再发数据
2. 服务端 → 客户端：`ACK`：收到关闭请求
3. 服务端 → 客户端：`FIN`：服务端数据发完，也要关闭
4. 客户端 → 服务端：`ACK`：确认关闭

## 五、常见问题定位（抓包分析常用）
1. **访问不通**
    - 没有抓到包：本地没发包，或者网卡选错；
    - 有ICMP Destination Unreachable：路由不可达；
    - 只有SYN包，没有SYN+ACK：防火墙拦截、服务没监听端口；
    - SYN+ACK收到，但是客户端没ACK：客户端防火墙丢弃。
2. **网络慢**
    - 看 `TCP Retransmission`：TCP重传，网络丢包；
    - `TCP ZeroWindow`：接收方缓冲区满，应用读取慢。
3. **DNS解析**：过滤 `dns`，查看DNS请求与响应，看是否解析超时、返回错误IP。

## 六、常用小技巧
1. 追踪流：选中TCP包 → 右键 `Follow → TCP Stream`，把这个TCP连接所有上下行数据合并展示（HTTP明文非常好用）
2. 专家信息：`Analyze → Expert Information`，自动汇总错误、警告（重传、乱序等）
3. 时间参考：`View → Time Display Format`，修改时间显示格式。

## 七、局限
1. HTTPS/TLS流量**没有密钥无法解密应用层内容**；
2. 抓包只能看到网卡上经过的流量，跨设备交换机默认不会把别人流量发给你（普通网卡无法嗅探同网段其他主机流量，交换环境下arp嗅探属于违规操作）；
3. VPN流量：抓包看到的是VPN封装后的包。

如果你需要，我可以：
- 给一段**练习用的pcap抓包场景**和对应的分析题；
- 或者讲解如何配置SSLKEYLOG解密HTTPS；
- 或者写常用过滤表达式合集。