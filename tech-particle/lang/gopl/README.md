# gopl
本案用于搜集整理 Golang 知识。

初见go语言，惊艳于go语言的设计之精巧，对照go语言的设计，常常能更深刻理解C/C++的设计思想。

Go的语法接近C语言，但对于变量的声明有所不同。
Go的GC实现、GMP模型设计。
Go的并行模型，channel的设计。
在1.8版本中开放插件（Plugin）的支持，这意味着现在能从Go中动态加载部分函数。
与C++相比，Go并不包括如枚举、异常处理、继承、泛型、断言、虚函数等功能，但增加了itoa、error、切片(Slice) 型、并发、管道、垃圾回收、接口（Interface）等特性的语言级支持。
Go 2.0版本将支持泛型？
Go内嵌了关联数组（也称为哈希表（Hashes）或字典（Dictionaries）），就像字符串类型一样。

*本案所引用资料均会在适当的位置说明，没有说明到的，敬请指出。

## 目录
### [第一篇 基础篇](basic/README.md)
1. [第一章 程序结构](basic/cpt1-destruct.md)
2. [第二章 基础数据类型](basic/cpt2-valuetype.md)
3. [第三章 复合数据类型](basic/cpt3-composetype.md)
4. [第四章 函数](basic/cpt4-function.md)
5. [第五章 方法](basic/cpt5-method.md)
6. [第六章 接口类型](basic/cpt6-interface.md)
7. [第七章 goroutine](basic/cpt7-goroutine.md)
8. [第八章 反射](basic/cpt8-reflect.md)

### [第二篇 标准库](standard/README.md)
1. []()
2. []()

PS:
+ channel底层实现 => src/runtime/chan.go
+ GMP实现 =》 src/runtime/runtime2.go
    + 调度 =》 src/runtime/proc.go
+ GC实现 =》 src/runtime/mgc.go
+ netpoll => src/runtime/netpoll.go
+ map => src/runtime/map.go

### [第三篇 进阶](advanced/README.md)
1. []()
2. []()

### [第四篇 实战](practice/README.md)
1. []()
2. []()

### [其他]
1. Go汇编 

https://lrita.github.io/2017/12/12/golang-asm/

https://golang.org/doc/asm
## 附录
golang wiki：https://github.com/golang/go/wiki/Projects