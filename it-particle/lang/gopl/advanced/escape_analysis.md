# interface导致的逃逸

## 什么是逃逸分析
逃逸分析是编译器用来判定代码中的变量是放在协程的栈上，还是放置在堆上：
1.是否有超出局部(对象定义所在的不会发生出栈的一段代码块，一般是一个函数)的调用而导致，是则被放到堆上；
2.对象是否太大（即使没有被引用），无法放在栈区，是则放到堆上；
否则，优先放到栈上

编程应该尽量避免逃逸，有下列好处:
1.减少gc的压力 - 不逃逸的对象会分配在栈上，当函数返回时回收资源，不需要gc标记清除；
2.逃逸分析完后可以确定哪些变量可以分配在栈上，栈的分配比堆快，系统开销少；
3.减少动态分配所造成的内存碎片；
> 堆是进程用于存储值、全局变量、内存占用大的局部变量。进程从操作系统获取堆内存，需要自己管理堆内存的使用，自己解析成特定的结构，标记使用的大小。

>> 有些编程语言的堆内存是开发者来管理，有些编程语言是编译器设定了GC机制。
>> GC运行占用不可忽略的CPU资源，且可能会产生微秒级的“stop the world”效果。
>> GC的好处是开发者不用关注堆内存的管理问题，这样就降低了复杂度和出错几率。
>> Golang程序使用三色标记法来进行堆内存的垃圾回收，实现GC。
>> 堆中不再被栈上指针直接或间接引用的每个地址都需要清理。需要检查和清理的地址越多，GC每次运行时必须执行的工作就越多。
>> GC算法也在一直优化在堆内存管理和运行速度之间寻求平衡。


> 栈是线程或协程私有的一块内存区域，用来存自己的局部变量（尽量占用少的内存）、跳转地址、返回值等数据。栈大小在编译时已经确定，其结构解析和寻址方式运行时决定，寻址比堆快。栈上数据增长太大就会发生"stack overflow"。
>> C/C++程序栈上的空间被释放后仍能访问，常常引发不可预测的行为(C++程序员称为飞了的现象)（已经释放的栈内存可能被覆盖为函数返回地址等变量，对其的读写常常不是期望且事后很难判断影响的行为）；有GC的Go语言程序编译时，会做逃逸分析，把逃逸的变量放到堆上，避免这种问题。
>> Go 协程栈初始化大小时8KB，可以动态（不足时）翻倍扩展或（四分之一时）折半收缩。这样协程栈不会轻易溢出，但深度递归调用、局部变量占用过大、超出系统可用资源限制 还是会导致栈溢出。
>> Go 1.3之前采用分段栈机制，物理内存中不连续；之后改用连续栈机制，物理内存连续，解决了热分裂问题（在一个循环中调用函数 恰逢出发栈扩展，就会反复扩展、收缩 造成巨大开销，称为热分裂问题）。

## 如何进行逃逸分析
可以使用go build来进行逃逸分析.
```shell script
# 查看详细的逃逸分析过程
#   编译参数介绍(-gcflags):
#   -N: 禁止编译优化
#   -l: 禁止内联(可以有效减少程序大小)
#   -m: 逃逸分析(最多可重复四次)
#   -benchmem: 压测时打印内存分配统计堆
go build -gcflags '-m -l' main.go

# 反编译命令查看
go tool compile -S main.go
```

## 逃逸分析示例
### 1.示例-参数泄露
测试代码
```go
// testaoyi.go
package main
import (
    _ "fmt"
)
// 定义一个简单的结构体
type user struct {
    name    string
    age     int
    webSite string
}
// 获取用户信息
func GetUserInfo(u *user) (*user) {
    return u
}
// 获取用户名称
func GetName(u *user) (string) {
    return u.name
}

func main() {
    // 初始化user结构体的指针对象
    user := &user{"BGBiao",18,"https://bgbiao.top"}
    GetUserInfo(user)
    GetName(user)
}
```
使用逃逸分析来进行内存分析
```shell
go build -gcflags '-m -l' taoyi.go
# ./taoyi.go:21:18: leaking param: u to result ~r1 level=0
# ./taoyi.go:25:14: leaking param: u to result ~r1 level=1
# ./taoyi.go:31:31: main &user literal does not escape
```
由上述输出的leaking param可以看到，在GetUserInfo和GetName函数中的指针变量u是一个泄露参数，在两个函数中均没有对u进行变量操作，就直接返回了变量内容，因此最后的该变量user并没有发生逃逸，&user对象还是作用在了main()函数中。

