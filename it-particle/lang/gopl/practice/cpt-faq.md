FAQ
---
trouble shoot记录
# 1.go mod invalid pseudo-version
项目的依赖包更新了，go build报错`invalid pseudo-version`，起因是远程包的时间戳和gomod记录的时间戳对不上。
删除本地go.sum重新生成后也没解决，切换到go 1.12解决了

# Go特点
+ 协程：更轻量级的（相比于线程的上下文切换成本更低）IO型多任务模型
    + 语言层面解决了并发复杂度，而不是应用程序来解决
+ GC：垃圾回收，避免了很大一部分未定义的实现
+ 开发更便捷，丰富的特性和标准库，快速编译？

相比于C++，砍掉了90%了特性，减少了90%的麻烦。
更好的C。

## 为什么没有 泛型、try catch、assertion、隐式转换、?:三元操作符
为什么要有？
1. 运算符重载、方法重载
    + 这种功能解决了0.1%的问题，带来1%的代码混淆和可读性降低
2. interface的复合无法使用，比如`[]interface`和`[]int`并不同
    + 均衡
3. 隐式转换
    + 隐式转换带来更多问题，没有必要

## 为什么map非线程安全
是种权衡 性能和安全

## what's interface
>Under the covers, interfaces are implemented as two elements, a type and a value. The value, called the interface’s dynamic value, is an arbitrary concrete value and the type is that of the value. For the int value 3, an interface value contains, schematically, (int, 3). An interface value is nil only if the inner value and type are both unset, (nil, nil). In particular, a nil interface will always hold a nil type. If we store a nil pointer of type *int inside an interface value, the inner type will be *int regardless of the value of the pointer: (*int, nil). Such an interface value will therefore be non-nil even when the pointer inside is nil.
表面之下，每个变量实际都对应一个type和一个value，interface也是。
interface的value就是它的动态的值，是一个任意的具体的值；它的type是其value的type。
例如int 3，interface的值包括int和3 。
一个interface是nil，当且仅当其内部的value和type都是未设置的(nil, nil) 。
特别地，一个nil的interface总是type也是nil。一个nil指针的interface不是nil。

## Go不支持tagged or untagged union
tagged union是什么？
untagged union不安全，tagged union或者Variant types, algebraic type则和interface有重合

## 为什么map、slice、channel都是引用，而数组是值
slice是对数组的引用。
具体的场景中更方便，保留了数组提供更丰富的操作。

## 什么时候应该用interface指针
几乎永远不应该用interface指针。
>The insight is that although a pointer to a concrete type can satisfy an interface, with one exception a pointer to an interface can never satisfy an interface.
深入的理解是，虽然指向具体类型的指针可以满足接口，但除了一个例外，指向接口的指针永远不能满足接口。
传一个interface指针给一个interface会报错。
只能传interface指针给interface指针。

## Go程序为什么占用很多虚拟内存
+ mmap：创建一个新的虚拟内存区域，但这里需要注意，**就是当系统调用 mmap 时，它只是从虚拟内存中申请了一段空间出来，并不会去分配和映射真实的物理内存，而当你访问这段空间的时候，才会在当前时间真正的去分配物理内存。**那么对应到我们实际应用的进程中，那就是 VSZ 的增长后，而该内存空间又未正式使用的话，物理内存是不会有增长的。

madvise：提供有关使用内存的建议，例如：MADV_NORMAL、MADV_RANDOM、MADV_SEQUENTIAL、MADV_WILLNEED、MADV_DONTNEED 等等。
mprotect：设置内存区域的保护情况，例如：PROT_NONE、PROT_READ、PROT_WRITE、PROT_EXEC、PROT_SEM、PROT_SAO、PROT_GROWSUP、PROT_GROWSDOWN 等等。
sysctl：在内核运行时动态地修改内核的运行参数。

