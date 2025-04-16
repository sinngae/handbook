channel
---
channel 是Go内置的类型，使用循环数组环和锁，提供协程间的通信和同步机制。

+ 无缓冲 VS 有缓冲
+ 写模式&读模式，只用于函数参数，限制函数内的操作；读写操作模式，双向通道
    + make(chan<- int) / make(<-chan int) / make(chan int)

注意事项：
+ 关闭一个未初始化的chan， panic
+ 重复关闭chan，panic
+ 向未初始化的chan发送，死锁
+ 向已关闭的chan发送，panic
+ 向chan发送，阻塞或成功
+ 向未初始化的chan接收，阻塞，死锁
+ 向已关闭的chan接收，缓冲区为空，则取到零值；否则可以成功
+ 正常，阻塞或成功
+ 多个消费者，随机消费。会不会有惊群效应？

使用场景：
消息传递、消息过滤，信号广播，事件订阅与广播，请求、响应转发，任务分发，结果汇总，并发控制，限流，同步与异步

# channel 底层原理
channel底层实现在src/runtime/chan.go中

channel内部是一个循环链表。内部包含buf, sendx, recvx, lock ,recvq, sendq几个部分；

buf是有缓冲的channel所特有的结构，用来存储缓存数据。是个循环链表；

sendx和recvx用于记录buf这个循环链表中的发送或者接收的index；
lock是个互斥锁；
recvq和sendq分别是接收(<-channel)或者发送(channel <- xxx)的goroutine抽象出来的结构体(sudog)的队列。是个双向链表。
channel是线程安全的。

# select & for 操作
for-select-channel