### 2.示例-未知类型
修改一下：
```go
func main() {
    user := &user{"BGBiao",18,"https://bgbiao.top"}
    fmt.Println(GetUserInfo(user))
    fmt.Println(GetName(user))
}
```
再次进行逃逸分析:
```shell script
go build -gcflags '-m -m  -l' taoyi.go
# ./taoyi.go:21:18: leaking param: u to result ~r1 level=0
# ./taoyi.go:25:14: leaking param: u to result ~r1 level=1
# ./taoyi.go:31:31: &user literal escapes to heap
# ./taoyi.go:32:16: main ... argument does not escape
# ./taoyi.go:32:28: GetUserInfo(user) escapes to heap
# ./taoyi.go:33:16: main ... argument does not escape
# ./taoyi.go:33:24: GetName(user) escapes to heap
```
由上可以发现我们的指针对象&user在该程序中发生了逃逸，具体是在GetUserInfo(user)和GetName(user)发生了逃逸.

为什么加了个fmt.Println之后对象就发生了逃逸呢?

主要是因为fmt.Println的原因:
```go
func Println(a ...interface{}) (n int, err error)
```
fmt.Println()函数入参为interface{}类型，interface{}类型一般情况下底层会进行reflect，而使用的reflect.TypeOf(arg).Kind()获取接口类型对象的底层数据类型时发生了堆逃逸，最终就会反映为当入参是空接口类型时发生了逃逸。

### 3.示例-指针
再改代码
```go
// 返回结构体对象的指针，此时就会产生逃逸
func GetUserInfo(u user) (*user) {
    return &u
}

func main() {
    user := user{"BGBiao",18,"https://bgbiao.top"}
    GetUserInfo(user)
}
```
逃逸分析:
```shell script
go build -gcflags '-m -l' taoyi.go
# ./taoyi.go:21:18: moved to heap: u

# 查看汇编代码(可以看到有个CALL	runtime.newobject(SB)的系统调用)
go tool compile -S taoyi.go | grep taoyi.go:21
	0x0000 00000 (taoyi.go:21)	TEXT	"".GetUserInfo(SB), ABIInternal, $40-48
	0x0000 00000 (taoyi.go:21)	MOVQ	(TLS), CX
	0x0009 00009 (taoyi.go:21)	CMPQ	SP, 16(CX)
	0x000d 00013 (taoyi.go:21)	JLS	147
	0x0013 00019 (taoyi.go:21)	SUBQ	$40, SP
	0x0017 00023 (taoyi.go:21)	MOVQ	BP, 32(SP)
	0x001c 00028 (taoyi.go:21)	LEAQ	32(SP), BP
	0x0021 00033 (taoyi.go:21)	FUNCDATA	$0, gclocals·fb57040982f53920ad6a8ad662a1594f(SB)
	0x0021 00033 (taoyi.go:21)	FUNCDATA	$1, gclocals·263043c8f03e3241528dfae4e2812ef4(SB)
	0x0021 00033 (taoyi.go:21)	FUNCDATA	$2, gclocals·9fb7f0986f647f17cb53dda1484e0f7a(SB)
	0x0021 00033 (taoyi.go:21)	PCDATA	$0, $1
	0x0021 00033 (taoyi.go:21)	PCDATA	$1, $0
	0x0021 00033 (taoyi.go:21)	LEAQ	type."".user(SB), AX
	0x0028 00040 (taoyi.go:21)	PCDATA	$0, $0
	0x0028 00040 (taoyi.go:21)	MOVQ	AX, (SP)
	0x002c 00044 (taoyi.go:21)	CALL	runtime.newobject(SB)
	0x0031 00049 (taoyi.go:21)	PCDATA	$0, $1
	0x0031 00049 (taoyi.go:21)	MOVQ	8(SP), AX
	0x0036 00054 (taoyi.go:21)	PCDATA	$0, $-2
	0x0036 00054 (taoyi.go:21)	PCDATA	$1, $-2
	0x0036 00054 (taoyi.go:21)	CMPL	runtime.writeBarrier(SB), $0
	0x003d 00061 (taoyi.go:21)	JNE	104
	0x003f 00063 (taoyi.go:21)	MOVQ	"".u+48(SP), CX
	0x0044 00068 (taoyi.go:21)	MOVQ	CX, (AX)
	0x0047 00071 (taoyi.go:21)	MOVUPS	"".u+56(SP), X0
	0x004c 00076 (taoyi.go:21)	MOVUPS	X0, 8(AX)
	0x0050 00080 (taoyi.go:21)	MOVUPS	"".u+72(SP), X0
	0x0055 00085 (taoyi.go:21)	MOVUPS	X0, 24(AX)
	0x0068 00104 (taoyi.go:21)	PCDATA	$0, $-2
	0x0068 00104 (taoyi.go:21)	PCDATA	$1, $-2
	0x0068 00104 (taoyi.go:21)	MOVQ	AX, "".&u+24(SP)
	0x006d 00109 (taoyi.go:21)	LEAQ	type."".user(SB), CX
	0x0074 00116 (taoyi.go:21)	MOVQ	CX, (SP)
	0x0078 00120 (taoyi.go:21)	MOVQ	AX, 8(SP)
	0x007d 00125 (taoyi.go:21)	LEAQ	"".u+48(SP), CX
	0x0082 00130 (taoyi.go:21)	MOVQ	CX, 16(SP)
	0x0087 00135 (taoyi.go:21)	CALL	runtime.typedmemmove(SB)
	0x0091 00145 (taoyi.go:21)	JMP	89
	0x0093 00147 (taoyi.go:21)	NOP
	0x0093 00147 (taoyi.go:21)	PCDATA	$1, $-1
	0x0093 00147 (taoyi.go:21)	PCDATA	$0, $-1
	0x0093 00147 (taoyi.go:21)	CALL	runtime.morestack_noctxt(SB)
	0x0098 00152 (taoyi.go:21)	JMP	0
```
由以上输出可以看到在GetUserInfo(u user)函数中的对象u已经被移到堆上了，这是因为该函数返回的是指针对象，引用对象被返回到方法之外了(此时该引用对象可以在外部被调用和修改)，因此编译器会把该对象分配到堆上(否则方法结束后，局部变量被回收岂不是很惨)。