runtime-osinit：获取 CPU 核心数。
runtime-schedinit：初始化程序运行环境（包括栈、内存分配器、垃圾回收、P等）。
runtime-newproc：创建一个新的 G 和 绑定 runtime.main。
runtime-mstart：启动线程 M。

mallocinit
+ spans：记录 arena 区域页号和 mspan 的映射关系。
+ bitmap：标识 arena 的使用情况，在功能上来讲，会用于标识 arena 的哪些空间地址已经保存了对象。
+ arean：arean 其实就是 Go 的堆区，是由 mheap 进行管理的，它的 MaxMem 是 512GB-1。而在功能上来讲，Go 会在初始化的时候申请一段连续的虚拟内存空间地址到 arean 保留下来，在真正需要申请堆上的空间时再从 arean 中取出来处理，这时候就会转变为物理内存了。

工具：dtruss、strace、pprof

VSZ（进程虚拟内存大小）与共享库等没有太大的关系，主要与 Go Runtime 存在直接关联，也就是在前图中表示的运行时堆（malloc）。转换到 Go Runtime 里，就是在mallocinit这个内存分配器的初始化阶段里进行了一定量的虚拟空间的保留。

虚拟内存空间受不同的 OS 系统架构（GOARCH/GOOS）和位数（32/64 位）的影响。受内存对齐的影响，计算回来的内存空间大小是需要经过对齐才会进行保留。
+ VSZ 并不意味着你真正使用了那些物理内存，因此是不需要担心的。
+ VSZ 并不会给 GC 带来压力，GC 管理的是进程实际使用的物理内存，而 VSZ 在你实际使用它之前，它并没有过多的代价。
+ VSZ 基本都是不可访问的内存映射，也就是它并没有内存的访问权限（不允许读、写和执行）。

>The Go memory allocator reserves a large region of virtual memory as an arena for allocations. This virtual memory is local to the specific Go process; the reservation does not deprive other processes of memory.
To find the amount of actual memory allocated to a Go process, use the Unix top command and consult the RES (Linux) or RSIZE (macOS) columns.
Go内存分配器保留了一大块虚拟内存作为分配的场所。这个虚拟内存是特定Go进程的本地内存；保留不会剥夺其他进程的内存。

要查找分配给Go进程的实际内存量，请使用Unix top命令并查阅RES（Linux）或RSIZE（macOS）列。

Go是为了更高效地使用存储，所以提前分配了虚拟内存。

Go 的设计是考虑到 arena 和 bitmap 的后续使用，先提早保留了整个内存地址空间。 Go Runtime 和应用的逐步使用，肯定也会开始实际的申请和使用内存，这时候 arena 和 bitmap 的内存分配器就只需要将事先申请好的内存地址空间保留更改为实际可用的物理内存就好了，这样子可以极大的提高效能。

## select 原理
select源码位于src\runtime\select.go，最重要的scase 数据结构为：
```GO
type scase struct {
	c    *hchan         // chan
	elem unsafe.Pointer // data element
}
```
scase.c为当前case语句所操作的channel指针，这也说明了一个case语句只能操作一个channel。

scase.elem表示缓冲区地址：
caseRecv ： scase.elem表示读出channel的数据存放地址；
caseSend ： scase.elem表示将要写入channel的数据存放地址；

1. 锁定scase语句中所有的channel
2. 按照随机顺序检测scase中的channel是否ready
    + 如果case可读，则读取channel中数据，解锁所有的channel，然后返回(case index, true)
    + 如果case可写，则将数据写入channel，解锁所有的channel，然后返回(case index, false)
    + 所有case都未ready，则解锁所有的channel，然后返回（default index, false）
3. 所有case都未ready，且没有default语句
    + 将当前协程加入到所有channel的等待队列
    + 当将协程转入阻塞，等待被唤醒
4. 唤醒后返回channel对应的case index
    + 如果是读操作，解锁所有的channel，然后返回(case index, true)
    + 如果是写操作，解锁所有的channel，然后返回(case index, false)


