# 核心前提
Go里分两类阻塞：
1. **网络IO（net包）**：底层全部改成**非阻塞socket + runtime netpoller(epoll/kqueue)**，G会被park，**M不会阻塞、不会剥离P**，这是Go网络高并发的基础。
2. **真正会阻塞M、触发`entersyscall`把P剥离**的：**直接阻塞型syscall**，M陷入内核睡眠；这类里面，**有一部分没有简单的O_NONBLOCK非阻塞版本可以直接替换**（普通文件IO、waitpid、cgo调用等）。

> 一句话：**socket可以非阻塞+poll；但是普通磁盘文件、子进程等待、cgo阻塞调用，没有简单的非阻塞替代方案。**

## 一、普通文件的 read / write（磁盘IO，最典型）
`read(2)` / `write(2)` 操作**普通文件（磁盘文件）**
- 即使设置`O_NONBLOCK`，**对普通文件fd是无效的**，内核仍然可能阻塞（缺页、磁盘寻道、页面回写），不会返回EAGAIN
- epoll/kqueue**不能监听普通文件fd**，epoll认为普通文件永远就绪
- Linux AIO只支持O_DIRECT裸IO，不支持page cache，使用限制巨大，不能当做通用替代；io_uring是现代方案，但不属于简单的“设置非阻塞标志”替代，标准库没有默认走io_uring

✅ 结论：
Go `os.Open`打开本地文件，`Read()`/`Write()` 触发阻塞syscall，**M阻塞，P剥离交给其他M**；**没有简单的非阻塞fd方式替代**。
> 编码影响：大量并发读大文件，会不断创建M（内核线程），M暴涨，sys CPU变高。

## 二、wait4 / waitpid：等待子进程退出
`wait4()`，Go的`syscall.Wait4`、`exec.Cmd.Wait()`底层
- 作用：阻塞等待子进程退出，子进程没退出，线程永久在内核休眠
- **没有O_NONBLOCK可以设置**。虽然可以传`WNOHANG`变成轮询，但这是**轮询**不是非阻塞IO；轮询会疯狂占用CPU，不是优雅的事件通知方案。
- epoll无法监听子进程。只能信号+轮询，复杂度很高，不是等价替代。

✅ 结论：`cmd.Wait()` 会阻塞M，触发P剥离；没有直接的非阻塞syscall替代。

## 三、pipe / fifo（管道）的阻塞读/写（区分一下）
pipe**可以设置O_NONBLOCK**，也能被epoll监听，所以**不属于这一类**。Go如果自己封装pipe，可以走poller。
> 注意：只有**普通磁盘文件不行，pipe/socket/tty可以非阻塞+epoll**。

## 四、cgo调用阻塞C函数（非常高频面试）
cgo调用C代码，如果C内部调用阻塞syscall（比如C里面读文件、锁、sleep）：
- Go runtime 会执行`entersyscall`，**剥离P，M阻塞**
- 本质：cgo调用会进入C栈，runtime不知道C内部在干嘛，**无法用Go netpoller接管**
- 没有办法简单改成非阻塞，除非C代码自己改成异步。

> 重点：哪怕C里面只是pthread_mutex锁等待，Go侧看就是阻塞M、P剥离。

## 五、其他不容易简单非阻塞的syscall
1. `open`打开某些特殊文件/设备（比如打开某些字符设备，等待设备就绪）：部分设备支持O_NONBLOCK，但很多硬件设备打开操作本身仍可能阻塞，没有通用替代。
2. `nanosleep`：直接调用底层`syscall.Nanosleep()`会阻塞M；⚠️ 但是`time.Sleep`不是！Go标准库time.Sleep是runtime timer，G被park，**M不阻塞，不会剥离P**。这点面试坑。
3. futex_wait：注意，**Go runtime内部的futex_wait（M空闲休眠）是runtime自己管理的**；业务代码不会直接调用futex。业务层sync.Mutex阻塞是用户态Gpark，**不会阻塞M**。

# ❗ 区分：哪些阻塞**不会剥离P**（netpoller接管，M不阻塞）
- `net.Conn Read/Write`：socket设置O_NONBLOCK，G挂到netpoller，M继续跑其他G，**M不会阻塞**
- channel、sync.Mutex、sync.Cond：G进入等待队列，M直接调度其他G，用户态阻塞，**不进syscall、不剥离P**
- `time.Sleep`：runtime timer，G休眠，M继续跑别的G

# 📌 编码层面带来的影响（回到GMP，写代码的价值）
1. **大量并发本地文件读写**：每一个阻塞Read都会剥离P、新建M；M数量暴涨，OS内核线程切换开销飙升，`top`看到`sys`占用高。
    > 优化方案：不要无限开goroutine读文件，用worker池限制并发文件IO数量，控制M的上限。
2. **大量exec.Cmd并发等待子进程**：每个`cmd.Wait()`阻塞M，M膨胀，同样需要worker池。
3. **cgo阻塞调用**：和文件IO一样，阻塞会创建大量M，cgo并发高的时候性能灾难；尽量减少阻塞cgo调用。

# 面试一句话总结
- Socket网络IO：可以设置O_NONBLOCK + epoll，Go netpoller接管，**不会阻塞M**；
- 本地普通磁盘文件read/wait4子进程等待、阻塞cgo：**没有简单的O_NONBLOCK非阻塞syscall替代**；一旦调用，M在内核阻塞，runtime执行`entersyscall`剥离P，P交给别的M继续跑G，代价是M（内核线程）数量动态上涨。

如果你想，我可以举一个**pprof/trace例子**：一段并发读本地文件代码，trace中看到大量`Gsyscall`、M增多，结合GMP分析怎么优化。

----
