Closure
---
# 数学上的闭包
闭包是离散数学中对关系进行扩展的核心概念，指通过添加最少数量的有序对，使原关系具备自反性、对称性或传递性而形成的新集合。
在拓扑学中，闭包描述集合与其极限点构成的闭集，满足Cl(S)=S时称为闭集。

在拓扑学中，集合 S 是闭集当且仅当 Cl(S)=S（这里的 Cl 即 closure，闭包）。

在离散数学中，设R是集合A上的关系，P是给定的某种性质（如自反、对称、传递），满足下列所有条件的关系R1称为R关于P的闭包：R⊆R1；R1满足性质P；如果存在集合A上的关系R‘，R’满足性质P并包含R，则R1⊆R‘。自反闭包r(R) = R∪I_A（I_A是A上的恒等关系），对称闭包s(R) = R∪R⁻¹（R⁻¹是R的逆关系），传递闭包t(R) = R*（连通关系），对于有限集合A（|A|=n），t(R) = ⋃_{i=1}^{n} Rⁱ [1]。

# 编程语言中的闭包
闭包（Closure） 是编程语言中的一个核心概念，指的是一个函数与其引用的外部变量绑定后形成的整体。这个函数即使离开了创建它的作用域，依然能"记住"并访问那些外部变量。

## 一、一句话定义
闭包 = 函数 + 函数创建时能访问到的外部变量（自由变量）

(自由变量，既不是函数参数，也不是函数内部的局部变量，是指通过语法分析出的函数的外部变量。如果闭包逃逸出函数，则自动分配到堆上。其生命周期，由GC管理，与闭包函数对象一致。)

关键特征：
+ 函数内部引用了不属于自己参数或局部变量的外部变量
+ 这个函数被传递到别处执行（作为返回值、回调、goroutine 等）
+ 外部变量被函数"捕获"并持续存活，不会被垃圾回收

## 二、为什么叫"闭包"
想象一个"封闭包裹"：函数被创建时，把它周围的环境（变量）一起打包带走。无论这个函数之后在哪里运行，它都能打开包裹，使用里面的东西。

## 三、典型示例
JavaScript（最经典的场景）
```JavaScript
function makeCounter() {
    let count = 0;  // 外部变量
    return function() {  // 返回的匿名函数 + count = 闭包
        count++;
        return count;
    };
}

const counter = makeCounter();
console.log(counter()); // 1
console.log(counter()); // 2
console.log(counter()); // 3

// makeCounter 已经执行完毕，count 本应销毁，
// 但因为被内部函数引用，所以被闭包保留了下来。
```

Python
```Python
def make_multiplier(n):
    def multiplier(x):  # multiplier 捕获了外部变量 n
        return x * n
    return multiplier

double = make_multiplier(2)
triple = make_multiplier(3)

print(double(5))  # 10
print(triple(5))  # 15
```

Go
```go
func makeAdder(base int) func(int) int {
    return func(x int) int {
        return base + x  // base 被闭包捕获
    }
}

addFive := makeAdder(5)
fmt.Println(addFive(3)) // 8
```

## 四、闭包解决了什么问题？
- 问题	闭包如何解决的
+ 数据隐藏	类似私有变量，外部无法直接访问 count，只能通过返回的函数操作
+ 状态保持	函数执行完毕后，内部状态不丢失
+ 工厂模式	用同一个函数模板生成行为不同但结构相似的函数
+ 回调与异步	回调函数携带上下文数据执行

## 五、常见陷阱：循环中的闭包
这是面试高频题：
```JavaScript
for (var i = 0; i < 3; i++) {
    setTimeout(function() {
        console.log(i);  // 预期 0,1,2？实际全是 3
    }, 100);
}
```
原因：var 是函数作用域，三个回调共享同一个 i。循环结束时 i 已经是 3，所以三个回调都打印 3。

修复（用 let 创建块级作用域）：
```JavaScript
for (let i = 0; i < 3; i++) {  // let 每次迭代创建新的绑定
    setTimeout(function() {
        console.log(i);  // 0, 1, 2
    }, 100);
}
```

## 六、闭包 vs 匿名函数 vs 类
- 概念	区别
+ 匿名函数	只是没有名字的函数。如果不引用外部变量，就不是闭包
+ 闭包	一定是引用了外部自由变量的函数
+ 类/对象	闭包可以实现类似私有字段的效果，但类更适合复杂继承体系；闭包更轻量

## 七、性能与内存考量
内存泄漏风险：闭包持有的外部变量不会被释放。如果闭包长期存活（如全局事件监听器），而引用的变量又很大，可能造成内存泄漏。
捕获方式：不同语言实现不同。Python 在闭包中修改外部变量需显式声明 nonlocal（或 global），否则视为新建局部变量：
```Python
def make_counter():
    count = 0
    def increment():
        nonlocal count  # 必须声明，否则报错
        count += 1
        return count
    return increment
```

## 总结
闭包的本质是"函数携带其词法环境远行"。
它让函数超越了"代码块"的局限，成为可以封装状态、延迟执行、跨作用域通信的强大工具。理解闭包，就理解了函数式编程中"函数是一等公民"的深层含义。

# 闭包的自由变量的生命周期
自由变量（Free Variable） 的生命周期，是理解闭包内存行为的核心。简单来说：
自由变量的生命周期被闭包"延长"了——只要闭包还存在，它引用的自由变量就不会被销毁。

