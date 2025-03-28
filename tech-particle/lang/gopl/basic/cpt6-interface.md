# 第六章 接口类型
Go语言接口类型是对其他类型行为的抽象和概括!接口不与特定实现细节绑定在一起。
接口类型让函数更加灵活，更具适应能力。

Go语言接口概念的独特之处在于满足隐式实现，也即不需绑定具体类型到接口类型上，可以创建一些新接口类型且不用修改具体类型的定义。
接口类型的隐式实现，尤其适用于来自第三方开发的包中设计的具体类型。
接口只需要简单拥有一些方法就可以了？

个人理解接口类型也很像泛型？

## 1. 接口约定
一段代码中，一个具体类型可以使用另一个满足相同接口的类型来进行替换，称作这段代码里这个类型具有可替换性（LSP里氏替换）。
Go语言中io包的Writer就是接口类型，fmt.Fprintf接收一个io.Writer类型的参数。
fmt包中的Stringer都是接口类型，它有一个返回string的String()方法，用于支持传统符号打印。

## 2. 接口类型
接口类型表述的是一系列方法的集合，实现了这些方法的具体类型是该接口类型的实例。
惯例：Go语言中一些基础接口类型是单方法接口类型，方法名称使用动词，类型名称使用对应的名词，如read/reader。
接口类型可以通过组合已有接口来定义，与内嵌相似，成为接口内嵌（当然也可以使用方法，是等价的）。

## 3. 实现接口的条件
如果一个类型实现了一个接口的所有方法，则这个类型就实现了这个接口。
开发者把一个或多个具体类型描述成一个特定接口类型的过程就是抽象。
可以把实现了接口的具体类型（或另一个接口类型）的右值表达式赋值给接口类型的变量实例。
注意：类型的指针上定义的方法不是类型上定义的方法，在赋值给接口类型的变量时是有区别的？

`godoc -anlysis=typetool` 展示每个类型的方法和具体类型和接口之间的关系。

接口类型用于封装和隐藏具体类型的其他方法和值，只暴露出其定义的方法。

接口类型interface{}称为空接口类型，在Go语言有特殊含义，该接口类型对实现它的类型没有要求，可以把任何值赋值给它。
空接口类型没有成员或方法，不能直接操作空接口类型的变量。

是否实现接口仅依赖于判断两个类的方法，没必要书写两个类的关系，或文档、或断言。编译时会断言是否实现。

## 4. flag.Value接口
Go语言标准中的flag.Value接口类型用于帮助命令行标记定义新的符号。
自定义flag.Value具体类型

## 5. 接口值
接口值有两部分组成，具体的类型和该类型的值，又称为接口的动态类型和动态值。
Go语言是静态类型的语言（类型是编译期间的概念；类型不是值；）。
提供每个类型信息的值（如类型的名称和方法）称为类型描述符（接口的动态类型即是）。
一个接口的零值是它的类型和值的部分都是nil，可用nil判断接口是否为空，调用空接口值的任意方法都会产生panic。
具体类型的值可以隐式或显式转为接口类型。
编译期间，接口值的动态类型是未知的，运行时会动态分配的，接口值的类型调用是间接调用的，接受者是接口动态值的拷贝。
接口的动态值是指向具体类型值的指针。？

大部分接口值可用==或=？比较。接口相等意味着都是nil或接口动态类型和动态值的每一项都相等。
接口值可以做map的键值或switch的操作数。
有些接口值不可比较，如切片类型的比较会引发panic。
接口值比较特别，对于接口值比较或包含接口的聚合类型比较时，是有潜在panic风险的。

fmt包%T打印接口值的动态类型，使用反射来获取接口动态类型的名称。

### 警告：一个包含nil指针的接口不是nil接口
包含nil指针的接口变量和nil做!=比较的结果为true。
遇到不能把nil做有效接受者的具体类型就容易发生问题，如*bytes.Buffer。
Go语言很多是遵从契约编程，常常隐含nil值非法的先决条件，需要调用者保证不传入nil值。

## 6. sort.Interface接口
## 7. http.Handler接口
## 8. error接口
## 9. 类型断言
## 10. 基于类型断言识别错误类别
## 11. 通过类型断言查询接口
## 12. 类型分支
## 13. 示例：基于标记的XML解码