### 4.示例-其他
```go
// taoyi-2.go
package main

func main() {
    name := new(string)
    *name = "BGBiao"
}
```
``` shell
$ go build -gcflags '-m -l' taoyi-2.go
#./taoyi-demo.go:13:16: main new(string) does not escape
```
上面的示例告诉我们虽然*name是一个指针类型，但是并未发生逃逸，这是因为该引用类型未被外部使用.

但是又如第二个示例中所说，如果我们在上面的示例中增加fmt.Println(name)后，会发现该实例又会出现逃逸.

注意:虽然当使用fmt.Println的时候又会出现逃逸，但是当使用fmt.Println(*name)和fmt.Println(name)，也是不同的。
```go
$ cat demo1.go
package main
import ("fmt")
func main() {
    name := new(string)
    *name = "BGBiao"
    fmt.Println(*name)
}

$ go build -gcflags '-m -m  -l' taoyi-demo.go
./taoyi-demo.go:13:16: main new(string) does not escape
./taoyi-demo.go:15:16: main ... argument does not escape
./taoyi-demo.go:15:17: *name escapes to heap
```
由上述输出可看到，当使用引用类型来获取底层的值时，在fmt.Println的入参处*name发生了逃逸.
```
$ cat demo2.go
package main
import ("fmt")
func main() {
    name := new(string)
    *name = "BGBiao"
    fmt.Println(name)
}

$ go build -gcflags '-m -m  -l' taoyi-demo.go
./taoyi-demo.go:13:16: new(string) escapes to heap
./taoyi-demo.go:15:16: main ... argument does not escape
./taoyi-demo.go:15:16: name escapes to heap
```
而这次我们使用fmt.Println(name)来输出底层值，就会发现变量name在初始化的时候就会出现逃逸new(string)，



## 实际使用过程中要注意:
静态分配到栈上，寻址比分配到堆上的要快，但一般不需过度考虑；*分配到堆还是栈对顶层应用应是透明的，不需过度考虑；*
每个 Go 版本的逃逸分析都会有所优化；
直接通过go build -gcflags '-m -l' 就可以看到逃逸分析的过程和结果；
一般，基本简单类型只做读取时，使用值传递比指针传递更合适；
map & slice 初始化时，预估容量，避免由扩展导致的内存分配。但是如果太大（10000）也会逃逸，因为栈的空间是有限的；

### 函数传递指针真的比传值效率高吗？

传递指针可以减少值拷贝以提高效率，但是如果拷贝的数据量小且量大，逃逸的指针会增加GC的负担，所以传递指针不一定是高效的。

### 内存碎片化问题
程序运行使用堆内存，先从操作系统申请一块比较大的内存，然后按需分成小块使用，通过链表链接来管理。每次程序申请堆内存，会先从链表上面遍历每一小块，找到大小符合的就返回其地址，没有合适的就从操作系统再申请。

如果申请内存较多次数后，出现的很多小块内存没有被使用到的现象，称为内存碎片化（内存的占用和未占用变得无序，本质上是由于计算机组成的寻址方式必须按字节整数倍地址寻址造成的）。
碎片化严重的，大量的内存碎片没有充分使用，但操作系统或程序的堆内存链表却不再能分配更大的内存（前者称为外部内存碎片化，后者称为内部内存碎片化）。

Go 项目一般用 c := make([]int, 0, l) 来申请内存，长度固定的且小的，会优先申请到栈上；长度不固定的或大的，会申请到堆上面.

### Go 使用的垃圾回收算法是『标记——清除』.



