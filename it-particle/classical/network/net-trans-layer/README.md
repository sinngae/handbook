# Socket

## Socket
socket中文翻译为插口、插座。

Socket 技术用语所谓套接字，是对网络中主机上应用进程之间进行的双向通信的端点的抽象
```
CPU的插座 Socket 370\Socket 462\Socket AM3，是实体插座。

ARPANET:
a "connection" is a pair: a "send socket" at one end and a "receive socket" at the other.
... ...
Further, the ARPANET is just beginning to evolve a view of "process-to-process" protocols where cooperating programs on dissimilar systems communicate over network sockets in a true use of sockets as interprocess communication media.

Unix BSD TCP/IP:
Socket layer
The socket layer deals with the interprocess communications facilities provided by the system. A socket is a bidirectional endpoint of communication which is "typed" by the semantics of communication it supports.

套接管，socket pipe，用于套接的水管
```
socket是一种进程间通信机制，提供一种供应用程序访问通信协议的操作系统调用，并且通过将socket与Unix系统文件描述符相整合，使得网络读写数据 和 读写文件一样操作。socket不再是硬件上的物件，而是一组操作“指令”。按汉语理解，具备“套接”（建立进程间网络通讯）和“字”（可读写交互的）。
也曾被翻译成 媒介字、信口、软插口。

## 类型
### 1. 流套接字 sock_stream
面向连接，可靠的数据传输服务。
使用的是传输控制协议，TCP the Transmission Control Protocol。
序列号、确认应答、超时重传、流量控制、拥塞控制。
### 2. 数据报套接字 sock_dgram
无连接、不可靠的服务。
不能保证数据传输的可靠性，数据可能丢失、重复、乱序。
UDP, User Datagram Protocol

### 3. 原始套接字 sock_raw
可以读写内核没有处理的IP数据包（流套接字 TCP协议报文，数据报套接字 UDP协议报文）
需要访问其他协议发送的数据。


# listen() （及 bind()）

## TCB（传输控制块）
## 常见错误码
listen() 是 TCP 服务器编程中的关键系统调用，用于将 socket 置于被动监听状态。以下是各平台（以 Linux/POSIX 为主）中 listen() 可能返回的错误码及其典型触发场景：
### 核心错误码
错误码	含义	典型触发场景
+ EBADF	无效的文件描述符	sockfd 不是已打开的文件描述符，或已关闭
+ ENOTSOCK	非 socket 描述符	sockfd 指向的是普通文件、管道等，而非 socket
+ EOPNOTSUPP	操作不支持	socket 类型不是 SOCK_STREAM / + SOCK_SEQPACKET（如 UDP 套接字调用了 listen）
+ EINVAL	参数无效	
    1. backlog 为负数
    2. socket 已处于已连接状态（已 accept 或 connect 成功）
    3. socket 未执行 bind() 就直接调用 listen()（Linux 下）
+ EADDRINUSE	地址已被占用	另一个 socket 已在同一地址+端口上处于监听状态

### 资源与权限相关
错误码	含义	典型触发场景
+ ENOMEM / ENOBUFS	内存/缓冲区不足	内核无法为连接队列分配足够内存（高负载或系统资源耗尽时）
+ EACCES	权限不足	某些防火墙规则（如 iptables/nftables）或 SELinux 策略禁止在该端口监听（较少见，通常在 bind() 阶段触发）

### 平台差异说明
Linux：若未 bind() 直接调用 listen()，会返回 EINVAL（某些 BSD 系统可能自动绑定到随机端口）。
Windows (Winsock)：错误码以 WSA 前缀表示，如 WSAENOTSOCK、WSAEOPNOTSUPP，语义与 POSIX 对应。
macOS/BSD：额外可能出现 ECONNABORTED 等，但极少在 listen() 本身返回。

### 排查建议
总是先 bind() 再 listen() — 未绑定是 EINVAL 的常见原因。
检查 backlog 值 — 通常设为 SOMAXCONN 或合理值（如 128/512），避免负数。
确认 socket 类型 — 只有面向连接的流式 socket（TCP / Unix Domain Stream）才能 listen()。
端口冲突 — 若报 EADDRINUSE，用 netstat / ss 检查是否已有进程占用该端口。

# accept()

## 常见错误码
accept() 从监听 socket 的已完成连接队列中取出一个连接，返回新的已连接 socket 描述符。以下是各平台（Linux/POSIX 为主）中 accept() 可能返回的错误码：
### 描述符与状态相关
错误码	含义	典型触发场景
+ EBADF	无效的文件描述符	sockfd 未打开或已关闭
+ ENOTSOCK	非 socket 描述符	sockfd 指向普通文件、管道等
+ EOPNOTSUPP / EINVAL	操作/参数无效	socket 未调用 listen() 就执行 accept()；或 addrlen 为无效值
+ EFAULT	地址空间错误	addr 或 addrlen 指针指向无效的进程地址空间

