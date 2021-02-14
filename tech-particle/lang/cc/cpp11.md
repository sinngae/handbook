# C++11
C++11是C++98后，第二个具有真正意义的C++标准，增加很多现代编程语言特性，如类型推导、智能指针、lambda表达式等，让人感觉是完全新的语言。。

+ 通过内存模型/线程/原子操作等来支持本地并行编程
+ 通过统一初始化表达式/auto/decltype/移动语义来统一对泛型编程的支持
+ 通过constexpr/POD更好地支持系统编程
+ 通过内联命名空间/继承构造函数和右值引用等，以更好地支持库的构建

更倾向适用库实现特性而不是扩展语言，比如std::thread/std::unorder_map/std::regex


## 主要新语言特性
### 关键字
+ alignas
+ alignof decltype
+ auto重新定义
+ static_assert
+ using 重新定义
+ noexcept
+ export 弃用
+ nullptr
+ constexpr
+ thread_local
+ override
+ final

## 《Understanding C++11 ReadNOte》note
```cpp
vector<list<int>> veclist; // C++11 有效；C++98/03报错>>被解释为右移

template <int i> class X {};
X<1 >> 5> x; // C++98编译通过，C++11编译错误，吧第一个>优先和<匹配了，需要用圆括号把`1 >> 5`括起来

// 强类型枚举
enum class Color {red, blue, green};
int x = Color::red;     // C++11报错，Color不能转int
Color y = 7;            // C++11报错，int不能转Color
Color z = red;          // C++11报错，red不在作用域
Color c = Color::red;   // C++11正常；C++98/03报错

// 指示编译器，该文件只编译一次
/* 同头文件预编译#ifndef-define-endif */
#pragma once

// 操作符_Pragma，效果同#pragma，但可以用在宏中
_Pragma("once");

// 日志
#define LOG(...) { \
    fprintf(stderr, "%s: Line %d: \t", __FILE__, __LINE__); \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
}

/* C++引入C头文件编译c++03被预定义位199711L；C++11被预定义为201103L
    #ifdef __cplusplus
    extern "C" {
    #endif
    // ... 
    #ifdef __cplusplus
    }
    #endif
*/
#ifdef __cplusplus < 201103L 
    #error "should use C++11 implementation"
#endif

// 禁止包含某些头文件，并提示包含其包装头文件
// 这是一种断言
#ifndef __DEST_H__
#error "Never use <this header file> directly; include <dest.h> instead."
#endif

/* 静态断言小技巧 
    原assert是用在运行时的，有些断言最好应该用在编译时。
    比如模板类型的sizeof运算值是否相等，进而决定是否拷贝。
*/
#define assert_static(e) \
do { \
    enum { assert_static__ = 1/(e) }; \
} while (0)
/* 编译时判定expr，如果否则编译失败，输出msg
    expr必须是常量表达式
*/
static_assert(expr, msg);

/* noexcept修饰符与noexcept操作符
    C++11弃用了动态异常声明，使用新的noexcept
    noexcept/noexcept(expr=true)修饰的函数如果抛出异常，则程序被终止运行
    noexcept比throw更高效，广泛使用于标准库 */
    void excpt_func() throw(int, double) { ... } // 弃用异常规范
    void excpt_func() noexcept; // 相当于noexcept(true)，相当于原throw()
    void excpt_func() noexcept(expr); // expr - 常量表达式：true - 不会抛异常； false - 会抛出异常
/* noexcept(expr)操作符通常用于模板 */
    template <class T>
        void fun() noexcept(noexcept(T())) {} 
/* 第二个noexcept是一个操作符，fun()是否是noexcept修饰函数取决于T()是否会抛出异常。
    当T()表达式会抛出异常（即noexcept(false)修饰函数），noexcept操作符返回false；否则为true。
    这样设计保证了通过表达式推导达到编译时判断泛型编程函数是否会抛出异常 */
/* 注意：noexcept修饰函数默认通过std::terminate调用终止异常函数，无法保证对象析构函数调用及栈自动释放。大多数时候，暴力终止程序是简单有效的。
    noexcept保证应用程序的安全的同时，提高标准库的性能，解决了throw修饰符异常扩散问题。
    C++11默认将类析构函数/delete函数/delete[]函数设置成noexcept，因为其不应抛出异常。
    相反，new函数/new[]函数声明成noexcept(false)的，原来是throw(std::bad_alloc)
*/

```

## 类型
1. 支持long long  
之前只加入了C99，被很多编译器支持。至少64位，常量使用LL后缀，无符号亚型使用ULL后缀。

2. 窄字符串转宽字符串
连接宽字符串和窄字符串，C++11标准编译器先把窄字符转成宽字符串，再连接。

