# gdb命令
```sh
set args

## gdb默认反汇编为att格式指令
# 查看反汇编指令格式
show disassembly-flavor
# 设置为intel
set disassembly-flavor intel
# 将内存中机器码程序以指令助记符形式显示出来
disas/disass/disassemble [函数名|起始地址[,结束地址]]

## 查看某个line的相关信息
info line [函数名|*内存地址]

ni // 单步步过
si // 单步步入

finish // 执行到返回

display /x $eax // 显示寄存器的值

x /nfu // 查看内存单元
       说明：
       n表示要显示的内存单元的个数
       f表示显示方式, 可取如下值：
       x 按十六进制格式显示变量。
       d 按十进制格式显示变量。
       u 按十进制格式显示无符号整型。
       o 按八进制格式显示变量。
       t 按二进制格式显示变量。
       a按十六进制格式显示变量。
       i 指令地址格式。
       c 按字符格式显示变量。
       s 按字符串格式显示变量。
       f 按浮点数格式显示变量。
       u表示一个地址单元的长度，其中：
       b表示单字节，
       h表示双字节，
       w表示四字节，
       g表示八字节。

p [变量名]  // 查看变量值
p &[变量名] // 查看变量地址

## 查看所有线程堆栈信息
thread apply all bt
```

有一组专用的gdb变量可以用来检查和修改计算机的通用寄存器，gdb提供了目前每一台计算机中实际使用的4个寄存器的标准名字：
1. $pc： 程序计数器
2. $fp： 帧指针（当前堆栈帧）
3. $sp： 栈指针
4. $ps： 处理器状态

寄存器：$r0~$r15~$rx $rl

```sh
  backtrace          # 显示程序中的当前位置和表示如何到达当前位置的栈跟踪（同义词：where）     
  breakpoint         # 在程序中设置一个断点     
  cd                 # 改变当前工作目录     
  clear              # 删除刚才停止处的断点     
  commands           # 命中断点时，列出将要执行的命令     （#add相当于vs的when hit）
  continue           # 从断点开始继续执行     
  delete             # 删除一个断点或监测点；也可与其他命令一起使用     
  display            # 程序停止时显示变量和表达时     
  down               # 下移栈帧，使得另一个函数成为当前函数     
  frame              # 选择下一条continue命令的帧     
  info               # 显示与该程序有关的各种信息     
  jump               # 在源程序中的另一点开始运行     
  kill               # 异常终止在gdb   控制下运行的程序     
  list               # 列出相应于正在执行的程序的原文件内容     
  next               # 执行下一个源程序行，从而执行其整体中的一个函数     
  print              # 显示变量或表达式的值     
  pwd                # 显示当前工作目录     
  ptype              # 显示一个数据结构（如一个结构或C++类）的内容     
  quit               # 退出gdb     
  reverse-search     # 在源文件中反向搜索正规表达式     
  run                # 执行该程序     
  search             # 在源文件中搜索正规表达式     
  set   variable     # 给变量赋值     
  signal             # 将一个信号发送到正在运行的进程     
  step               # 执行下一个源程序行，必要时进入下一个函数     
  undisplay          # display命令的反命令，不要显示表达式     
  until              # 结束当前循环     
  up                 # 上移栈帧，使另一函数成为当前函数     
  watch              # 在程序中设置一个监测点（即数据断点）     
  whatis             # 显示变量或函数类型 
```

## attach
```sh
gdb
(gdb)attach xxx
(gdb)stop # 暂停子进程
(gdb)break 137            #
(gdb)break test.cpp:137   #
(gdb)break Test::foo      #
(gdb)break # 例程断点
(gdb) # 动态库断点？
(gdb)
(gdb)info break # 查看断点
(gdb)continue   # 继续
# ... 触发断点
(gdb)step # 单步
(gdb)n
```

## list
```sh
(gdb)list [bgn],[end] # bgn->end line num code
(gdb)list
(gdb)list n
(gdb)list [func]
(gdb) # 查看文件?
```

## run
```sh
(gdb)set args -x
(gdb)show args
(gdb)run
```

## print
```
(gdb)print p
(gdb)whatis p
```

## file
```
(gdb)search text
(gdb)file [exe file]
(gdb)load [so file]
(gdb)dir [src code dir]
(gdb)sharedlibrary [so dir]
```