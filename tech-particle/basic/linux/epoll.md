epoll
---
IO多路复用，多个网络IO（也就是网络连接）复用同一资源来处理其数据到达、数据发送等读写操作。
+ BIO时期

## 历史
select()追溯到4.2BSD  
poll system从Linux 2.1.23开始支持  
epoll API从Linux kernel 2.5.44 glibc 2.3.2开始支持

+ BIO时期:一个线程一个连接
+ NIO时期：一个线程通过死循环轮询有数据的连接，通知程序处理
+ 多路复用器产生：select引入
+ 多路复用器优化：引入poll
+ 再优化引入epoll

1. BIO时期
前置知识：每个连接会建立一个socket，每个socket都有一个它自己的等待队列，当socket阻塞时就会把任务放到等待队列中，当它唤醒时就会调用等待队列中的任务

BIO时期客户端向内核发起连接时，在进程回应之前，这个连接的socket是阻塞的，导致要想建立多个连接就得开启多个线程。而线程开启是要消耗内核的，因此数量有限，并不能支持高并发

2. NIO时期
为了解决上述问题，内核做了优化，线程是非阻塞的了，也就是说可以用一个线程来处理多个连接，那么就可以建立一个死循环来轮询连接（这里实际上轮询的是socket的文件描述符fd，本质就是连接，为了方便理解后续也统一描述为连接，有兴趣可了解底层源码），看这个连接有没有数据到达，如果没有就下一个，如果有那么就开始处理

3. select引入
NIO时期听起来好像很不错了，但是实际上如果有1000个连接，那么就要遍历1000遍才能知道每个进程的连接是否有数据了。
这里要注意这1000个连接是针对整个服务器而言的，服务器中有各个进程，这些连接是分别对应各个进程的，一个进程可能会关注好几个连接，所以只有连接遍历完每个进程才知道自己关注的连接有没有数据进来。而这些遍历是是需要基于用户态内核态的切换的（这中间还涉及数据的拷贝等流程，具体这里不展开说了，有兴趣可额外了解），所以遍历的成本是很高的。

因此引入了IO多路复用器，一开始想到的办法是：预先传入一个连接列表，如果列表中的连接都没有数据，那么就挂起进程；如果某一个连接收到数据，那就唤醒对应的进程。这就是select的设计思想。

select的执行过程举例来讲就是：假如有3个socket连接到进程A上，那么进程A要关注的连接就有3个，在调用了select之后，操作系统会把进程A的引用分别加入到这三个连接（socket）的等待队列中，当任何一个socket收到数据后，网卡会发出中断信息，执行中断程序，socket等待队列中的进程A就会被唤醒。但是因为没有地方记录是哪些socket收到了数据，所以进程A是不知道具体哪个连接有数据的，所以进程A还需要把3个连接都遍历一遍，来找到有数据的连接（可能是1个也可能是多个连接有数据），得到有数据的连接后，select的工作就完成了，再由进程自身来完成后续的数据的读写操作

NIO时期要遍历，select还是要遍历，那select的优化是优化了什么？

        如果你没有这个疑问就跳过。上述所说的1000个连接的遍历，是指通过遍历去找有没有socket有数据来了，并且唤醒对应的进程。但是同样的，唤醒后的进程仍然不知道自己关注的socket中是哪些有数据，所以是还要再遍历一遍的，这第二次遍历就是select也要做的遍历，所以select的优化在于它省掉了第一次遍历


4. poll引入
由上述的select过程可以知道，select也是需要再遍历自己关注的连接的。每次调用select是需要将进程引入添加到每个监听的socket的等待队列中的，并且还是需要遍历，所以仍然需要较大的成本的。因此规定了select的最大监听数量，默认是只能监听1024个socket

后来引入基于链表存储的poll多路复用器，没有了最大监听数的限制，但是本质上并没有多大改变，所以在性能上和select差别不大

5. epoll
看到这里请思考一个问题：select的缺点在哪儿？考虑清楚后往下看


