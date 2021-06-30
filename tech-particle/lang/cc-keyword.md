# C++ 关键字
C++11 共计73个关键字。

+ 相对C++98新增的有：
alignas alignof char16_t char32_t constexpr decltype noexcept nullptr static_assert thread_local
+ auto意义改变
+ register被认为过时，可能在未来标准移除
+ export 实现支持太少，编译效率低下，取消原有意义，保留给未来标准使用

## C++11 新增
### alignas
`alignof(expr)`获取表达式指定的对齐。
alignas用于声明？

### constexpr
作用类似const，修饰函数或对象，标识其是编译时的常量。
const与constexpr的区别？

### char16_t 和 char32_t
16位字符型和32位字符型，类似char和wchar_t。
C++11 新增用于表示Unicode字符，以客服wchar_t在不同平台无法保证确定宽度的缺点。
为何不同平台的厂商没有支持wchar_t宽度统一？

### decltype

### nullptr

### noexcept

### static_assert

### auto

## C++98
### asm

### auto
自动储存类，局部变量，指示编译器其生命周期在块结束时结束。

### const_cast/dynamic_cast/reinterpret_cast/static_cast
dynamic_cast是运行时转换，其他的都是静态检查。为了增强类型检查。

### bool/true/false
布尔类型
### break/continue/goto
跳转语句，汇编的jmp？

### case/default/switch
switch分支语句

### catch/throw/try
异常处理，throw用于抛出异常，及抛异常约束（C++11中仅用于抛异常）

### char/wchar_t
字符型/宽字符型，包括unsigned char，用于表示字符。
char 和 unsigned char的区别。

### const volatile
cv-qualifier，cv-限定符。
const - 只读类型，修饰变量或函数，常用于函数参数。
volatile - 被修饰变量的读取不能随便优化（比如被合并），适合映射I/O寄存器？

### struct/class/union
class - 一般类类型，可以作为模板类型参数(可用typename替代)或模板参数的语法组成部分
struct - 特殊的类类型，默认的访问级别不同。
union - 联合体

POD类型可以和C亲和。

class中可以包含struct类型，struct中可以包含类类型？

### delete/new
动态存储分配释放，对应delete/new(去配器？分配器)函数和析构函数/构造函数。

### do/for/while

### 数值类型
double/float/long/int/short/signed/unsigned

### if/else

### enum
C++11 带作用域的枚举？

### explicit
显式调用约束，一般用于限制默认构造/拷贝构造/移动构造等

### export
分离编译，导出模板。因不具有现实可用性，被C++11废除了，并保留。

### extern
外部链接/内部链接？
C++11 中用于模板显式实例化声明，可提升编译效率。

### friend
友元，不受访问级别限制地访问。

### inline
内联，编译器不一定内联。
改变了C++的ODR实用性(One Definition Rule)。

### mutable
非静态非const成员，用于表示被const修饰函数的例外成员。

### namespace

### operator
操作符

### private/protected/public

### register
inline类似。
C++11中标记为过时。
### return

### static
静态存储/函数具有内部链接(C语言)/类的对象共有。
除非被thread_local修饰，类的静态数据成员具有静态存储期。

### sizeof
返回类型或表达式类型的大小。
sizeof的表达式是不被求值的。
C++11可以对类的数据成员做操作数。

### template
模板声明/模板特化。
模板打破了类型系统的限制，推迟类型检查到其实例化，实现了代码复用、泛型、参数化编程。

### this
类的对象的指针右值

### typedef
别名声明。
C++11 中被using的部分代替。

### virtual
声明虚基类/虚函数

### typeid
获取类型的std::type_info，是C++的RTTI。
std::bad_typeid?

### typename
？

### using
using声明(ns内部成员的别名)/using指示(展开命名空间)

### void
C/C++的特殊的`空`。
+ 函数无返回值/无参数
+ 表达式void用法？
+ void*指针类型
