# C++ basis
C++ 基础知识

## hiwork
```cpp
#include <iostream>             // 标准库
using namespace std;            // 命名空间
main() {                        // 无参无返回的main函数
    cout << "hi, work" << endl; // 
}
```

## 数据类型
### 文字常量
+ 字符型 单引号或一字节数字 转义序列(\n \001)
+ 整型 数字 U/u后缀的无符号类型 L/l后缀的长整型 不同进制(0开头八进制 0x开头十六进制)
+ 浮点型 f/F后缀 e/E中缀（科学计数法）
+ 字符串 双引号 L前缀（宽字符文字wchar_t，不可把char[]和wchar_t[]拼接）
+ true/false

静态数据区/const常量
### 内置类型
+ 字符型char
+ 整型 int
+ 浮点型 float double
+ unsigned/short/long修饰符
+ bool类型
### C字符串 与 C++字符串
C风格：字符数组/C++：std::string类/c_str()
### 指针类型
内存地址/动态内存分配/栈上地址
### 引用类型
```cpp
int &ref = entity;
```
引用即别名，提高函数压栈效率而生？栈顶的函数域可以访问整个栈上的数据
### 枚举类型 enum
### 数组类型
[]操作符
### 模板类型
stl容器/pair类型
### 类类型
class/操作符重载/拷贝构造函数
### const修饰符 与 static修饰符

### typedef关键字

### volatile修饰符
被修饰的变量，编译器有些优化不会做。

## 表达式与操作符
算数（+/-/*///%）/关系与逻辑（>/</=/&&/||/!）/赋值=/递增递减/条件（a?b:c）/
sizeof/new&delete/,(多个语句从左到右计算)/位操作符（~/<</>>/&/|）/bitset操作/
隐式转换/算术转换/显式转换/强制转换

### new&delete
动态分配对象/对象数组/常量对象

#### 动态内存分配

## 流程控制
声明语句/if-else/switch-case-default/for/while/do-while/break/continue/goto

## 函数
引用参数/指针参数/数组参数/缺省参数/缺省号/递归/inline/extern "C"/main()/
函数指针/
**函数重载**  
函数名相同，参数类型不同/操作符重载

## 域设计
局部/函数/类/命名空间（可嵌套）
### 命名空间

## 泛型设计
函数模板/类模板/标准库

## 异常设计
throw/try-catch-finally
### 函数的throw规范
### 异常类设计

## 基于对象设计
类定义/友元/静态成员/uion类？/位域？bitset类模板/嵌套类/局部类（函数内部声明）/
构造函数/析构函数/拷贝构造函数/public&protected/private

### 拷贝构造函数
引用参数与实参/形参

## 面向对象设计
继承&派生/多继承/虚函数/纯虚函数/接口编程？
### 虚函数表
### 构造函数/析构函数