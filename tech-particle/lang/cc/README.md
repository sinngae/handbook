# C++
cpp-map C++知识图谱

+ C++98
    + STL including
    + containers and algorithms
    + strings
    + I/O streams
    + Templates
+ C++11
    + move semantic
    + unified initialization
    + Auto and decltype
    + Lambda fuctions
    + Multi-Threading
    + Regular expressions
    + Smart pointers
    + Hash Tables
    + std::array
    + memory model

## basis
[基础知识](basis/README.md)

C++语言的哲学思想：
+ 通用为本，专用为末
+ 拒绝GC（垃圾回收）
+ 追求提高性能即操作硬件的能力
+ 类型安全（编译器做更多的检查工作）

C++如何能做到新手易学，老兵易用？
C++标准的演进如何能更贴合实际？

## libstdc++与libc++

## STD
标准库

### STL
标准模板库

## boost
准标准库

## C++11
C++98之后新的里程碑

## faq
**C/C++如何实现一个barrier？**

**C/C++ 异步IO**

**构造函数或析构函数中调用虚函数**
派生类的构造过程：基类构造（包括虚函数表初始化）=》构造函数初始化数据=》虚函数表赋值
派生类的析构过程：？

**如何实现只用new构造的约束**
使用一个类包裹，把该类作为内部子类；

析构函数声明为私有，添加delete函数
