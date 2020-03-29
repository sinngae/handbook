# Go语言专题
初见go语言，惊艳于go语言的设计之精巧，对照go语言的设计，常常能更深刻理解C/C++的设计思想。

Go的语法接近C语言，但对于变量的声明有所不同。
Go支持垃圾回收功能。
Go的并行模型，通道传输。
在1.8版本中开放插件（Plugin）的支持，这意味着现在能从Go中动态加载部分函数。
与C++相比，Go并不包括如枚举、异常处理、继承、泛型、断言、虚函数等功能，但增加了itoa、error、切片(Slice) 型、并发、管道、垃圾回收、接口（Interface）等特性的语言级支持。
Go 2.0版本将支持泛型？
Go内嵌了关联数组（也称为哈希表（Hashes）或字典（Dictionaries）），就像字符串类型一样。
## Go 类型安全

## Go routine并发模型
http://www.sizeofvoid.net/goroutine-under-the-hood/

## Go GC
垃圾回收机制


## Go 多进程多线程机制
没有创建多线程和多进程的api，但是有控制使用CPU多核使用的api，runtime.GOMAXPROCS(1)