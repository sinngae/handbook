lock
---

操作系统锁是为了防止多处理器并发访问临界区。
锁也造成了线程的竞争。

分类
+ spinlock 自旋锁
    + 获取不到锁，则自旋等待，忙等，不会睡眠
    + 不可抢占，可被中断
    + 一直处于用户态，不会做系统调用（不会陷入内核）
    + 缺点：一直占用cpu，而且持有锁既锁bus总线，其他处理器不能使用bus总线。
+ mutex 互斥锁
    + 获取不到锁，则sleep，挂起当前线程
    + mutex获取锁分为两阶段
        + 第一阶段在用户态采用spinlock锁总线的方式获取一次锁，如果成功立即返回；
        + 否则进入第二阶段，调用系统的futex锁去sleep，当锁可用后被唤醒，继续竞争锁。
    + 缺点：mutex 会陷入内核，进入昂贵的系统调用
+ futex - fast mutex
+ semaphore 信号量
    + 控制并发度
    + 获取不到锁，则等待？
+ 读写锁
    + 作用：区分读写操作，支持读操作的并发、写操作的互斥
    + 工作模式 Go Write-Prefer
        + Read-preferring 优先读，可能导致写饥饿
        + Write-preferring 优先写，避免写饥饿
        + 不指定优先，无饥饿问题
    + 实现
        + 使用互斥锁和信号量实现

试用场景
+ spinlock 适用锁持有时间较短的场景，睡眠是不必要的，自旋锁效率远高于互斥锁。
+ semaphore 适用锁持有时间较长的场景，

faq
+ AA锁问题 重复加锁导致的死锁
    + 需要支持recursive 递归锁即可解决
+ AB-BA锁问题 互相持有导致的互相等待的死锁
    + 需要设计锁的添加顺序
    + 一般操作系统都有死锁检测
+ 获得锁之后，线程崩溃了，锁会释放掉么
    + 会造成未定义的行为
+ spinlock 基于硬件的CAS（compare and swap)?
    + rwlock是基于spinlock实现的
    + mutex的实现也依赖spinlock

# 原子操作与spinlock
如果只是原子地增加变量值，使用原子操作即可，但上锁场景更加复杂。
原子操作也是通过硬件实现（总线锁定、缓存锁定），是轻量级的操作，比陷入内核要轻量很多了。

单处理器中，单条指令都可以认为是原子操作，中断只能发生在指令之间。

在SMP（Symmetric Multi-Processor，对称多处理器）体系结构中：
原子性不可能由软件单独保证--必须需要硬件的支持，因此是和架构相关的。在x86 平台上，CPU提供了在指令执行期间对总线加锁的手段。CPU芯片上有一条引线#HLOCK pin，如果汇编语言的程序中在一条指令前面加上前缀"LOCK"，经过汇编以后的机器代码就使CPU在执行这条指令的时候把#HLOCK pin的电位拉低，持续到这条指令结束时放开，从而把总线锁住，这样同一总线上别的CPU就暂时不能通过总线访问内存了，保证了这条指令在多处理器环境中的原子性。

在所有支持的体系结构上原子类型atomic_t都保存一个int值。在x86的某些处理器上，由于工作方式的原因，原子类型能够保证的可用范围只有24位。volatile是一个类型描述符，要求编译器不要对其描述的对象作优化处理，对它的读写都需要从内存中访问。

# spinlock的多种实现方式
1. CAS compare and swap

最古老的一种做法是：spinlock用一个整形变量表示，其初始值为1，表示available的状态。当一个CPU（设为CPU A）获得spinlock后，会将该变量的值设为0，之后其他CPU试图获取这个spinlock时，会一直做CAS，直到CPU A释放spinlock，并将该变量的值设为1。

那么其他的CPU是以何种形式等待的，如果有多个CPU一起等待，形成了竞争又该如何处理？这里要用到经典的CAS操作（Compare And Swap）。

CAS 很快，但是缺点：它是「不公平」的。 一旦spinlock被释放，第一个能够成功执行CAS操作的CPU将成为新的owner，没有办法确保在该spinlock上等待时间最长的那个CPU优先获得锁，这将带来延迟不能确定的问题。


2. Ticket Spinlock
采用排队形式的"ticket spinlock"。如 x86-64 ACRN版本、Linux实现。

使用ticket spinlock可以让CPU按照到达的先后顺序，去获取spinlock的所有权，形成了「有序竞争」。根据硬件维护的cache一致性协议，如果spinlock的值没有更改，那么在busy wait时，试图获取spinlock的CPU，只需要不断地读取自己包含这个spinlock变量的cache line上的值就可以了，不需要从spinlock变量所在的内存位置读取。

但是，当spinlock的值被更改时，所有试图获取spinlock的CPU对应的cache line都会被invalidate，因为这些CPU会不停地读取这个spinlock的值，所以"invalidate"状态意味着此时，它们必须重新从内存读取新的spinlock的值到自己的cache line中。

而事实上，其中只会有一个CPU，也就是队列中最先达到的那个CPU，接下来可以获得spinlock，也只有它的cache line被invalidate才是有意义的，对于其他的CPU来说，这就是做无用功。内存比cache慢那么多，开销可不小。