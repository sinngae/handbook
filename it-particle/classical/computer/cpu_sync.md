# CPU有哪些同步机制

CPU 的同步机制是一个从硬件微架构到指令集架构（ISA）再到内存模型的分层设计。以下按从底层到上层的逻辑梳理：

## 一、原子操作指令（Atomic Instructions）
这是同步的基石，保证对单个内存位置的"读-改-写"不可中断。
表格
机制	说明	典型指令
TAS (Test-And-Set)	读取旧值并写入 1，原子完成	x86: xchg, bts
CAS (Compare-And-Swap)	比较并交换，现代并发核心原语	x86: cmpxchg (配合 LOCK)
FAA (Fetch-And-Add)	原子读取并增加	x86: xadd (配合 LOCK)
LL/SC (Load-Link/Store-Conditional)	RISC 架构的原子对指令，无总线锁开销	ARM: LDXR/STXR, RISC-V: LR/SC
关键实现：x86 通过 LOCK 前缀锁定缓存行或总线；LL/SC 则通过监控地址是否被其他核心修改来实现。

## 二、内存屏障 / 栅栏（Memory Barrier / Fence）
编译器和 CPU 都可能重排指令以优化性能，屏障用于强制约束重排边界。
表格
类型	语义	典型场景
LoadLoad	屏障前的读操作不能重排到屏障后的读操作之后	消费者等待生产者写入数据
StoreStore	屏障前的写操作不能重排到屏障后的写操作之后	先发布数据，再设置标志位
LoadStore	屏障前的读不能重排到屏障后的写之后	较罕见，某些模型隐含
StoreLoad	屏障前的写对后续读可见（最强，也最慢）	实现完全顺序一致性
全屏障 (Full Fence)	以上全部	内核自旋锁、信号量实现

x86 的简化：由于 x86 是强内存模型（TSO），LOCK 指令和 mfence 已隐含全屏障语义。sfence（写屏障）用于非临时存储（Non-Temporal Store），lfence（读屏障）主要用于 SSE/AVX 指令序。

## 三、缓存一致性协议（Cache Coherence）
多核同步的物理基础。

最经典的是 MESI 及其扩展：
表格
状态	含义
M (Modified)	本核独占，已修改，与内存不一致
E (Exclusive)	本核独占，未修改，与内存一致
S (Shared)	多核共享，只读
I (Invalid)	无效，需重新加载

同步作用：
当核 A 写入一个 Shared 状态的缓存行时，必须向其他核广播 Invalidate 消息，使它们的副本失效。
这构成了可见性的硬件保证：一个核的写入，通过缓存一致性协议传播到其他核。
x86 的 LOCK 前缀往往只是锁定缓存行（Cache Line Locking），而非整个总线，依赖 MESI 保证原子性。

## 四、总线锁定 vs 缓存锁定（Bus Locking / Cache Locking）
表格
方式	机制	开销
总线锁定	锁定整个前端总线/内存总线，禁止其他核内存访问	极大，现代 CPU 尽量避免
缓存锁定	仅锁定目标缓存行（Cache Line），利用 MESI 的 Exclusive 状态	小，但要求操作完全在缓存行内且对齐

触发总线锁的情况：
操作跨缓存行（未对齐的 64 位操作在 32 位系统上）
操作目标不在缓存中（需回内存）
某些早期的原子指令实现

## 五、内存序与内存模型（Memory Ordering / Memory Model）
不同 ISA 对重排的约束不同，直接影响同步机制的设计：

表格
模型	代表	特点
顺序一致性 (SC)	理论模型	无重排，程序顺序即执行顺序
TSO (Total Store Order)	x86/x86-64	Store 按程序顺序，Store→Load 可重排（通过 Store Buffer），读自身写立即可见
弱序 (Weak Ordering)	ARM, RISC-V, PowerPC	几乎允许任意重排，必须显式使用屏障/原子指令保证顺序

对编程的影响：
x86 上写个无锁队列相对简单，因为 TSO 已经保证了很多顺序。
ARM 上实现同样的逻辑，必须在关键位置插入 dmb（数据内存屏障）、dsb（数据同步屏障）或 isb（指令同步屏障）。

## 六、Store Buffer 与 Invalidate Queue
这是理解"为什么需要内存屏障"的关键微架构细节：

plain
核心 A 写入 X：
  → 写入 Store Buffer（先不刷缓存，提高性能）
  → 发送 Invalidate 给其他核心
  → 其他核心把 Invalidate 放入 Invalidate Queue（延迟处理）
  → 核心 A 继续执行

问题：核心 B 读 X 时，可能从自己的缓存读到旧值，
      因为 Invalidate 还在 B 的 Queue 里没处理。
      
解决：内存屏障会刷新 Store Buffer 和/或排空 Invalidate Queue。

## 七、总结：同步机制的分层视图
plain
┌─────────────────────────────────────┐
│  软件层：mutex, semaphore, rwlock   │  ← 由下层原语实现
├─────────────────────────────────────┤
│  原子指令：CAS, FAA, LL/SC + LOCK   │  ← 保证单操作原子性
├─────────────────────────────────────┤
│  内存屏障：lfence, sfence, mfence   │  ← 约束指令重排
├─────────────────────────────────────┤
│  缓存一致性：MESI + Invalidate      │  ← 保证多核可见性
├─────────────────────────────────────┤
│  微架构：Store Buffer, Inv. Queue   │  ← 性能优化，也是"乱序"根源
├─────────────────────────────────────┤
│  物理层：总线仲裁、缓存锁定           │  ← 真正的互斥执行
└─────────────────────────────────────┘
一句话：CPU 的同步不是单一机制，而是"原子指令保证操作不可分割 + 内存屏障保证顺序可见 + 缓存一致性保证传播"的三位一体。