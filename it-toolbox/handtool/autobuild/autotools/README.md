# 概述
使用autotools工具就是一个不错的选择,只要输入工程中的目标文件、依赖文件、文件目录等信息就可以自动生成Makefile。

autotools工具是个系列工具，主要有：aclocal、autoscan、autoconf、autoheader、automake。

## Autotools使用流程
+ 1.目录树的最高层运行autoscan，生成configure.scan文件；
+ 2.运行aclocal，生成aclocal.m4文件；
+ 3.运行autoconf，生成configure配置脚本；
+ 4.运行autoheader，生成config.h.in文件；
+ 5.手工编写Makefile.am文件；
+ 6.运行automake，生成Makefile.in；
+ 7.运行配置脚本configure，生成Makefile

## 解析
|名称|功能|
|---|---|
|autoscan|autoscan是用来扫描源代码目录生成configure.scan文件的。configure.scan包含了系统配置的基本选项，里面都是一些宏定义。我们需要将它改名为 `configure.in`|
|aclocal|aclocal是一个perl脚本程序。aclocal根据configure.in文件的内容，自动生成aclocal.m4文件。aclocal的定义是：“aclocal - create aclocal.m4 by scanning configure.ac”。生成的aclocal.m4是宏展开文件|
|autoconf|autoconf是用来产生configure文件的configure.in文件的内容是一些宏，这些宏经过autoconf处理后会变成检查系统特性、环境变量、软件必须的参数的shell脚本|
|autoheader|自动生成`config.h.in`，在configure生成config.h时候的in文件|
|automake|我们使用automake --add-missing来产生Makefile.in Makefile.am是用来生成Makefile.in的，需要你手工书写|

## 安装
yum install automake

## 实际操作流程
+ 1.编写原文件
+ 2.运行autoscan，生成configure.scan文件，重命名成configure.ac文件
+ 3.编辑configure.ac文件，修改AC_INIT、AC_OUTPUT，增加AM_INIT_AUTOMAKE([project],[version])
+ 4.运行aclocal
+ 5.运行autoconf
+ 6.运行autoheader
+ 7.创建Makefile.am文件
+ 8.运行automake --add-missing
+ 9.执行./configure
+ 10.运行make
+ 11.可以安装（make install）、卸载(make uninstall)、打包(make dist)

## configure.ac标签说明如下表
| 标签 | 功能 |
| ---- | ---- |
| AC_PREREQ	| 声明autoconf要求的版本号。 |
| AC_INIT			| 定义软件名称、版本号、联系方式 |
| AM_INIT_AUTOMAKE	| 必须要的，参数为软件名称和版本号 |
| AC_CONFIG_SCRDIR	| 宏用来侦测所指定的源码文件是否存在,来确定源码目录的有效性.。此处为当前目录下main.c |
| AC_CONFIG_HEADER	| 宏用于生成config.h文件，以便autoheader命令使用。 |
| AC_PROG_CC		| 指定编译器，默认是GCC |
| AC_CONFIG_FILES	| 生成相应的Makefile文件，不同文件夹下的Makefile通过空格分隔。例如：AC_CONFIG_FILES([Makefile,src/Makefile]) |
| AC_OUTPUT         | 用来设定 configure所要产生的文件，如果是makefile，configure会把它检查出来的结果带入makefile.in文件产生合适的makefile。 |

Automake工具会根据 configure.in 中的参量把 Makefile.am 转换成 Makefile.in文件。
最终通过Makefile.in生成Makefile文件，所以Makefile.am这个文件非常重要，定义了一些生成Makefile的规则。
1).AUTOMAKE_OPTIONS：由于GNU对自己发布的软件有严格的规范,比如必须附带许可证声明文件COPYING等，否则automake执行时会报错.automake提供了3中软件等级:foreign,gnu和gnits,供用户选择。默认级别是gnu。在本例中，使用了foreign等级,它只检测必须的文件。
2). bin_PROGRAMS =auto_test：生成的可执行文件名称，生成多个可执行文件，可以用空格隔开。
3). auto_test_SOURCES：生成可执行文件auto_test需要依赖的源文件。其中auto_test_为可执行文件的名称。

执行automake --add-missing命令。
该命令生成 Makefile.in 文件。使用选项"--add-missing"可以让Automake自动添加一些必需的脚本文件。
如果发现一些文件不存在，可以通过手工touch命令创建。

