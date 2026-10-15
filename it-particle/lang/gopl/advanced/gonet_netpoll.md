# 先澄清名词
你说的 **GoNet = Go标准库 `net`（net包，俗称Go-Net）**；
**netpoll** 分两个，容易混淆：
1. **runtime netpoll：Go运行时内置网络轮询器**（标准库net底层依赖）
2. **cloudwego/netpoll：字节开源独立高性能网络库（Hertz、Kitex底层）**

> 行业里聊Go高性能网络对比，一般是：**Go标准库 net（GoNet） VS cloudwego/netpoll（第三方库）**

## 一、关系：两种完全独立的网络实现
### 1）GoNet（标准库 `net`）
Go官方自带，`net.Listen` / `net.Dial` / `net.Conn`。
底层依赖 **runtime.netpoll（运行时netpoll）**
- 模型：**One Conn One Goroutine**，一个连接一个Goroutine
- 对外API：阻塞式Read/Write，用户写代码很简单，同步写法
- 原理：socket设非阻塞；读不到数据时，Goroutine park挂到runtime.netpoll；IO就绪唤醒这个G继续执行。
> 一句话：**GoNet（net包）是上层网络接口；runtime.netpoll是GoNet底层的IO多路复用引擎**

### 2）cloudwego/netpoll（第三方库，字节）
独立开源库，**不依赖标准库net，自带一套Reactor事件驱动模型**，不是Go runtime里那个netpoll。
- 模型：**Multi-Reactor，少量Reactor协程管理海量连接**，不是一连接一Goroutine
- 对外API：事件回调模型（OnRead、OnClose）
- 底层：Linux epoll LT；macOS kqueue；**原生不支持Windows**
- 典型使用者：Hertz HTTP框架、Kitex RPC框架，可以切换底层网络库：`net` / `netpoll`

# 二、GoNet（标准net） VS cloudwego/netpoll 核心区别
|对比项|GoNet（标准库net）|cloudwego/netpoll（字节netpoll）|
|---|---|---|
|模型|一连接一Goroutine|Multi-Reactor，少量Reactor协程管理大量连接，事件回调|
|编程范式|同步阻塞API（Read/Write），上手简单|事件回调模型，异步风格|
|epoll触发模式|ET 边缘触发|LT 水平触发|
|内存开销|海量连接下Goroutine数量暴涨，内存、调度开销高|连接数上涨，协程数量几乎不变，百万连接内存优势大|
|buffer|数据读到用户态由业务Read控制；大流式友好，不预读大量数据|内置buffer池，支持零拷贝NoCopy，小包RPC场景性能强|
|跨平台|全平台支持（Linux/macOS/Windows）|仅Linux、macOS；Windows不支持|
|TLS|原生完整支持TLS|TLS支持不完善|
|适用场景|通用业务、HTTP服务、大报文流式传输，开发简单|RPC、短小包、海量长连接，追求极致吞吐|

## 三、最容易混淆的三层，一句话理顺
1. **runtime.netpoll（运行时）**：Go语言内置IO多路复用，**是标准库net的底层底座**，属于Go runtime，业务代码看不到。
2. **GoNet（标准库 net包）**：上层封装，`net.Conn`，基于 runtime.netpoll，**一连接一G**。
3. **cloudwego/netpoll（第三方）**：独立网络库，**完全替代标准net**，自己封装epoll/kqueue，Reactor回调模型，和runtime.netpoll无关。

> 很多人踩坑：把 runtime.netpoll 和 cloudwego/netpoll 当成同一个东西。

## 四、Hertz里的切换（工程最常见场景）
Hertz框架可以二选一：
- `network: net` → 使用Go标准库 GoNet，底层走 runtime.netpoll
- `network: netpoll` → 使用字节 cloudwego/netpoll，不走标准net和runtime.netpoll

## 五、优缺点小结
✅ GoNet（标准net）
优点：稳定、跨平台、TLS完善、流式大报文友好、API简单
缺点：百万长连接场景，Goroutine数量巨大，调度开销上涨

✅ cloudwego/netpoll
优点：海量长连接、小包RPC场景性能强，内存占用低，零拷贝buffer
缺点：Windows不支持，TLS能力弱，事件回调开发复杂度更高，大报文场景优势减弱

如果你需要，我可以画一个架构调用链图，或者简单写两段最小demo对比 net 和 cloudwego/netpoll。