### 非阻塞与中断相关
错误码	含义	典型触发场景
+ EAGAIN / EWOULDBLOCK	资源暂时不可用	socket 设为 非阻塞（O_NONBLOCK），且当前没有已完成连接等待接受
+ EINTR	被信号中断	阻塞等待连接期间捕获到信号（如 SIGALRM、SIGINT），需重试

### 资源耗尽相关
错误码	含义	典型触发场景
EMFILE	进程文件描述符表满	当前进程已打开的文件数达到 RLIMIT_NOFILE 软限制
ENFILE	系统文件描述符表满	整个系统打开的文件总数达到内核上限
ENOMEM / ENOBUFS	内存/缓冲区不足	内核无法为新连接分配 socket 结构或缓冲区

### 连接异常相关
错误码	含义	典型触发场景
+ ECONNABORTED	连接已中止	三次握手完成后、但 accept() 取出之前，连接被对端 RST 或本端异常终止
+ EPROTO	协议错误	底层协议栈出现异常情况（如某些防火墙/SELinux 拦截握手完成后的连接）
+ EPERM	权限不足	防火墙规则（如 iptables conntrack）阻止接受该连接（较少见）

### 平台差异说明
Linux：EAGAIN 与 EWOULDBLOCK 值相同；若 sockfd 未 listen()，返回 EINVAL。
BSD/macOS：未 listen() 时通常返回 EOPNOTSUPP；ECONNABORTED 比 Linux 更常见。
Windows (Winsock)：对应 WSAEBADF、WSAEWOULDBLOCK、WSAECONNABORTED 等，语义一致。

### 编程建议
处理 EINTR — 阻塞模式下被信号中断是正常情况，通常应循环重试 accept()。
处理 EAGAIN/EWOULDBLOCK — 非阻塞模式下需配合 select()/poll()/epoll() 使用，或在事件驱动循环中忽略此错误继续等待。
处理 ECONNABORTED — 直接关闭返回的 fd（或忽略），继续下一轮 accept()，不要终止服务。
资源限制 — 生产环境应监控 EMFILE/ENFILE，必要时调高 ulimit -n 或检查 fd 泄漏。

# connect()

## 三次握手
详见示意图。

## 常见错误码
connect() 用于主动发起 TCP 连接或关联 UDP 目标地址。以下是 Linux/POSIX 环境下 connect() 最常见的错误码：
### 连接结果类（最常用）
错误码	含义	典型触发场景
+ ECONNREFUSED	连接被拒绝	目标 IP 可达，但目标端口没有进程监听；或防火墙显式拒绝（RST）
+ ETIMEDOUT	连接超时	SYN 包发出后，在超时时间内未收到任何响应（对端宕机、网络丢包、防火墙静默丢弃）
+ ENETUNREACH	网络不可达	没有到目标网络的路由（如网关未配置、路由表缺失）
+ EHOSTUNREACH	主机不可达	路由可达网络，但无法到达具体主机（如 ARP 解析失败、主机离线）
+ ECONNRESET	连接被重置	三次握手过程中收到 RST（较少见，通常发生在数据传输阶段）
### 非阻塞模式专属
错误码	含义	典型触发场景
+ `EINPROGRESS`	操作正在进行	非阻塞 socket 上首次调用 connect()，连接已启动但尚未完成，需后续用 select/poll/epoll 检测可写
+ EALREADY	操作已在进行	非阻塞 socket 上 connect() 返回 + EINPROGRESS 后，在连接完成前再次调用 connect()
+ EISCONN	socket 已连接	对已连接的 socket 再次调用 connect()（TCP 已建立连接后重复调用）
### 参数与描述符类
错误码	含义	典型触发场景
+ EBADF	无效的文件描述符	sockfd 未打开或已关闭
+ ENOTSOCK	非 socket 描述符	sockfd 是普通文件、管道等
+ EINVAL	参数无效	addrlen 错误；或 sockaddr 结构内容不合法
+ EFAULT	地址空间错误	addr 指针指向无效内存
+ EAFNOSUPPORT	地址族不支持	socket 创建时指定的地址族与 addr 中的地址族不匹配
### 地址与端口类
错误码	含义	典型触发场景
+ EADDRINUSE	地址已被使用	本地绑定的源端口已被占用（常见于固定源端口复用或快速重连）
+ EADDRNOTAVAIL	地址不可用	绑定的本地 IP 不是本机网卡地址；或临时端口耗尽
### 资源与权限类
错误码	含义	典型触发场景
+ EINTR	被信号中断	阻塞等待连接期间捕获信号，通常应重试
+ EACCES / EPERM	权限不足	防火墙（iptables/nftables/SELinux）阻止出站连接；或尝试连接广播/组播地址
+ ENOBUFS / ENOMEM	内存/缓冲区不足	内核资源耗尽，无法为新连接分配必要结构
### 平台差异速查
Linux：EAGAIN 在 connect() 中极少返回；非阻塞成功启动统一返回 EINPROGRESS。
BSD/macOS：语义与 Linux 基本一致，EADDRINUSE 在快速重连（TIME_WAIT 状态）时更常见。
Windows (Winsock)：对应 WSAECONNREFUSED、WSAEWOULDBLOCK（等价 EINPROGRESS）、WSAEISCONN 等。
### 编程要点
阻塞模式：ECONNREFUSED、ETIMEDOUT、ENETUNREACH、EHOSTUNREACH 是最需要处理的四种失败场景。
非阻塞模式：首次 connect() 返回 -1 且 errno == EINPROGRESS 是正常流程，不是错误；后续通过 getsockopt(..., SO_ERROR, ...) 获取真实连接结果。
快速重连：主动关闭后立刻重连同一目标，可能遇到 EADDRINUSE，可设置 SO_REUSEADDR。