## 一、什么是自由变量？
自由变量指的是在函数内部被引用，但既不是函数参数、也不是函数局部变量的外部变量。
```JavaScript
function outer() {
    let x = 10;        // ← 自由变量（对外层而言是局部变量，对内部函数而言是外部变量）
    return function() {
        return x + 1;  // x 在这里被引用，但 x 不是这个内部函数的参数或局部变量
    };
}
```

## 二、正常情况 vs 闭包情况下的生命周期
正常情况（无闭包）
```JavaScript
function normal() {
    let x = 10;
    console.log(x);  // 使用完即释放
}
normal();
// x 在这里就应该被垃圾回收了
```
函数执行完毕，局部变量 x 的作用域结束，内存释放。

闭包情况
```JavaScript
function createClosure() {
    let x = 10;      // 自由变量
    return function() {
        return x;
    };
}

const closure = createClosure();
// createClosure 已经执行完毕，按常理 x 应该被销毁
// 但因为 closure 引用了 x，x 继续存活
console.log(closure()); // 10
```
关键变化：x 的生命周期从"函数执行期间"被延长到了"闭包被销毁之前"。

## 三、不同语言中的具体机制
JavaScript（V8 引擎）  
V8 使用逃逸分析（Escape Analysis） 和隐藏类（Hidden Class） 优化闭包内存：
+ 被闭包引用的变量会被移到堆（Heap） 上，而非栈
+ 这些变量与闭包对象形成引用链
+ 只有当闭包本身不再被引用时，整个闭包链才能被垃圾回收
```JavaScript
function heavyClosure() {
    let hugeArray = new Array(1000000).fill('x');  // 大对象
    let smallVar = 1;
    
    return function() {
        return smallVar;  // 只引用了 smallVar
    };
}

const fn = heavyClosure();
// 注意：在 V8 中，hugeArray 理论上未被 fn 引用，应该被回收
// 但早期/简单实现的引擎可能保留整个词法环境对象，导致 hugeArray 泄漏
```
现代 JS 引擎优化：只捕获实际引用的变量（精细化闭包），而非整个外层作用域。但开发者不应依赖此优化。

Python  
Python 的闭包通过 __closure__ 属性暴露自由变量：
```Python
def outer():
    x = 10
    def inner():
        return x
    return inner

f = outer()
print(f.__closure__)           # (<cell at 0x...: int object at 0x...>,)
print(f.__closure__[0].cell_contents)  # 10
```
Python 使用 cell 对象 存储自由变量
cell 是一个引用容器，被闭包函数的内部 __closure__ 元组持有
当 f 被垃圾回收时，cell 的引用计数归零，x 才被释放

Go  
Go 的闭包通过堆分配实现：
```go
func makeClosure() func() int {
    x := 10        // 原本应在栈上，但编译器发现它被返回的闭包引用
    return func() int {
        return x   // x 被"逃逸"到堆上
    }
}
```
Go 编译器进行逃逸分析：如果变量被闭包引用且闭包逃逸出函数，变量自动分配到堆
生命周期由垃圾回收器管理，与闭包函数对象一致

## 四、生命周期延长的实际影响
1. 内存泄漏风险
这是闭包最常见的问题：
```JavaScript
// 危险：全局事件监听器持有大对象
function setupHandler() {
    let hugeData = fetchHugeData();  // 100MB 数据
    
    document.getElementById('btn').addEventListener('click', function() {
        console.log('clicked');  // 闭包没有直接引用 hugeData
        // 但某些旧引擎可能保留整个词法环境
    });
}
```

更隐蔽的泄漏：
```JavaScript
function leaky() {
    let data = new Array(1000000).fill(0);
    
    return {
        getData: function() { return data; },  // 引用 data
        getLength: function() { return data.length; }  // 也引用 data
    };
}

const api = leaky();
// 即使我们只调用 api.getLength()，data 的整个数组都活在内存中
// 因为闭包持有的是整个 data 引用，不是 data.length 的副本
```

2. 循环中的陷阱
```JavaScript
for (var i = 0; i < 3; i++) {
    setTimeout(function() {
        console.log(i);  // 全是 3
    }, 1000);
}
```
生命周期分析：  
var i 是函数作用域变量，三个闭包共享同一个 i
循环结束时 i = 3
1 秒后回调执行，读取的都是同一个已变为 3 的 i

修复（为每个闭包创建独立的自由变量）：
```JavaScript
for (let i = 0; i < 3; i++) {  // let 每次迭代创建新的绑定
    setTimeout(function() {
        console.log(i);  // 0, 1, 2
    }, 1000);
}
```

## 五、如何管理自由变量的生命周期？
表格
策略	做法
及时释放闭包引用	将闭包变量设为 null，断开引用链
避免不必要的捕获	闭包中只引用真正需要的变量
使用 WeakRef / WeakMap（JS）	不阻止垃圾回收的弱引用
显式清理（事件监听器）	removeEventListener 时一并清理闭包
```JavaScript
// 显式释放
let handler = createClosure();
handler();       // 使用
handler = null;  // 释放，让 GC 回收自由变量
```

## 六、一句话总结
自由变量的生命周期 = 闭包函数对象的生命周期。闭包是自由变量的"续命符"——只要闭包活着，自由变量就不能死；闭包死了，自由变量才能被回收。

# Go语言的闭包
+ Go语言能通过escape analyze识别出变量的作用域，自动将变量在堆上分配。将闭包环境变量在堆上分配是Go实现闭包的基础
+ 返回闭包时并不是单纯返回一个函数，而是返回了一个结构体，记录下函数返回地址和引用的环境中的变量地址

基于逃逸分析，可以实现闭包；基于闭包，实现了函数的一等公民。