## 类
### 快速出初始化成员变量
C++11允许类/结构体的非静态成员变量使用=或者{}进行就地初始化（头文件中初始化），类似构造函数使用()的初始化列表，后者作用在后面。非常量的静态成员变量需要在头文件外初始化。
### 非静态成员变量
C++11允许sizeof作用于类的非静态成员，像静态成员一样访问。
C++98中需要使用`sizeof(((Class*)0)->menber)`来计算。
### friend
C++11声明类的友元，不再需要class关键字，也可以使用typedef的别名
扩展了泛型编程中，类的友元的使用。也方便地把测试类当作目标类的友元。
### final/override
C++11（向Java学习的？）使用final关键字阻止基类虚函数被重写。
C++11使用override描述符修饰派生类重载函数，指示编译器该派生类是否有对应的基类虚函数（检查函数名/原型/是否虚函数，不检查参数变量名），如果没有则报错。虽然不能避免重载了另一个基类虚函数而不是程序员期望的，依然具有实际意义。
只用于函数名后，仍可做变量名，最好避免使用。
### using继承
子类中声明`using Base::Base;`和`using Base::func;`用于继承基类的构造函数和普通函数，就不用再透传基类的构造函数了，而且C++11隐式声明了继承构造函数，比原来更节省代码。
带有默认参数的构造函数，不会继承其默认值，但会继承多个形式的构造函数。
当派生类的多个基类的构造函数冲突时，需要显式声明解决了。
私有构造函数不会被继承。
使用了using继承构造函数，编译器不会再为派生类生成默认构造函数，如果基类没有显式的默认构造函数，则派生类没有默认构造函数，`Class c;`声明就编译报错。
### 委托构造函数
`Class(int val) : Class() { _val = val; };`，C++11可以在构造函数的初始化列表位置委派另一个构造函数。不能同时使用委派构造函数和初始化列表。
委托构造函数会形成链状委托构造，不能形成环形。
这样，可以使用构造模板函数产生目标构造函数，使构造函数的泛型编程成为可能。
委派构造函数可以使用try包裹，并catch捕获异常，只做跟踪，更外面不被处理的话，仍会终止运行。

### 移动构造函数
C/C++一般把可以放到赋值运算左边的叫做左值lvalue，能放在右边的叫做右值rvalue。更广泛地定义是可以取地址的有名字的是左值，反之就是右值。

C++11中，右值可分为两种，纯右值(prvalue)和**将亡值**(xvalue,expiring value)。
纯右值是指临时变量或者不跟对象关联的值，比如函数返回的临时变量值(非引用的，局部变量)，运算表达式，lambda表达式，字面量值，类型转换函数的返回值。

C++11新增的将亡值是将要被移动的对象，可以理解为纯右值的引用，比如函数返回的右值引用T&&，std::move的返回值，函数的转换为T&&的类型转换函数的返回值。(其他的都是左值，所有值必属于三者之一)

```cpp
class T {
public:
    T() : ptr(new int(10)) {}

    T(const T & t) : ptr(new int(10)) { /* 深拷贝 */ }

    T(T && t) : ptr(t.ptr) { t.ptr = nullptr; } // t1从t0"偷走了"堆上分配的内存
private:
    int *ptr;
};

T GetTemp() {
    T t;
    return t;
}

int main() {
    /* 为了实现调用者更简洁的书写：*/
    calc(GetTemp(), some(other(), maybe()); // 书写高效，可读性高

    T a = GetTemp(); // 需要深拷贝
    T & b = GetTemp(); // 左值引用，C++98中报错，bool &flag = true不会报错
    T && c = GetTemp(); // 右值引用，比a要少了一次构造析构

    /* 右值引用不能绑定到左值 */
    int m;
    int && n = m; // 异常，右值引用不能绑定到左值
} 
```
## 语法特性
### std::move
强制转化为右值

std::move_if_noexcept

引用折叠问题：typedef和声明里的引用及右值引用展开判定，如果其一时引用则是引用，否则是右值引用。

默认的拷贝/移动构造函数（移动构造/移动赋值/拷贝赋值），以及需要额外实现的情况。需要同时提供或同时不提供，以具有或不具有拷贝/移动语义。
#### 完美转发
std::forward
#### 总结
+ 引用和右值引用的作用都是把右值/将亡值绑定到左值的操作。
+ 有了移动语义，swap函数等模板编程将更加高效。特别是加上g++/clang++的RVO/NRVO
+ forward用于实现make_pair/make_unique等一系列模板函数
(待补充)

### 隐式类型转换
转换构造函数带来方便，也带来隐患，C++98使用explicit添加约束，使调用者只能显式转换。
C++11把explicit约束扩大到类型转换操作符函数。