# write() 和 read()

## 常见错误码
write() 和 read() 在 socket 上的行为与文件 I/O 有显著差异，尤其是连接状态变化会直接影响返回值。以下是 Linux/POSIX 环境下最常见的错误码：
### 一、read() 常见错误码
错误码	含义	典型触发场景
+ EAGAIN / EWOULDBLOCK	资源暂时不可用	非阻塞 socket 上当前无数据可读；或数据正在传输中尚未到达
+ EINTR	被信号中断	阻塞等待数据期间捕获到信号，通常应重试
+ ECONNRESET	连接被重置	对端异常崩溃或强制关闭 socket，本端读数据时收到 RST
+ ETIMEDOUT	连接超时	长时间无数据传输，TCP keepalive 超时或应用层读超时触发
+ ENOTCONN	未连接	TCP socket 尚未 connect() 成功（或 accept() 前）就读取
+ EINVAL	参数无效	buf 为 NULL、count 为负数等
+ EBADF / ENOTSOCK	描述符无效	socket 已关闭或不是 socket

`read() 特别说明：返回 0`  
read() 返回 0 不是错误，表示对端已正常关闭连接（发送了 FIN）。这是 TCP 连接终止的标准信号，应`关闭本端 socket`。

### 二、write() 常见错误码
错误码	含义	典型触发场景
+ EAGAIN / EWOULDBLOCK	资源暂时不可用	非阻塞 socket 上内核发送缓冲区已满
+ EINTR	被信号中断	阻塞发送期间捕获信号，通常应重试（已发送部分数据需处理）
+ `EPIPE`	管道断裂	对端已关闭连接（收到 FIN 或 RST），本端继续写入；默认会触发 SIGPIPE 信号终止进程
+ `ECONNRESET`	连接被重置	写数据时收到对端发来的 RST（对端异常崩溃或端口被复用）
+ EMSGSIZE	消息过长	UDP 模式下，单次写入超过 MTU 或内核限制（如 > 65507 字节）
+ ENOBUFS / ENOMEM	缓冲区/内存不足	内核发送队列或网络设备队列满（高负载或突发流量）
+ EDESTADDRREQ	需要目标地址	UDP 未 connect() 且未通过 sendto() 指定地址
+ ENOTCONN	未连接	TCP socket 未建立连接就写入
+ EINVAL	参数无效	buf 为 NULL、count 过大或 socket 标志冲突
+ EBADF / ENOTSOCK	描述符无效	socket 已关闭或不是 socket

### 三、关键区别：EPIPE vs ECONNRESET
场景	read() 表现	write() 表现
+ 对端正常关闭（FIN）	返回 0	首次返回 EPIPE（+ SIGPIPE）
+ 对端异常重置（RST）	返回 -1，errno = ECONNRESET	返回 -1，errno = ECONNRESET

SIGPIPE 防护：`生产环境通常应忽略或捕获 SIGPIPE（signal(SIGPIPE, SIG_IGN)），避免进程被意外终止。`

