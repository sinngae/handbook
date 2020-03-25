# C++/C 知识图谱

## C

### C basis

### libc/glibc/glib
libc实际上是泛指实现了C标准规定的库，都是一种libc 。
libc是Linux下的ANSI C函数库，详见[libc.md](libc.md)。
glibc是GNU组织对libc的一种实现，是unix/linux的根基之一。
微软也有自己的 libc实现，叫msvcrt 。
嵌入式行业里还常用uClibc，是一个迷你版的libc 。

Linux下原来的标准c库Linux libc逐渐不再被维护。Linux下面的标准c库不仅有这一个，如uclibc、klibc，以及上面被提到的Linux libc，但是glibc无疑是用得最多的。
```sh
# glibc version
ldd  --version
/lib/libc.so.6
```
#### glibc
glibc是Linux下的GUN C函数库。GNU C函数库是一种类似于第三方插件的东西。glibc是linux下面C标准库的实现，即GNU C Library。glibc本身是GNU旗下的C标准库，后来逐渐成为了Linux的标准c库。glibc在/lib目录下的.so文件为libc.so.6。

#### glib
glib是用C写的一些utilities，即C的工具库，和libc/glibc没有关系。
glib是GTK+的基础库，它由基础类型、对核心应用的支持、实用功能、数据类型和对象系统五个部分组成。
glib 是 Gtk+ 库和 Gnome 的基础。
glib 可以在多个平台下使用，比如 Linux、Unix、Windows 等。
glib 为许多标准的、常用的 C 语言结构提供了相应的替代物。 

glib是一个综合用途的实用的轻量级的C程序库，它提供C语言的常用的数据结构的定义、相关的处理函数，有趣而实用的宏，可移植的封装和一些运行时机能，如事件循环、线程、动态调用、对象系统等的API。GTK+是可移植的，当然glib也是可移植的，可以在linux下，也可以在windows下使用它。使用gLib2.0（glib的2.0版本）编写的应用程序，在编译时应该在编译命令中加入`pkg-config --cflags --libs glib-2.0`，如：
`gcc pkg-config --cflags --libs glib-2.0 hello.c -o hello`
使用glib最有名的就是GNOME了。

## C++
### STD
标准库
#### STL
标准模板库

### boost
准标准库

### C++11
C98之后新的里程碑

### Lua
简介优美的脚本语言，非常亲和C/C++

## faq
**C/C++如何实现一个barrier？**

**C/C++ 异步IO**

**构造函数或析构函数中调用虚函数**
派生类的构造过程：基类构造（包括虚函数表初始化）=》构造函数初始化数据=》虚函数表赋值
派生类的析构过程：？

**如何实现只用new构造的约束**
使用一个类包裹，把该类作为内部子类；

析构函数声明为私有，添加delete函数