### epoll、select、poll的区别
+ 1.select最大并发数限制1024个，可以修改  
可以更改数量，但由于select采用轮询的方式扫描文件描述符，文件描述符数量越多，性能越差；  
在linux内核头文件中，有这样的定义：#define __FD_SETSIZE    1024
epoll最大并发连接数没有限制，受系统中资源如内存的限制
+ 2.select效率低，每次线性扫描整个fd_set，集合越大速度越慢  
select返回的是含有整个句柄的数组，应用程序需要遍历整个数组才能发现哪些句柄发生了事件  
epoll通过内核与用户控件mmap同一块内存实现，省去不必要的内存拷贝
+ 3.内核/用户空间内存拷贝问题  
select需要复制大量的句柄数据结构，产生巨大的开销；
+ 4.select的触发方式是水平触发  
应用程序如果没有完成对一个已经就绪的文件描述符进行IO操作，那么之后每次select调用还是会将这些文件描述符通知进程。  
epoll只有活跃的socket才会主动调用callback，效率提升。epool的ET边缘触发模式，只触发一次。

相比select模型，poll使用链表保存文件描述符，因此没有了监视文件数量的限制，但其他三个缺点依然存在。  
并发量低的情况，socket都比较活跃，select不一定比epoll慢；  
高并发时，任意时间，只有少数socket是活跃的，epoll效率更高：
许多测试中，如果没有大量的idle_connection和dead_connection，epoll的效率并不比select/poll高很多，反过来就有很大提高。
### 大部分场景
有100万个客户端同时与一个服务器进程保持着TCP连接。而每一时刻，通常只有几百上千个TCP连接是活跃的。高峰时间段会很高。

### 流的概念
文件、socket、pipe、等可以进行I/O操作的对象；
流的接口，read/write，读取和写入；
场景：从流中读取数据，但是流中还没有数据。
阻塞：等待信号唤醒，陷入睡眠状态；

=>

非阻塞忙轮询，明显是个坏方法

阻塞 vs 非阻塞 vs 阻塞超时

### recv/send/write/read 返回值
对于非阻塞或超时，当缓冲区满或缓冲区空，write/read会返回-1，errno=EAGAIN（Resource temporarily unavilable）（windows上是EWOULDBLOCK？Linux也有，是非阻塞和阻塞的区别么？）。

errno=EINTR，表示操作被系统中断唤醒，需要重新读/写。（EAGAIN可以模拟，EINTR如何模拟？是被操作系统别的信号中断了？）

recv/send/write/read返回值，一般>0 - 收/发/写/读到数据；=0 - 连接断开；<0 - 如果errno in (EAGAIN/EWOULDBLOCK/EINTR)则（过段时间）重试，否则连接异常。
### 阻塞机制
四个事件
 + 缓冲区非空  
 缓冲区状态为空，读者阻塞。缓冲区被写入数据后，内核产生把读者唤醒的事件叫做“缓冲区非空”事件。
 + 缓冲区满
 无读者（内核保证缓冲区数据不丢失），写者写缓冲区直到缓冲区状态为满，内核产生把写者阻塞的事件叫做“缓冲区满”事件。
 + 缓冲区非满
 读者读数据，内核产生把写者唤醒的事件叫做“缓冲区非满”事件。
 + 缓冲区空
 读者把缓冲区数据读完，内核产生把读者阻塞的事件叫做“缓冲区空”事件。

四个事件实现阻塞同步，使用到OS内核的锁/信号量/条件变量。阻塞模式下，内核通过阻塞和唤醒处理I/O事件。非阻塞模式下，把I/O事件交给select/poll或epoll处理或者忽略。

如果所有流都没有数据，则阻塞轮询空转，白白浪费CPU：
```cpp
while (true) {
    for /* i in stream[] */ {
        if /*i has data*/ {
            // read until unavilable
        }
    }
}
```
引入代理，空闲时阻塞当前线程，有流I/O事件时唤醒它进入轮询：
```cpp
while (true) {
    select(stream[]);
    for /* i in stream[] */ {
        if /*i has data*/ {
            // read until unavilable
        }
    }
}
```
select仅知道I/O事件发生，却不知道那几个流，需要遍历所有，O(n)。

**非阻塞必要轮询?**

### 多路复用
multiplexing，一个信道可以处理多个信号。
多路复用本身是数据通信里或者互联网里的概念，指同一信道里同时传输多路信号，有时分多址、码分多址、频分多址等。
select/poll/epoll一个轮询里处理多个流事件。

## epoll机制
epoll，event poll，与select不同，epoll将哪些流的哪些事件返回，O(k)或O(1)。

