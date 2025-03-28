# 第五章 方法
Go语言没有对象？，但是有方法，有OOP编程中的关键：封装和组合。

## 1. 方法声明
在函数声明中的函数名之前放上一个带有类型的变量（用小括号括起来），即是一个方法声明，称该函数是该类型的一个方法，称该变量为方法的接收器（receiver），OOP中调用该方法称为向一个对象发送消息。
此书建议使用类型首字母小写作为接收器名，具备方法间传递的一致性和简洁性。
调用无接收器的函数称为包级别的函数调用。
调用有接收器的函数称为接收器的类型下声明的方法的调用，该调用表达式称为选择器（选择该类型的某个函数或该struct类型的某个字段来调用）。
struct类型的方法和字段不能重名，也不能和其字段的方法重名。
Go语言中，可以为一些简单的数值、字符串、slice、map（只要不是一个指针？或者interface）来定义方法，附加行为。
方法比函数的好处：名字更简短；包外调用不用包名。

## 2. 基于指针对象的方法
当需要修改类型实例的值时，需要用指针；当这个类型比较大时，也可以用指针而不是对象来声明方法。
惯例：Go语言中，如果一个类型有方法用一个指针作为接收器，则所有的方法都必须用指针做接收器。但可以不这样？
接收器只能是类型和指向他们的指针。不能是二级指针？
如果一个类型本身是一个指针，是不能作为接收器的。
调用指针类型方法，仅需要书写变量即可，和类型方法一样调用。
Go语言会隐式调用&取址和*取值？
无法获取临时变量的内存地址，也无法用临时变量调用指针方法。右值表达式无法获得地址？

如果一个类型的所有方法都使用类型自己做接收器，则拷贝类型是安全性的；如果一个类型的所有方法使用指针做接收器，需要避免拷贝，可能会破坏其不变性。

### Nil也是一个合法的接收器类型的值
值为nil的对象也可以调用方法。
惯例：如果允许nil调用方法，在方法的前面的注释中指出nil值代表的意义。

## 3. 通过嵌入结构体来扩展类型
Go语言结构体实例可以调用嵌入它的成员的方法（省略了成员的书写），来做到类型扩展。
结构体并不是嵌入其成员的继承类（不能用它隐式转成其成员类型实例来使用），两者是"has a"关系。
内嵌把一个类型的方法引入另一个类型，编译器会生成额外的包装方法来委托内嵌成员类型已经声明的方法。
类型中内嵌的匿名成员，且是命名类型指针的，字段和方法是间接地引入该类型（也即需要通过该指针对象访问，如此，可以共享通用结构并动态地修改）。

当编译器解析一个选择器的方法时，先找其类型定义的方法，再找内嵌字段引入的方法，再找内嵌字段的内嵌字段引入的方法，依次递归向下。
如果选择器有二义性的话（同一级里有同名方法），编译器会报错。不同级呢？

对于字段特别多的复杂类型，可以把字段组合起来成小类型，结合方法设计，形成嵌入结构。
方法可以定义在在类型或指向类型的指针或类型嵌套的成员（尤其匿名成员）上。

一个小trick：关联性很强的包级变量可以用一个变量封装，这个变量是一个嵌套了这些包级变量的匿名类型的实例。这样带来更好的可读性。

## 4. 方法值和方法表达式
常用instance.fun(param)调用方法，可以分为两步：
```go
method = instance.fun
method(param)
```
其中，instance.fun称为选择器，返回一个方法值，将方法绑定到特定接收器变量的函数类型变量。这样不用指定接收器，只需传入参数即可调用方法。
`func() { instance.fun() }`可以缩写为`instance.fun`。
方法相比函数，必须指定选择器以决定接收器。

方法表达式T.f或(*T).f，返回一个函数值，该函数值把第一个参数当作接收器（不用书写选择器，而是相等价）。
方法表达式常用于根据变量来决定调用同一类型的不同具体函数时使用。

## 5. Bit数组
Go语言中，常用map[T]bool表达集合，很灵活，但用bit数组更好，在数据流分析领域bit数组表现更加理想。

set 适用场景：
1. 去重
2. 集合运算，并集、交集、差集
3. 快速查找，hashset O(1) 复杂度内查找
4. 有序存储，TreeSet 基于红黑树实现

bitmap 适用场景：
1. 大规模数据存储和查询，如用户签到状态、页面访问情况？
2. 节省空间，如1亿用户的活跃状态，空间使用远小于set
3. 高效位运算，适于支持快速位运算的数据，比如复杂的数据统计和组合查询
4. 布隆过滤器，bitmap是布隆过滤器的核心，判断元素是否存在
5. 连续性数据处理，适于连续的或近似连续的关键字序列状态标记

分布式大数据 map-reduce， 先哈希 再map

## 6. 封装
一个对象的变量或方法对调用方不可见，称为封装/信息隐藏。
Go语言只有通过首字母大小写控制可见性，适用于包级成员、struct成员，最小封装单元是包（package）。
封装三优点：1.调用方不能直接修改隐藏的变量值，包开发者可以用来做到到包的不变性；2.调用方不用关心函数（包括方法）实现细节；3.？

惯例：Go语言省略掉getter函数的Get前缀，setter不省略Set前缀（这种偏好也使用与Find、Fetch、Lookup等）。
Go不禁用包或struct的字段导出，但为了API兼容，请一开始设计时，深思熟虑。

封装是必要的，但并不总是理想的，有些场景需要暴露包内部字段。