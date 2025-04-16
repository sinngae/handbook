Go Memory Management
---

golang内存管理基本是参考tcmalloc来进行的。go内存管理本质上是一个内存池，只不过内部做了很多优化：自动伸缩内存池大小，合理的切割内存块。

一些基本概念：
+ 页Page：一块8K大小的内存空间。Go向操作系统申请和释放内存都是以页为单位的。
+ span : 内存块，一个或多个连续的 page 组成一个 span 。如果把 page 比喻成工人， span 可看成是小队，工人被分成若干个队伍，不同的队伍干不同的活。
+ sizeclass : 空间规格，每个 span 都带有一个 sizeclass ，标记着该 span 中的 page 应该如何使用。使用上面的比喻，就是 sizeclass 标志着 span 是一个什么样的队伍。
+ object : 对象，用来存储一个变量数据内存空间，一个 span 在初始化时，会被切割成一堆等大的 object 。假设 object 的大小是 16B ， span 大小是 8K ，那么就会把 span 中的 page 就会被初始化 8K / 16B = 512 个 object 。所谓内存分配，就是分配一个 object 出去。

# mheap
一开始go从操作系统索取一大块内存作为内存池，并放在一个叫mheap的内存池进行管理，mheap将一整块内存切割为不同的区域，并将一部分内存切割为合适的大小。
+ mheap.spans ：用来存储 page 和 span 信息，比如一个 span 的起始地址是多少，有几个 page，已使用了多大等等。
+ mheap.bitmap 存储着各个 span 中对象的标记信息，比如对象是否可回收等等。
+ mheap.arena_start : 将要分配给应用程序使用的空间。

# mcentral
用途相同的span会以链表的形式组织在一起存放在mcentral中。这里用途用sizeclass来表示，就是该span存储哪种大小的对象。

找到合适的 span 后，会从中取一个 object 返回给上层使用。

# mcache
为了提高内存并发申请效率，加入缓存层mcache。每一个mcache和处理器P对应。Go申请内存首先从P的mcache中分配，如果没有可用的span再从mcentral中获取。

参考资料：Go 语言内存管理（二）：Go 内存管理