### 列表初始化
```cpp
int brr[3] = {0};
int brr[] = {1, 2, 3};
int brr[] {1, 2, 3};            // C++11支持
vector<int> v{1, 3, 5};         // ...
map<int, int> m = {{1, 5}, {2, 6}, {3, 7}}; // ...

int i(1); int i{1}; // 没有区别

int *ptr = new int(1);
long *ptr = new long{2};

vector<int> func() { return {1, 3}; }

MyData d;               // MyData有一个operator[](<initializer_list>)函数 和 operator=(int)函数
d[{1, 3, 5}] = 3;       // 把第1、3、5位置，设置3
```
+ {}初始化，同()
+ STL容器{}初始化的支持来自于`<initializer_list>`头文件，使用模板类`initializer_list<T>`作为参数，即可实现自定义的列表初始化。
+ `<initializer_list>`还可以用于普通函数参数列表，还可用于`operator []`等操作符函数，可以实现调用者的编码灵活性
+ `<initializer_list>`还可以用于函数返回值，与返回一个字面常量相同
+ 防止类型收窄  
列表初始化是唯一一种可以防止类型收窄的初始化方式。

### POD类型
POD，Plain Old Data，意思是普通的简单布局的数据，用于说明一个类型的属性是平凡的(trivial)和标准布局的(standard layout)的。

平凡判定：
+ 拥有平凡的默认构造函数和析构函数
+ 拥有平凡的拷贝/移动构造函数
+ 拥有平凡的拷贝赋值运算符/移动赋值运算符函数
+ 不能包含虚函数/虚基类

C++11使用`template <typename T> struct std::is_trivial;`判定类型T是否平凡。

标准布局判定：
+ 所有非静态成员有相同的访问权限。
+ 基类和派生类不同时有非静态成员
+ 类中第一个非静态成员的类型与基类的不同  
  如果派生类的第一个成员仍然是基类，编译器仍会为基类（派生类中的）分配1字节的空间，因为C++标准要求类型相同的对象必须地址不同（指向同一实例？的基类对象和派生类对象必须不同）。
+ 没有虚函数和虚基类
+ 所有非静态数据成员均符合标准布局类型，其基类也符合标准布局。

C++11使用`template <typename T> struct std::is_standard_layout;`判定类型T是否平凡。

使用`template <typename T> struct std::is_pod;`判定类型T是否是POD。
很多内置类型都是POD的。

使用POD的好处：
+ 字节赋值，可以使用memset和memcpy对其做初始化和拷贝
+ 提供对C内存布局兼容，C++和C可以用POD互相操作。
+ 保证了静态初始化的安全有效

### 非受限联合体
C++98中，Union成员不能是非POD类型，也不能是静态类型或引用类型。
C++11中，Union成员可以是任何非引用类型。Union静态成员没有实用意义。如果有非POD类型成员，则需要为其添加非平凡的构造函数和析构函数，才可以实例化一个该Uinion对象。

枚举类有一个匿名的非受限联合体成员和一个枚举成员，具有很大灵活性。

### 用户自定义字面量
`T operator "" _C(const char* str, size_t n) {}`，
C++11中可通过自定义后缀标识操作符（字面量操作符）函数，来解析以_C为后缀的字符串，并返回类型T，其中_C是自定义的后缀，类型T是自定义的数据类型。字面量操作符函数的参数可以是字符串、整型、浮点型等。

调用规则：
+ 如果字面量为整型，则字面量操作符函数只能接收unsinged long long或const char*参数。过长则使用后者。
+ 如果字面量为浮点型，则字面量操作符函数只能接收long double或const char*参数。过长则使用后者。
+ 如果字面量为字符串，则字面量操作符函数只接收const char*,size_t做参数。
+ 如果字面量为字符，则字面量操作符函数只接收一个char为参数。

注意：
定义字面量操作符函数，双引号和自定义后缀之间必须有空格；后缀建议以下划线开头的唯一定义，以避免混乱。

### 内联名字空间
C++98中，不允许不同的命名空间中特化模板。
C++11中，允许在父命名空间定义或特化inline修饰的子命名空间模板，也适用于子匿名命名空间。
内联命名空间会破坏命名空间的封装，应该仅在不需要隔离名字的时候适用。

可用宏定义#if __cplusplus == 201103L判定当前编译是否是c++11.

ADL，Argument-Dependent name lookup，参数关联名称查找，该特性允许编译器在函数参数的所在命名空间中查找函数的名字(仅在当前命名空间找不到时)，无需在使用时声明该函数所在的命名空间。ADL也是破坏了封装，并不被推荐适用。

### 模板的别名
```cpp
using uint = unsigned int; // C++11中，using可用于定义变量别名，同typedef
typedef unsigned int UINT;

template<typename T> using MapString = std::map<T, char *>;
MapString<int> numberString;
// C++11中，typedef的能力是using的子集，using更灵活。
```
C++11标准库is_same模板类可用于判定两个类型是否相同。

