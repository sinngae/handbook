# 核心一句话
`builtin.go` 里面的 `type Type int` **不是真正参与编译运行的代码！只是给 godoc 文档用的占位标记**，不是运行时类型，也不是说 int 类型本身能存任意类型的值。

> 源码位置：`src/builtin/builtin.go`
> ```go
> // Type is here for the purposes of documentation only.
> // It is a stand-in for any Go type, but represents the same type for any given function invocation.
> type Type int
> ```
> 翻译：**Type 仅用于文档。它是任意Go类型的占位替身，在单次函数调用里代表同一个类型。**

## 1. builtin包的特殊性质
builtin.go 只是文档文件，**不会编译进程序**。Go编译器内置预定义标识符（`any`、`bool`、`int`、`Type`、`IntegerType`）并不是在这里定义实现的，这个文件只是给文档生成工具 godoc 看的注释载体。

- `type Type int`：语法上写成派生int，纯粹是**语法占位**，让godoc能渲染文档；
- 它不是普通代码，**不能在你的业务代码 import builtin 然后拿来当变量类型使用**；
- 它的含义：**在泛型函数文档中，用来代表「某一个任意Go类型」**（不是同时代表所有类型）。

举例：内置函数 `len(v Type) int` 的文档
```go
func len(v Type) int
```
文档上写参数是`Type`，意思是：**这个参数可以是任意一个合法Go类型（slice、map、string、array、channel），但一次调用只能固定为其中某一类**。
不是说它底层是int，就可以放任意类型的值。

配套还有：
```go
type IntegerType int
```
注释：stand-in for any integer type（所有整数类型的文档占位替身）。

## 2. 区分容易混淆的几个Type（非常关键）
1. ✅ `builtin.Type`：文档占位，`type Type int`，**只用于godoc，不参与编译**
2. ✅ `reflect.Type`：**接口类型**，`type Type interface{...}`，运行时反射，代表一个Go类型的元信息，不是值本身
3. ✅ `go/types.Type`：静态类型检查包的接口，做源码静态分析（go vet、编译器）

> 很多人踩坑：把 `builtin.Type` 当成运行时的类型，以为`type Type int`这个int派生类型可以存任意类型的值，**完全不是**。

## 3. 和 any / interface{} 的区别
- `any = interface{}`：**真正的空接口，运行时可以承载任意类型的值**，是语言原生特性；
- `builtin.Type`：文档层面的“类型占位符”，描述**类型本身**，不是承载**值**。
    - `any`：放**值**（比如 `var x any = 123`）
    - `builtin.Type`：文档里描述**类型T**，用于描述内置函数/泛型的类型参数

## 4. 举个例子看懂文档里的Type
```go
func append(slice []Type, elems ...Type) []Type
```
builtin文档里的append签名，这里的`Type`就是这个占位：
> slice是某个类型T的切片，追加的元素也是T，返回同样T的切片；
> Type代表**同一个任意类型T**，不是混合多种不同类型。

## 总结
1. `type Type int` 在 `builtin.go`，**只是文档占位，不会编译进二进制**；
2. 写成`int`只是为了满足Go语法，没有实际int语义；
3. 作用：在内置函数文档中，代表**任意单个Go类型**；
4. 不要和 `reflect.Type`（反射接口）、`any`（空接口，存任意值）搞混。

如果你想，我可以对比内置函数文档里的 `Type`、`IntegerType`、`any` 三者的文档示例。