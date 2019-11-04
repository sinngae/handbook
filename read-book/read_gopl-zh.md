# 第一章 入门
## 1.1 hello world
```
go run xxx.go
go build xxx.go
go get gopl.io/ch1/helloworld 从xxx官网下载源代码
```
每个原文件都以一条package声明语句开始，表示该文件属于哪个包;
紧接着一系列导入import的包;
缺少了必要的包或导入了不需要的包，程序都无法编译通过。
import必须紧跟package之后，导入顺序并不重要。
之后是存储在这个文件里的程序语句，
函数func、变量var、常量const、类型type的声明语句。
go语言惯例：在每个包的包声明之前添加注释，注释语句以//开头。

go语言 不需要在语句或者声明的结尾添加分号，除非一行上有多个语句。
编译器主动把特定符号后的换行符转换为分号。
左括号｛必须和func在同一行上，且位于末尾，不能独占一行，
在表达式x+y中，可在+后换行，不能在+号前换行。

go标准库提供100多个包，以支持输入、输出、排序及文本处理。

gofmt把代码格式化为标准格式，养成格式化代码的习惯，避免无意义的争执。
goimports，根据代码需要自动添加或者删除import声明。
godoc,查看Go语言文档

## 1.2 命令行参数
os包，跨平台提供一些与操作系统交互的函数和变量。
os.Args[0],是命令本身的名字。其他元素os.Args[x]则是参数。
切片slice，同python切片，是一个简版动态数组。
Go语言中，i++是语句，不是表达式，j = i++ 与 --i都是非法的。
Go语言只有"for init; cond; post {...}"循环，且左大括号必须和for同行；init和post都省略时，分号可省略；cond也省略相当于无限循环。
Go语言声明了变量必须使用，不然会导致编译错误，可以使用空标识符("_")替代声明。
空标识符用于任何语法需要，但程序逻辑不需要的时候。
隐式地而非显式地处理，可以减少代码改动？

变量声明一般使用短变量声明(s := "")和普通变量声明(var s string)。前者只能用于函数内部，不能用于包变量。
string使用+=做字符串连接，如果涉及的数据量很大，代价很高。一种简单高效的解决方案是用strings.Join(os.Args[1:], " ")。
Println格式化输出

变量在声明时直接初始化（没有显示初始化的，隐式初始化为零值）

## 1.3 查找重复的行
Go语言的map类似Java的HashMap,Python中的dict，Lua中的table，数学界中的映射。
map的迭代顺序不定，是随机的，每次运行都不一样（Go有意做成这样，防止程序依赖特定顺序遍历）。

bufio包用于高效处理输入输出，Scanner是其最有用的特性之一。
fmt.Printf同C语言的printf(转化符(conversion character)：%g,%e - 浮点数; %t - 布尔值; %q - 带双引号的字符串或单引号的字符?; %v - 变量； %T - 变量类型; ),(转义符(escape  sequences))。惯例：Printf、Errorf以f结尾指format，以ln结尾指line。

Go语言的函数和包级别的变量可以以任意顺序声明，单最好遵循一定规范（我推荐先底层后高层）。

Go语言中，map实例本身是一个引用，作为参数传入函数时，被调用函数的任何修改，调用者都“可见”。

实现上，bufio.Scanner/ioutil.ReadFile/ioutil.WriteFile都使用*os.File的Read和Write函数这些低级函数，但bufio、ioutil的高级函数更易用。

## 1.4 GIF动画