### 一般化的SFINEA规则
C++模板中的一条著名规则，SFINEA，Substitution failure is not an error，匹配失败不是错误。具体的是指，对重载的模板的参数进行展开，如果发生了一些类型的不匹配，编译器不会报错。
编译器根据模板推导规则时，或者说在模板类实例化时，会匹配更“精确？”的模板。
C++98中，SFINEA并不清晰，且不被主流编译器支持。
C++11中，模板参数替换使用了表达式时，表达式中没有出现外部于表达式本身的元素，如一些模板实例化，或隐式产生拷贝构造函数，这样的模板推导都不会产生SFINAE失败。
没大懂这段。

## 泛型
### 函数模板的默认模板参数
可以用=提供类模板/函数模板的默认模板参数及默认值。
模板参数不是由模板函数的默认形参推导出来的，而是由函数的实参推导的。
### 外部模板
同一函数模板在不同源文件中被实例化，链接在一起的时候报重名错误。以前，编译器需要去除重复的模板实例化。类似于extern声明全局变量一样，如`extern template void fun<int>(int);`（一般放在头文件里），使用其他源文件里的函数模板的显式实例化（所有源文件里唯一）。这样链接器的工作变得简单，缩短了编译时间（模板展开的开销十分可观，在某些大型项目）。
外部模板声明不能用于静态函数，可用用于类静态成员函数。
### 局部/匿名类型做模板实参
C++11支持了局部类型/匿名类型做模板实参。如匿名结构体/联合体/枚举。
但不支持匿名结构体直接嵌在模板实参位置。

**类型收窄问题**

## 类型推导
### auto
```cpp
auto var = expr; // 适用基本类型/结构体/类等，也支持命名空间、模板
auto * var = expr; // 指针类型
auto & var = expr; // 引用类型
// C++11规定，auto不能从初始化表达式中获取cv限制符（声明为引用/指针的，保持与其引用的对象相同的属性），但是可以和cv限制符一起适用
// cv限制符，const volatile
const auto var = expr;
volatile auto var = expr;
auto x = expr1, y = expr2; // 多变量声明需要类型相同，否则编译报错
auto i = 1, &ref = i, *ptr = &p; // 从左到右推导
const auto *cptr = &i, j = 2;
// ? auto k = 3, const* cptr = &k;
auto var(expr);
auto var{expr};
auto var = new auto(expr);
```
auto并非一种类型声明，而是一个类型声明占位符placeholder。在编译时期，由编译器填充推导出的类型。
C++11只保留auto的类型指示符永福，C++98和C语言中的`auto int i = 1`

auto并非万能，受制于语法不能二义性约束(也有可能受限于开发编译器的复杂度)，人为观察很容易推导，但C++11还不支持的：
+ 不能做函数形参类型；
+ 不能做结构体的非静态成员变量类型(即使有初始值，可推导出类型，仍然被标准禁止)；
+ 不能声明auto数组，如`auto var[3]`
+ 不能使用auto实例化模板

好处：
+ 简化迭代器书写（之前迭代器都很长）
+ 泛型编程扩展性提高（之前不同的模板类的函数只能返回相同的类型）

限制：
+ auto推导，必须是对变量初始化
+ 不能用于函数参数
+ 不能用于类的非静态成员变量
+ 不能用于数组
+ 不能用于模板参数

nonconst的左值引用不能和一个临时变量绑定；指针不能指向临时变量。

## decltype
```cpp
decltype(exp) var = value;

T t;
decltype(t) var = val;
Ta t1;
decltype(t+t1) var = val;

// 匿名的强类型枚举/union/struct数组
enum class{E1, E2, E3}anon_e;
union {
    decltype(anon_e) key;
    char *name;
} anon_u;
struct {
    int d;
    decltype(anon_u) id;
} anon_a[100];
```
C完全不支持动态类型，C++98就有了RTTI。
C++ RTTI，runtime type identify，C++ 运行时类型识别。该机制为每个类型产生一个type_info类型的数据，可以使用typeid查询变量的type_info信息。type_info的name成员函数返回类型名（不同的编译器会打印出不同的名字，g++打印出的带有前缀5），C++11中增加了hash_code函数返回该类型唯一的哈希值。
is_same模板函数是在编译器获得结果，RTTI是运行时。
C++ RTTI会带来一些运行时的开销，一些编译器提供选项关闭该特性（XL C/C++的-qnortti；GCC的-fno-rttion；微软编译器的/GR-）。
运行时确定类型带来某些场景编码的灵活性，但更多的需要在编译期确定类型、运行期使用类型。

类型推导是为了模板和泛型编程的广泛使用而引入的。泛型编程中，类型对编写者是未知的，编译器辅助做类型推导，增加了泛型编程的适用范围和编写方式。
auto/decltype之前，各编译器有自己的类型推导，如gcc的typeof操作符。C++11中标准化为auto/decltype。

