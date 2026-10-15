动态分发
---


Go 的动态派发（dynamic dispatch）全部建立在**接口（interface）**上——Go 没有类继承体系，不存在虚函数表挂在类型上的那种"面向对象派发"，接口是唯一的多态机制。

## 1. 基本用法

```go
type Speaker interface {
    Speak() string
}

type Dog struct{ name string }
func (d Dog) Speak() string { return d.name + ": woof" }

type Cat struct{ name string }
func (c Cat) Speak() string { return c.name + ": meow" }

func main() {
    var s Speaker        // 接口变量
    s = Dog{"旺财"}        // 动态指派具体类型
    fmt.Println(s.Speak()) // 运行时决定调用哪个方法
    s = Cat{"咪咪"}
    fmt.Println(s.Speak())
}
```

`Speak()` 调哪个实现，**运行时**由接口变量里实际装的类型决定，这就是动态派发。

## 2. 关键规则：方法集决定"能不能赋进去"

```go
type I interface{ M() }

type T struct{}
func (t T)  M() {} // 值接收者
type P struct{}
func (p *P) M() {} // 指针接收者
```

- 值接收者方法：值和指针都实现接口（`T`、`*T` 都行）；
- 指针接收者方法：只有指针实现接口（`*P` 行，`P` 不行）。

```go
var i I
i = T{}   // OK
i = &T{}  // OK
i = P{}   // 编译错误：P 没有方法 M
i = &P{}  // OK
```

这是 Go 面试高频坑：**类型 T 的方法集只包含值接收者方法，*T 包含全部方法。**

## 3. 底层实现：两个字 + itab

接口变量在内存里就是两个字（64 位机 16 字节）：

```go
// runtime 里的 iface
type iface struct {
    tab  *itab   // 类型信息 + 方法地址表
    data unsafe.Pointer // 指向实际数据
}
```

- **tab**：指向 itab（interface table），里面有接口要求的**方法地址表**；
- **data**：指向具体值的副本（或原值的指针）。

调用 `s.Speak()` 的实际过程：

1. 从 `s.tab` 找到方法表；
2. 按方法在接口中的序号索引出函数指针；
3. 以 `s.data` 作为 receiver 调用。

同一接口 + 同一具体类型组合，itab 只生成一次并全局缓存，之后复用。

## 4. 静态派发 vs 动态派发

| | 静态派发 | 动态派发 |
|---|---|---|
| 形式 | 直接方法调用 | 接口方法调用 |
| 调用目标 | 编译期确定 | 运行期查 itab |
| 内联 | ✅ 可以内联 | ❌ 不能内联 |
| 灵活性 | 编译期绑定 | 运行时多态 |

```go
d := Dog{"旺财"}
d.Speak()        // 静态派发：编译期就知道地址，可能内联

var s Speaker = d
s.Speak()        // 动态派发：查 itab 间接跳转
```

## 5. 性能：有多慢？要不要在意？

动态派发的成本主要是：

- **一次间接跳转**（查 itab）；
- **阻止内联**，连带阻止一系列编译器优化；
- 接口值传参可能造成**堆分配逃逸**（data 指针指向逃逸的值）。

在普通业务代码里这通常是**纳秒级**差异，可忽略。真正要注意的是热路径（循环内高频调用、底层库），此时有两个手段：

**a) 泛型（Go 1.18+）换静态派发：**

```go
func MakeSound[S Speaker](s S) string {
    return s.Speak() // 编译期单态化，直接调用，可内联
}
```

**b) PGO 去虚化（Go 1.21+）：**  
编译器利用 profile 发现接口变量绝大多数时候是某个具体类型，会直接去虚化（devirtualize），把动态调用改回直接调用 + 类型守卫。

## 6. 一个常见误区

```go
func NewSpeaker() Speaker {
    return nil // 返回一个接口值为 nil
}
```

以及经典的 **nil interface vs 非 nil 接口装 nil 指针**：

```go
var p *Dog = nil
var s Speaker = p
fmt.Println(s == nil) // false！接口非 nil
s.Speak()             // 能调用，但方法内访问字段可能 panic
```

因为接口为 nil 要求 `(tab, data)` 两个字都是 nil；`s = (*Dog)(nil)` 时 tab 有值、data 是 nil。

## 总结

- Go 的动态派发**只通过接口**，机制是 `itab` 方法表 + 间接调用；
- 规则核心：**指针接收者只有指针实现接口**；
- 代价：不能内联 + 一次间接跳转，多数场景无所谓，热路径用**泛型**或 **PGO** 回到静态派发；
- 动态派发换来的是：调用方与实现完全解耦，这是 Go 组合式设计的根基。