epoll通过在Linux内核中申请一个简易的文件系统(文件系统一般用什么数据结构实现？红黑树和双向链表)，把原先的select/poll调用分成了3个部分：
```cpp
/*
创建epoll对象(在epoll文件系统中为这个句柄对象分配资源);
size告诉内核监听的fd数目有多大，从linux 2.6.8被忽略，但仍需要大于0（尤其是运行在更早的内核）；
占用一个fd，需要使用close()释放，否则导致句柄泄漏；
ls /proc/[pid]/fd/ 可查看进程占用的句柄；
*/
int epoll_create(int size);

typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;

struct epoll_event {
    uint32_t     events;      /* Epoll events */
    epoll_data_t data;        /* User data variable */
};
/*
向epoll对象中添加百万个连接的套接字，相当于epoll_add/epoll_del/epoll_mod的合体。
fd：流对象
op：EPOLL_CTL_ADD/EPOLL_CTL_MOD/EPOLL_CTL_DEL
event.events：
    EPOLLIN - 文件描述符可读（包括对端socket关闭);
    EPOLLOUT - 文件描述符可写；
    EPOLLPRI - 有紧急数据可读（带外数据，Out-Of-Band data);
    EPOLLERR - 发生错误；
    EPOLLHUB - 被挂断；
    EPOLLET - 设置为ET模式；
    EPOLLONESHOT - 只监听一次事件，当监听完这次事件后，如果不需要监听，需要再次加入epoll队列；
*/
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

/*
等待事件发生
events: 内核收集的发生的事件集合；
maxevents：events的大小，不大于epoll_create的size参数；
timeout：超时时间，毫秒，0则立即返回，-1则阻塞模式
    如果主循环是单线程，则可以用-1；如果和别的业务共用线程，则可以用0；
返回需要处理的事件的数量，0则表示超时
*/
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
```

epoll只关心缓冲区非满和非空事件(EPOLL_IN/EPOLL_OUT并不等同于缓冲区非满/非空)。
```cpp
while (true) {
    activeStream[] = epoll_wait(epfd);
    for /* i in activeStream[] */ {
        // read or write until unavilable
    }
}
```

通过红黑树/双向链表数据结构，并结合回调机制（内核？业务层？），实现epoll高效  
每个epoll对象在内核都有一个独立的eventpoll结构体，包含一个红黑树rbr，用于存放epoll_ctl添加的事件（重复的添加会被识别报错）。所有添加的事件都都与设备（如网卡）驱动程序建立回调关系，事件发生时，将发生的事件添加到双向链表rdlist。epoll_wait只需检查rdlist是否为空，不为空则返回（mmap，没有内核/用户内存拷贝）。
### 两种模式
+ ET，Edge Trigger，边缘触发，只有数据到来，才触发；不管缓冲区中是否有数据。
+ LT，Level Trigger，水平触发，只要缓冲区中有数据，就会触发。（缺省默认）

理论上，ET比LT更有效，减少了系统调用，提高了并行效率，有待实测。

ET模式下，读数据的时候，当recv返回的大小如果等于请求的大小，那么很有可能是缓冲区还有数据需要再次处理；如果小于请求的大小或者产生EAGAIN，则已读完；如果等于0，则是对端的socket关闭；

**epoll读取比发送要快的情况**  
*在发送端流量比接收端要多的时候

socket设为非阻塞，send函数虽然返回，但实际只是写到缓冲区，缓冲区满后返回EAGAIN错误。
封装一个函数send_handler:
1. 该函数尽量将数据写入缓冲区再返回，返回-1表示错误。
2. 当缓冲区满，send返回-1，errno=EAGAIN，稍等后重试，
这样就在send_handler阻塞一段时间。

**LT模式，socket可写时，一直触发可写事件**
1. 需要写数据的时候，才把socket加入epfd，等待可写事件，写后移出epfd；
2. 改进：一开始不把socket加入epfd，write/send返回EAGAIN再加入，在epfd事件驱动下写数据，发送后移出epfd；(可以减少一些epoll调用)

**epoll调用频繁问题**


**更好的实现？**
操作系统支持用户进程回调函数拷贝数据到网卡？
内核是问题本源呀

windows的IOCP，真正意义上的网络异步I/O  
process在recvfrom操作的两个处理阶段上都不能等待，也就是process调用recvfrom后立刻返回，kernel自行去准备好数据并将数据从kernel的buffer中copy到process的buffer，通知process读操作完成了，然后process继续处理。

non-blocking I/O和asynchronous I/O的区别  
non-blocking I/O仅仅要求等待数据时不阻塞即可，而asynchronous I/O要求从内核到用户进程拷贝也不阻塞。

完全异步I/O的好处有待验证。