+ decltype编译时从表达式推导类型，而不用取其值。
+ decltype 与 using/typedef结合使用，可以增加代码可读性（复杂类型的变量或表达式时）。
+ 匿名的强类型枚举/union/struct/struct数组  
匿名一般是不想被复用，decltype留了后路
+ decltype扩大了泛型编程的适用范围  
常用于推导返回类型；如果推导结果并不合法（比如数组相加），还是需要提供合法的实现。
+ decltype不能用函数名做参数，可以用函数调用表达式，但不会真的调用函数

<type_traits>里std::result_of底层即使用了decltype。
标准内部使用declval语法技巧？

`decltype(expr)`:
1. 如果expr是没带括号的标记符表达式(id-expression)或类成员访问表达式，那么decltype(expr)就是expr所命名的实体的类型。如果expr是重载函数，编译会报错。
2. 否则，假设expr的类型是T，且是一个将亡值，则得到T&&；
3. 否则，假设expr的类型是T，且是一个左值，则得到T&;
4. 否则，假设expr的类型是T，则得到T;

标记符表达式/id-expr：代码中除去关键字、字面量、等编译器需要使用的标记之外的，程序员自定义的标记(token)都可以是标记符(identifier)。

```cpp
int i = 0;
int arr[3] = {1};
struct S { double d; }s;
void Overloaded(int);
void Overloaded(char);
void Func();
int && Xvalue();
const bool Fun(int);

// 编译错误
decltype(i) a;
decltype((i)) b; // 编译失败
decltype((i)) b = 1; // int&

// 规则1
decltype(arr) var;              // int[5]
decltype(ptr) var;              // int*
decltype(s.d) var;              // double
decltype(Overloaded) var;       // 编译错误
decltype(Func) var;             // void (*func)()

// 规则2 
decltype(Xvalue()) var = 1;     // int &&

// 规则3 左值规则 int& 
decltype(true?i:i) var = i;     // 三元运算符 返回左值
decltype((i)) var = i;          // ()表达式 返回左值
decltype(++i) var = i;          // ++i 返回左值
decltype(arr[3]) var = i;       // []返回左值
decltype(*ptr) var = i;         // *返回左值
decltype("lval") var = i;       // const char(&)[9]，字符串字面常量为左值

// 规则4
decltype(1) var;                // 右值
decltype(i++) var;              // i++ 返回右值，同i
decltype((Fun(1))) var;         // const bool，忽略圆括号
/* 左值，将亡值，右值的本质
C++中的左值，应该是数据寄存器中的值；
将亡值，是仅在数据寄存器中的值(不在内存中)；
右值，应该是地址寄存器中的地址，对应内存的一个地址。
以上并不严谨，仅是猜测
*/
```
C++11标准库<type_traits>，模板库类is_lvalue_reference/is_rvalue_reference/is_const/is_volatile用于帮助开发者做推导结果识别。

decltype推导结果的cv限制符与表达式的cv限制符一致。但对象定义有cv限制符，其成员的decltype推导结果不会有cv限制符。
decltype推导后，可与cv限制符、&等冗余符号复合使用。如果推导结果已有该冗余符号，复合则被忽略（特殊地，decltype推导结果是指针的与*符号复合，则为多级指针。）。

使用decltype更加自然（人类亲和）。

### 追踪返回类型
```cpp
template<typename T1, typename T2>
decltype(t1 + t2) Sum(T1 & t1, T2 & t2) { return t1 + t2; } // 编译器从左向右读取，无法解析decltype(t1 + t2)

template<typename T1, typename T2>
auto Sum(T1 & t1, T2 & t2) -> decltype(t1 + t2) { return t1 + t2; } 
/* 追踪返回类型：函数返回值移到参数声明之后，复合符号 -> decltype(t1 + t2)
    用auto做占位符，->return_type
*/

// 返回类型的后置
auto func(char *a, int b) -> int;

auto NS::fun() -> NS::type { return NS::value; } // 后续两个NS::可省略

// int (*(*pf())())() {} 返回一个（返回函数指针的）函数的指针的函数
auto pf() -> auto (*)() -> int (*)() {}
```
+ 扩展了泛型编程
+ 简化一些函数定义，提高可读性

### for循环
```cpp
int arr[3] = {1, 2, 3};
// for三元循环
for (int i = 0; i < sizeof(arr)/sizeof(arr[0]); ++i) { /**/ }
// for_each模板三元循环
for_each(arr, arr + sizeof(arr)/sizeof(arr[0]), /**/);
// C++11 基于运行时范围的for循环
for (int & e: arr) { /**/ }
for (int e: arr) { /**/ }   // 迭代的值不会被修改
for (auto e: arr) { /**/ }  // 
```

可以使用基于范围的for循环的条件：
1. for循环迭代的范围是可确定的，有begin和end函数的类/数组
2. 迭代的对象实现++和==等操作符

符合的有：
标准库容器，如string/array/vector/deque/list/queue/map/set等。
可以确定大小的数组。

for三元循环中使用auto是迭代器类型，基于范围for循环使用auto是解引用后的对象。

