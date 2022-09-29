FAQ
---

# Go 中 init 函数的特征?
+ 一个包下可以有多个 init 函数，每个文件也可以有多个 init 函数。
+ 多个 init 函数按照它们的文件名顺序逐个初始化。不可依赖，因为很容易被改动
+ 应用初始化时初始化工作的顺序是，从被导入的最深层包开始进行初始化，层层递出最后到 main 包。
+ 不管包被导入多少次，包内的 init 函数只会执行一次。？所以不需要sync.Once？
+ 但包级别变量的初始化先于包内 init 函数的执行。

# Go 中 uintptr 和 unsafe.Pointer 的区别？
unsafe.Pointer 是通用指针类型，它不能参与计算，任何类型的指针都可以转化成 unsafe.Pointer

unsafe.Pointer 可以转化成任何类型的指针

uintptr 可以转换为 unsafe.Pointer
unsafe.Pointer 可以转换为 uintptr。

uintptr 是指针运算的工具，但是它不能持有指针对象（意思就是它跟指针对象不能互相转换）；unsafe.Pointer 是指针对象进行运算（也就是 uintptr）的桥梁。

# Go竞态条件检查
竞态竞争 - 当两个或以上的goroutine访问相同资源时候，对资源进行读/写。

比如var a int = 0，有两个协程分别对a+=1，我们发现最后a不一定为2.这就是竞态竞争。

通常我们可以用`go run -race xx.go`来进行检测。

解决方法是，对临界区资源上锁，或者使用原子操作(atomics)，原子操作的开销小于上锁。