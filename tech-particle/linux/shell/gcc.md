# GCC
GCC是GUN Compiler Collection的简称，除编译程序外，还包含其他相关工具。GCC可将高级语言编写的源代码构建成计算机直接执行的二进制代码。GCC是Linux平台下最常用的编译程序，也是Linux平台编译器的事实标准。GCC支持四十余种不同目标体系结构(如X86系列、ARM及PowerPC等)，可运行在不同的操作系统上(如Linux、Solaris及Windows等)。GCC既支持基于宿主的开发(即在A平台上编译该平台使用的程序)，也支持交叉编译(即在A平台上编译供B平台使用的程序)。

GCC最初含义为GNU C Compiler。但目前除C语言外，GCC还支持多种其他语言，如C++、Ada、Java、Objective-C、FORTRAN、Pascal、COBOL以及支持函数式编程和逻辑编程的Mercury语言等。

对于GUN编译器而言，程序的编译经历预处理(也称预编译，Preprocessing)、编译(Compilation)、汇编(Assembly) 和链接(Linking)四个阶段。在预处理阶段，输入C语言源文件(通常为*.c)，该阶段主要处理源文件中的#ifdef、 #include和#define命令，并生成中间文件*.i(实际中并不常用)；在编译阶段，输入中间文件*.i，编译后生成汇编语言文件*.s；在汇编阶段，将输入的汇编文件*.s转换成机器语言*.o；最后，在链接阶段将输入的机器代码文件*.o(与其它机器代码文件和库文件)汇集成一个二进制可执行代码文件。

实际操作上，GCC可将上述四步合并为一个步骤来执行。

选项	|	描述	|	命令示例
--------|-----------|----------		
无 | 未指定输出文件时默认输出为a.out | gcc source.c 
-c | 仅作预处理、编译和汇编并生成目标文件(与源文件同名但后缀为.o)，不进行链接 | 
-c | 将汇编输出文件source.s编译输出source.o文件 | gcc -c source.s
-c | 编译源文件source.c并将目标文件更名为target.o | gcc -c source.c -o target.o
-g |  | 
-o | 将源文件source.c链接为可执行文件exec | gcc test.c -o test
-o | 将编译输出文件source.o链接为可执行文件exec | gcc test.o -o test
-E | 仅作预处理并直接显示，不进行编译、汇编和链接 |
-E | 将源文件source.c预处理结果输出到preproc.i文件 | gcc -E source.c -o preproc.i
-E | 将source.c预处理结果重定向到preproc.txt文件 | gcc -E source.c > preproc.txt
-S | 预处理和编译到汇编语言(自动生成.s文件)，不进行汇编和链接 | 
-S | 将源文件source.c汇编成source.s文件 | gcc -S source.c
-S | 将预处理输出文件preproc.i汇编成preproc.s文件 | gcc -S preproc.i
注：”gcc -E”也可换为”cpp”(C preprocessor)


-g
产生带调试信息的编译后目标代码，供调试器(如gdb使用)

默认产生gdb所用的调试信息，可用-gformat指定要生成的调试信息格式以供其他平台其他调试器使用
[e.g.1]gcc -g source.c
将源文件编译为带调试信息的目标文件a.out
-o <OutFile>
指定输出文件的文件名

[e.g.1]gcc -o source.exe source.c

[e.g.2]gcc -o source.asm source.c

-s
	

剥除可执行文件中的符号表和重定位信息
	

用于减小可执行文件的大小

-C
	

预处理时不删除注释信息
	

配合-E选项使用

-O
	

使用编译优化级别1编译程序。O后还可附带其他的数值，表示不同的优化级别。级别越大优化效果越好，但编译时间越长
	

-O0：无优化(默认) 。

-O和-O1：优化可执行文件大小及执行时间，且不使编译时间明显增加。编译大型程序时会显著增加编译时内存的使用。

-O2：包含-O1的优化，并增加无需在目标文件大小和执行速度上进行折衷的优化。编译器不执行循环展开及函数内联，即不进行“空间换时间”的优化。该选项将增加编译时间和目标文件的执行性能。

-Os：执行所有不增加目标文件大小的-O2选项，并执行专门减小目标文件大小的优化选项。

-O3: 打开所有-O2优化选项并且增加 -finline-functions, -funswitch-loops, -fpredictive-commoning, -fgcse-after-reload和-ftree-vectorize优化选项。

[e.g.1]gcc -O1 source.c -o exec

-shared
	

生成共享目标文件
	

用于建立共享链接库

-w
	

禁止显示任何警告信息
	

 

-Wall
	

显示全部警告信息
	

 

-Wshadow
	

当局部变量遮盖另一局部变量或全局变量时发出警告
	

比较有用，建议打开。 -Wall不会打开该项

-Wpointer-arith
	

对函数指针或void *类型的指针进行算术操作时发出警告
	

比较有用，建议打开。 -Wall不会打开该项

-Werror
	

把警告当作错误
	

出现任何警告就停止

-Wunreachable-code
	

探测到永不执行的代码时发出警告
	

比较有用

-Wredundant-decls
	

同一可见域内某定义多次声明时发出警告
	

即使这些重复声明有效且毫无差别

-pedantic
	

对不符合 ANSI/ISO C标准且为标准所要求诊断之处发出警告
	

该选项可发现部分不符合ANSI/ISO C标准的代码，如C++ style comments are not allowed in ISO C90

-m
	

生成文件关联的信息，包含目标文件所依赖的所有源代码
	

[e.g.1]gcc -m source.c

与-m类似的有：

-mm忽略由#include <file>造成的依赖关系；

-md将输出导入到.d的文件里；

-mmd忽略由#include <file>造成的依赖关系，并将输出导入到.d的文件里

-v
	

显示链接过程中编译器调用的程序
	

gcc编译链接生成执行文件时，自动把该文件代码作为第一个模块链接在可执行程序中

--help
	

显示gcc帮助说明
	

 

--target-help
	

显示目标机器特定命令行选项
	

 

--version
	

显示gcc版本号和版权信息