## 类型安全
### 强类型枚举
宏定义枚举；匿名/具名枚举类型枚举；静态常量枚举；

C/C++枚举类型的缺陷：(非强类型作用域；允许隐式转换成整型；占用存储空间及符号不确定)
+ enum类型的名字及其成员的名字都是全局可见，与C++的ns/class/struct/union不同，不同enum类型的成员可能会冲突，而且ns中的成员可能被enum成员污染。
+ 作者想要的是枚举类型比较，但编译器执行的是枚举转化成int类型数据的比较。
+ 占用空间大小不确定，由编译器决定，visual C++使用无符号类型支持枚举；g++为不同枚举类型使用不同的类型。

可以使用类内部枚举类型(封装)避免类型污染，但1.增加复杂度；2.class类型丢失POD属性；3.被系统ABI判定为结构体，不能使用寄存器传递；

C++11 引入强类型枚举 strong-typed enum （强作用域；转换限制；可指定底层类型）
```cpp
enum class position {up, down, left, right};
enum class position : int {up, down, left, right}; // 可指定除wchar_t以外的任何整型
position pos = position::up;
position pos = up; // 枚举成员的名字自动输出到父作用域

/* enum struct 同 enum class
    匿名的enum class对象，不能设置值/比较值，所以一般都是具名；匿名的enum struct可以；
*/
enum struct position {/**/}

enum position : int {/**/} // 也是合法的
```

### 智能指针
堆内存管理/智能指针/垃圾回收

C/C++程序常会发生运行时闪退，内存占用增长等最终重启的症状，大都与堆内存管理有关。
+ 野指针：内存单元(或其一部分)已被释放，指向其的指针仍被使用。特别在该内存单元重新被分配使用，结果无法预测。
+ 重复释放：释放已经被释放的内存单元，或被重新分配的内存单元。通常会导致程序打印大量错误及诊断信息。
+ 内存泄漏：不再需要的内存单元没有及时释放。

C++98中，智能指针通过模板类型auto_ptr实现；可以避免堆内存忘记释放的问题；但拷贝时返回一个左值，且不能调用delete[]，在C++11中被废弃了。
C++11中，使用unique_ptr/shared_ptr/weak_ptr等实现智能指针。
```cpp
unique_ptr<int> up1(new int(11));
unique_ptr<int> up2 = up1;              // 编译错误
unique_ptr<int> up3 = move(up1);        // up1失效，*up1运行时错误
up3.reset();                            // 显式释放内存，*up3运行时错误
up1.reset();                            // do nothing

shared_ptr<int> sp1(new int(22));
shared_ptr<int> sp2 = sp1;
sp1.reset();                            // sp2 仍可访问
```
*操作符访问数据；reset函数释放堆内存；
unique_ptr是独占堆内存对象的，仅能通过move语义转移到另一个unique_ptr；
unique_ptr是一个删除了拷贝构造函数，保留移动构造函数的指针封装类型；
unique_ptr仅可使用右值构造；
shared_ptr是共享堆内存对象的；
shared_ptr使用引用计数实现，只有引用计数归零时，才会真正释放所占有的堆内存空间；
weak_ptr可以指向shared_ptr对象，却不增加引用计数，使用其lock成员可返回一个shared_ptr对象（如果其堆内存对象已无效，则返回nullptr，可用于检测shared_ptr是否有效）；

#### 垃圾回收机制
Garbage Collection，完全不用写作者考虑回收堆内存的方案。
两种GC：基于引用计数的GC和基于跟踪处理的GC（reference counting/tracing）。
前者实现简单，没有程序暂停；存在环形引用的问题，引用计数的开销并不小。

后者产生跟踪对象的关系图，做垃圾回收：
1. 标记-清除（Mark-Sweep）
该算法将程序中正在使用的对象作为根对象(栈内存对象？静态区数据？)，从根内存查找堆内存对象，并做标记。被标记的就是可达对象或活的对象，没有被标记的就是垃圾，第二步中被清理。
一般GC时被标记的活对象不会被移动，存在大量的内存碎片问题。
2. 标记-整理（Mark-Compact）
标记同1，标记后将活的对象向左靠齐（向堆地址低地址对齐？），解决内存碎片问题。
根对象及活对象等对堆内存的引用都需要更新。
3. 标记-拷贝（Mark-Copy）
把堆内存分为两部分From/To，从From从拷贝活对象到To，然后释放From，再反向操作一次。只是2的一种实现，且堆的内存只利用一半。

#### C++最小垃圾回收支持
C++11指针也没有解决环形引用的问题，使用上有限制。

C++垃圾回收库Boehm，提供堆内存分配函数显式地替代malloc；实际使用还是有限制，可移植性也不好。

2007 C++垃圾回收议案，过于复杂，而且依然有问题。

