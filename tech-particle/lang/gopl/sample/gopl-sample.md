# gopl-sample
## 1. hiwork
```sh
go run xxx.go
go build xxx.go
go get gopl.io/ch1/helloworld # 从xxx官网下载源代码
```

```go
// go语言惯例：在每个包的包声明之前添加注释，注释语句以//开头。
package pkg_name // 每个原文件都以一条package声明语句开始，表示该文件属于哪个包;

/*
 紧接着一系列导入import的包;缺少了必要的包或导入了不需要的包，程序都无法编译通过。
 import必须紧跟package之后，导入顺序并不重要。
 */
import (
	"fmt"
)
/*
 之后是存储在这个文件里的程序语句，
函数func、变量var、常量const、类型type的声明语句。
 */
func main() {
	/*
	 go语言 不需要在语句或者声明的结尾添加分号，除非一行上有多个语句。
	 编译器主动把特定符号后的换行符转换为分号。
	 左括号｛必须和func在同一行上，且位于末尾，不能独占一行，
	 在表达式x+y中，可在+后换行，不能在+号前换行。
	 */
	fmt.Println("hi, work")
}
```

go标准库提供100多个包，以支持输入、输出、排序及文本处理。

gofmt把代码格式化为标准格式，养成格式化代码的习惯，避免无意义的争执。
goimports，根据代码需要自动添加或者删除import声明。
godoc,查看Go语言文档

## 2. 命令行参数
```go
var i int32
/*
 Go语言中，i++是语句，不是表达式，j = i++ 与 --i都是非法的。
 */
i++ // `i++` = `i = i + 1`
```
os包，跨平台提供一些与操作系统交互的函数和变量。
os.Args[0],是命令本身的名字。其他元素os.Args[x]则是参数。
切片slice，同python切片，是一个简版动态数组。
Go语言只有"for init; cond; post {...}"循环，且左大括号必须和for同行；init和post都省略时，分号可省略；cond也省略相当于无限循环。
Go语言声明了变量必须使用，不然会导致编译错误，可以使用空标识符("_")替代声明。
空标识符用于任何语法需要，但程序逻辑不需要的时候。
隐式地而非显式地处理，可以减少代码改动？

变量声明一般使用短变量声明(s := "")和普通变量声明(var s string)。前者只能用于函数内部，不能用于包变量。
string使用+=做字符串连接，如果涉及的数据量很大，代价很高。一种简单高效的解决方案是用strings.Join(os.Args[1:], " ")。
Println格式化输出

变量在声明时直接初始化（没有显示初始化的，隐式初始化为零值）

## 3. 查找重复的行
Go语言的map类似Java的HashMap,Python中的dict，Lua中的table，数学界中的映射。
map的迭代顺序不定，是随机的，每次运行都不一样（Go有意做成这样，防止程序依赖特定顺序遍历）。

bufio包用于高效处理输入输出，Scanner是其最有用的特性之一。
fmt.Printf同C语言的printf(转化符(conversion character)：%g,%e - 浮点数; %t - 布尔值; %q - 带双引号的字符串或单引号的字符?; %v - 变量； %T - 变量类型; ),(转义符(escape  sequences))。惯例：Printf、Errorf以f结尾指format，以ln结尾指line。

Go语言的函数和包级别的变量可以以任意顺序声明，单最好遵循一定规范（我推荐先底层后高层）。

Go语言中，map实例本身是一个引用，作为参数传入函数时，被调用函数的任何修改，调用者都“可见”。

实现上，bufio.Scanner/ioutil.ReadFile/ioutil.WriteFile都使用*os.File的Read和Write函数这些低级函数，但bufio、ioutil的高级函数更易用。

## 4. GIF动画
import的包，只需使用最后那层单词表示。
目前常量的声明值，仅限于数字值、字符串、或布尔值。

复合声明表达式：`[]pack.Func{a, b, ...}`和`pack.Construct{t: t0}`,前者生成切片，后者生成struct结构体。
struct变量的内部变量可以通过“.”来访问读写。

## 5. 获取URL
Go语言强大的net包用于处理访问互联网，包括网络收发信息、建立连接、编写服务器程序，结合并发特性。
`go run fetch.go http://www.baidu.com`

## 6. 并发获取多个URL
goroutine是一种函数的并发执行方式，channel是在goroutine之间用来做参数传递。
go func表是创建一个新的goroutine，并执行func函数，main也运行在一个goroutine。

make(chan string)创建了string类型的channel，chan是Go语言内置类型，类似于管道。
当一个goroutine往channel上做send/receive操作时，这个goroutine会阻塞在调用处，知道另一个goroutine往该channel里写入/接收，之后两个goroutine才会继续执行。可以用作barrier机制。
ch <- expr, 发送；expr <- ch, 接收。

## 7. Web服务
Go语言允许一个简单的语句结果作为循环的变量声明出现在if语句的最前面，这一点有利于错误处理（限制err这个变量的作用域）。
Go语言支持匿名函数。

## 8. 其他
Go语言的switch语句不需要在每一个case后写break，默认行为是自动退出。
如果需要switch的几个case都执行同一个逻辑，需要显式地写上一个fallthrough语句来覆盖着各种默认行为，但不推荐使用。
switch还可以不带操作对象，默认用true值代替，case后可以跟着表达式（C中仅能是常量），可以直接罗列多种条件。像其他语言中的if else一样。
无tag switch同switch true等价。
switch也可以跟for和if语句一样紧跟一个简短的变量声明、一个自增表达式、赋值语句、或者一个函数调用。

Go语言支持break、continue、goto来改变程序流，慎用跳转语句。

Go语言有select语句，其case必须是一个发送/接收通信操作，select随机选择一个可运行的case。
如果没有case可运行，select会阻塞至case可运行。
select是和channel绑在一起的。

命名类型也即struct类型名称。

Go语言提供了指针（一种直接存储了变量的内存地址的数据类型），可见的内存地址（&操作符获取变量内存地址，*操作符获取指针指向的变量的值），但是Go语言没有指针运算（C语言可以指针加减法，Go语言不可以）

方法和接口：
方法是和命名类型关联的一类函数。Go语言中方法可以被关联到任意一种命名类型。
接口是一种抽象类型，可以让我们以同样的方式处理不同的固有类型，不用关心其具体实现，只关心其提供的方法。

包：Go语言提供了很多好用的包,且有一个标准库packages（<https://golang.org/pkg>和<https://godoc.org>）。

godoc工具可以在本地阅读标准库的文档。
```sh
go doc package.Func
```
给每个package、func写一个说明函数行为的注释，是好习惯，这些内容可被godoc这样的工具检测到，并可以做检索。
多行注释可以用/* */，文件开头一般都是这种形式。注释中不再嵌入注释，是好习惯。