### 四、UDP 特殊注意
错误码	UDP 专属场景
+ EMSGSIZE	报文超过接口 MTU 且未设置 IP_PMTUDISC_DONT / 不允许分片
+ EDESTADDRREQ	未关联地址的 UDP socket 直接调用 write()（应使用 sendto()）
+ ECONNREFUSED	UDP connect() 后，向未监听端口发送数据，收到 ICMP Port Unreachable

### 五、编程建议
处理 EINTR — 阻塞 I/O 必须循环重试；注意 write() 被中断时已发送部分数据，需用 writev 或记录偏移量。
区分 read() == 0 与 < 0 — 返回 0 是正常 EOF，应优雅关闭连接。
非阻塞模式 — EAGAIN/EWOULDBLOCK 是正常流控信号，配合 epoll/kqueue 的 EPOLLOUT/EPOLLIN 事件处理。
防御 SIGPIPE — 服务器程序务必设置 SIG_IGN，或使用 MSG_NOSIGNAL 标志（Linux）/SO_NOSIGPIPE 选项（BSD/macOS）。

# close() 

## 四次挥手
详见示意图。

## 常见错误码
close() 在 socket 上的错误码相对简单，但行为陷阱比错误码本身更值得注意。
### 一、close() 直接返回的错误码
错误码	含义	典型触发场景
+ EBADF	无效的文件描述符	fd 不是已打开的文件描述符，或已关闭过
+ EINTR	被信号中断	POSIX 允许返回，但 Linux 内核实现会保证 fd 被关闭；返回 -1 时仍不可再次 close()，否则触发 EBADF
+ EIO	I/O 错误	底层刷新缓冲区时发生硬件/驱动错误（socket 上极少见，多见于磁盘文件）

核心事实：close() 成功时返回 0，失败时返回 -1。但即使返回 -1，在 Linux 上 fd 通常已经被释放，重复关闭会导致 EBADF。

### 二、比错误码更重要的：close() 的行为陷阱
1. 数据丢失风险  
    close() 不会等待未发送的数据全部到达对端。如果发送缓冲区还有数据，内核会尝试发送，但：
    + 若进程立即退出，内核可能来不及完成四次挥手
    + 对端可能永远收不到最后一批数据
    + 对策：先调用 shutdown(fd, SHUT_WR) 等待对端 ACK，再 close()。
2. SO_LINGER 的影响
```c
struct linger so_linger = { .l_onoff = 1, .l_linger = 5 };
setsockopt(fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));
```
+ l_onoff = 0（默认）：close() 立即返回，后台完成四次挥手
+ l_onoff = 1, l_linger = 0：强制发送 RST 立即终止连接，不进入 TIME_WAIT
+ l_onoff = 1, l_linger > 0：close() 阻塞 最多 l_linger 秒，等待数据发送+收到 ACK；超时则发送 RST 并返回 EAGAIN/ETIMEDOUT

3. 未读数据导致 RST  
若本端 socket 接收缓冲区还有未读数据就调用 close()，内核通常会发送 RST 而非 FIN，导致对端 read() 收到 ECONNRESET，而非正常的 EOF（read() == 0）。

### 三、shutdown() 的错误码（常与 close 配合使用）
错误码	含义	典型触发场景
+ EBADF	无效描述符	fd 未打开或已关闭
+ ENOTSOCK	非 socket	fd 指向普通文件、管道等
+ ENOTCONN	未连接	TCP socket 尚未建立连接
+ EINVAL	参数无效	how 不是 SHUT_RD/SHUT_WR/SHUT_RDWR

### 四、编程建议
+ 不要忽略 close() 返回值，但也不要在出错时重试关闭同一 fd（已关闭）
+ 优雅关闭流程：
```c
shutdown(fd, SHUT_WR);   // 通知对端不再发送
// 可选：等待对端也 shutdown 或 read() 返回 0
close(fd);
```
+ 避免 TIME_WAIT 堆积：服务端主动关闭大量短连接时，考虑 SO_REUSEADDR，而非 SO_LINGER = 0（RST 可能丢包导致对端重传无人响应）
+ 多线程环境：确保一个 fd 只被一个线程 close()，否则极易出现 EBADF 或竞态关闭错误 fd

# UDS, Unix Domain Socket
本地的进程间通信。

# 基于UDP协议实现可靠传输 QUIC

详见HTTP3


## 报文大小限制总结

HTTP报文不限制大小，TCP协议有最大报文段长度，HTTP大文件会被拆分成多个传输。
HTTP get url没有明确限制，但一般不超过2kB；post 可以处理数十MB 或更大的请求体。

nginx默认请求体大小 1MB，如果HTTP POST请求体的数据超过1MB会被nginx拒绝，413 Request entity too large错误。（client_max_body_size，重启生效）
tomcat 请求体大小默认2MB