C/C++的垃圾回收复杂来源于其堆指针的过分灵活，编译器即使实现了隐藏指针分析，其代价必然很大。
```cpp
// 指针运算
int *p = new int;
p += 10;                // 垃圾回收p？
p -= 10;                // GC处理p？
*p = 10;                // 失效？

int *p = new int;
int *q = (int*)(reinterpret_cast<long long>(p) ^ 2012);
// GC回收p
q = (int*) (reinterpret_cast<long long>(q) ^ 2012); 
*q = 10;                // 失效
```
C++11标准了最小的垃圾回收支持，但各编译器没有实现的。

安全派生的指针(safely derived pointer)：指向由new分配的对象或其子对象的指针。

安全派生指针的安全派生操作包括：
+ 解引用上的引用，&*p
+ 具有定义的指针操作，p+1
+ 具有定义的指针转换，static_void<void *>(p)
+ 指针和整型之间的reinterpret_cast<intptr_t>(p)，intptr_t是？

安全派生指针的不安全派生操作：如上文的异或(^2012)；整数转指针(*p = 10;)；

C++11 通过`pointer_safety get_pointer_safety() noexcept`判定编译器是否支持最小的垃圾回收。pointer_safety::[strict/relax/preferred]分别代表支持/略支持/不支持。

C++11的最小垃圾回收支持包括：
+ declare_reachable，显式通知GC某对象是可到达的，即使GC判定不可达
+ undeclare_reachable，取消可达声明
+ declare_no_pointers，声明一大片内存不存在有效的指针
+ undeclare_no_pointers，取消无有效指针声明

垃圾回收带来兼容性问题，难以处理。

C++11最小垃圾回收支持，仅限于系统new操作符分配内存，不包括malloc分配内存。malloc分配需要作者自己管理。

## 常量表达式
C++11中，使用constexpr关键字修饰函数表达式，称为常量表达式，使编译器在编译时对该函数表达式做计算，并获得一个常量（实际编译器可以不这么做，但要起到同样的效果）。
常量表达式还可以是数据声明，类的构造函数等。

常量表达式函数要求：
+ 函数体只有一条return语句  
可以有不会产生实际代码的语句，如`static_assert(0 == 0, "assert fail.")`，using指令，typedef指令等
+ 函数必须返回非void值
+ 在使用前必须已有定义
+ return返回语句中，不能使用非常量表达式的函数、全局变量，且必须是一个常量表达式

对编译环境和运行环境，浮点数精度可能不同，C++11允许编译时浮点数常量表达式，但要求起精度不低于运行时浮点数常量精度。

C++11中，constexpr不能用于修饰自定义类型，可用于修饰类型构造函数，不可用于修饰虚函数。用于修饰类型构造函数：
+ 函数体必须为空
+ 初始化列表只能由常量表达式来赋值

常量表达式可用于模板函数，如果模板特化结果不满足常量表达式，constexpr被忽略。

常量表达式支持递归，C++11中规定，常量表达式应支持512层递归。

```cpp
constexpr int f();
int a = f();                // 运行时调用
const int b = f();          // 运行时调用
constexpr int c = f();      // 编译时使用，但是没有定义，报编译错误
constexpr int f() { return 1; }
constexpr int d = f();      // 编译时使用

const int i = 1;            // 如果i在全局命名空间，则编译器一定会为i产生数据
constexpr int j = 1;        // 如果不被其他代码显式使用，编译器可以不产生值(类似右值字面常量，具名无数据枚举值的编译时常量)

struct Data {
    constexpr Data(int d) : data(d) {}
    constexpr int GetData() { return data; }
private:
    int data;
};
constexpr Data data {10};
constexpr int d = data.GetData();
```

C++模板有基于编译时期运算的编程方式，称为模板元编程，template meta-programming。
constexpr元编程，与template元编程都是图灵完备的？，而且constexpr支持浮点数计算，三元表达式，逗号表达式。
constexpr元编程有赖编译器的支持。

## 变长模板
```cpp
// C++ 变长函数 variadic function
void fun(int argc, ...) {
    va_list offset;
    va_start(offset, argc);               // 获得变长列表句柄argv
    for (int i = 0; i < argc; ++i) {
        double db = va_arg(offset, double);   // 每次获取一个参数
        /* do something */
    }
    va_end(offset);
    return;
}
```
printf使用没有定义转义字的非POD数据，会导致未定义行为。变长参数破坏了C++的类型安全，是个不规范的后门。

