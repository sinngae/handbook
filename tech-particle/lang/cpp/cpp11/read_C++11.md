# Understanding C++11 ReadNOte

## note
```cpp
vector<list<int>> veclist; // C++11 有效；C++98/03报错>>被解释为右移 

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

/* C++11弃用了动态异常声明，使用新的noexcept */
    void excpt_func() throw(int, double) { ... } // 弃用异常规范
    void excpt_func() noexcept; // 如果该函数出异常，则程序被终止运行，比throw机制更高效
    void excpt_func() noexcept(expr); // expr - 常量表达式：true - 不会抛异常； false
    template <class T>
        void fun() noexcept(noexcept(T())) {} // 把noexcept当作一个操作符
/*
*/
```

## 类型
1. 支持long long  
之前只加入了C99，被很多编译器支持。至少64位，常量使用LL后缀，无符号亚型使用ULL后缀。

2. 窄字符串转宽字符串
连接宽字符串和窄字符串，C++11标准编译器先把窄字符转成宽字符串，再连接。


**类型收窄问题**

## 类型推导
### auto
auto [var name] = [var value];

好处：
+ 简化迭代器书写（之前迭代器都很长）
+ 泛型编程扩展性提高（之前不同的模板类的函数只能返回相同的类型）

限制：
+ auto推导，必须是对变量初始化
+ 不能用于函数参数
+ 不能用于类的非静态成员变量
+ 不能用于数组
+ 不能用于模板参数

## decltype
decltype([exp]) [varname] = [value];

## 智能指针