C++11标准中的tuple类模板，比C++98的pair类模板更泛化。可以接受更多不同类型的组合，如`std::tuple<int, double, char, std::string> collects`；
可以使用模板函数特化tuple模板类型，如`std::make_tuple(1, 1.2, '3', "hiwork");`；
tuple类模板使用的既变长模板。
```cpp
template <typename... Elements> class tuple;
// Elements称为模板参数包，template parameter pack
// 上文是类型的模板参数包，也可以是非类型的，甚至模板类型的
template <int...A> class NonTypeVarTmpl{};
NonTypeVarTmpl<1, 0, 2> ntvt;

template<typename... B> class B{};
template<typename... A> class Tmpl : private B<A...> {};    // A...称为包扩展
template<X, Y> xy;


// tuple的变长模板的递归实现
template<typename... Elements> class tuple; // tuple是变长模板
template<typename Head, typname... Tail>
class tuple<Head, Tail...> : private tuple<Tail...> { // 递归的偏特化定义
    Head head;
}
template<> class tuple<> {};                // 递归推导到此边界为止

// 函数参数包，function parameter pack
template<typename ... T> void f(T ... args);
```

变长模板远强于变长函数。

C++11中，参数包可以展开的位置：
+ 表达式
+ 初始化列表
+ 基类描述列表
+ 类成员初始化列表
+ 模板参数列表
+ 通用属性列表
+ lambda 函数的捕捉列表

参数包展开的方式可以是Arg.../Arg&&...等
```cpp
template<typename... A> class T: private B<A>...{};
// 解包为class T<X, Y> : private B<X>, private B<Y>{}
template<typename... A> class T: private B<A...>{};
// 解包为class T<X, Y> : private B<X, Y>{}

// 比较复杂的
template<typename A, typename B> struct S{};
template<
    template<typename... > class T, typename... TArgs
   ,template<typename... > class U, typename... UArgs
   > struct S< T< TArgs... >, U< UArgs... > > {};
S<int, float> var;
S<std::tuple<int, char>, std::tuple<float>> var;
```
C++11引入sizeof...，用于计算参数包中参数个数。

语法上，编程上，使用变长模板都有复杂度，但对库的编写者，具有好呢好的实用性。
C++11标准库中，tuple、emplace_back等都是变长模板类和变长模板函数。

## 原子类型和原子操作
C++11之前，C/C++主要使用pthread和OpenMP编译器指令做为多线程编程模型。
OpenMP把线程化工作放在编译时，分割线程化区域工作放在编写时。
C++11标准引入多线程支持，原子操作，原子类型。

### 原子操作/原子类型
原子操作，即多线程程序中的 最小的且不可并行化的 操作。原子操作一般通过互斥访问来保证，即需要平台相关的特殊指令。
C++11之前，C/C++代码通过嵌入内联汇编代码实现，或者pthread库的互斥锁做到粗粒度的互斥。pthread的加解锁管理和嵌入内联汇编都是比较麻烦复杂的操作。

C11/C++11标准中，提供原子数据类型(cstdatomic头文件中)，不需要编写者加锁解锁，就能互斥访问。C++11还提供了thread库，支持线程管理。
需要同步的总是数据，而不是代码。

C++11提供atomic类模板，支持自定义原子类型。`std::atomic<T> t;`
C11使用新关键字_Atomic，支持自定义类型。

C++11中，原子类型只能从模板参数类型进行构造，禁止原子类型的拷贝构造、移动构造、operator=函数等，这些函数默认是删除的。
非原子类型可以使用原子类型构造或拷贝，因为原子类型模板定义了atomic<T>到T的转换函数。
大部分原子类型都支持读/写/交换/比较并交换等操作。
atomic还提供atomic_flag类型，可以实现自旋锁，用于实现自定义临界区。
熟悉底层的程序员还可以实现无锁编程，最大限度发挥并行编程性能，C++11也为无锁编程提供了语言支持。

### 内存模型/顺序一致性/memory_order
默认情况下，C++11中的原子类型的变量在线程中的执行顺序与编写顺序一致，即原子类型的顺序一致。非原子类型可以被编译器或CPU优化，调整执行顺序。
原子类型的顺序一致性（sequential consistent），保证了程序的正确性，即符合编写者预期？。

但不是所有的原子类型的顺序一致都是必要的，C++11还支持别的内存模型。
C++11对不同的平台、处理器、编程方式都进行了考量，总结出不同的内存模型（硬件的概念，指以什么样的顺序被处理器执行，需要CPU和编译器一起保证）。顺序一致往往意味着最低效的同步方式。
一些处理器可以将执行执行顺序打乱，实现超标量的流水线，即一个时钟里执行多条指令，这种内存模型是弱顺序的（weak ordered）。顺序一致是一种强顺序的（strong ordered）。
x86及SPARC都被看做是采用强顺序内存模型的平台；Alpha/PowerPC/Itanlium/ArmV7等要保证顺序一致，需要加入一条内存栅栏指令（memory barrier，如PowerPC的sync指令），对性能损耗极大。
C++11支持作者为原子类型操作指定内存顺序（memory order），包括多种内存模型:
+ memory_order_relaxed
+ memory_order_acquire
+ memory_order_release
+ memory_order_acq_rel
+ memory_order_consume
+ memory_order_seq_cst

在不同的编程模型中，对不同的操作，不同的硬件使用不同的内存顺序效果不同，