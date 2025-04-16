# C basis

## hiwork
```c
#include <stdio.h>              // 标准库

main() {                        // 无参无返回的main函数
    printf("hi, work\n");       // \n换行符
}
```

## 类型
1. 基本类型
```c
int
char
float
double
/* 类型限定符 long short signed unsigned */
long int 
short int
signed int
unsigned int
/* 相关头文件 <limits.h>与<float.h> */
```
2. 常量

3. 类型转换

## 运算符
1. 算术运算符
`+ - * / %`
2. 关系运算符 与 逻辑运算符
`> < >= <= != ==`
`&& || !`
3. 自增自减
`++ --` 左与右
4. 位运算
`& | ^ << >> ~`
5. 赋值运算与表达式
`= += -= *= /= %= <<= >>= &= |= `
6. 条件表达式
`expr1?expr2:expr3`

### 优先级
不用死记优先级，编译器不同，优先级是不同的，代码最好用括号清晰地表示运算的先后。

## 流控制
1. 分号代表一条语句的结束
2. if-else语句 与 else-if语句
3. switch-case-default语句
4. while语句与for语句
5. do-while语句
6. break与continue
7. goto语句 与 标号

## 函数与程序结构
1. 函数要素
2. 作用域 与 外部变量 与 程序块
3. 头文件
4. 静态变量
5. 寄存器变量
register修饰符修饰的变量会被优先放到寄存器中
6. 初始化
7. 递归
8. C预处理器
9. 变长参数

## 指针与数组
### 函数指针

## 结构体
1. 自引用
2. typedef
3. union 与 enum
4. 位字段

## 输入与输出
1. 标准输入/输出
2. 格式化输入/输出 scanf printf fprintf
3. 文件访问
4. 错误处理 exit函数
`fprintf([stdin|stdout|stderr], ...);`
5. 行处理

## Unix系统接口
1. 文件描述符 fopen
2. 低级I/O read write create open close unlink lseek
3. 访问文件系统 opendir readdir closedir
4. malloc free

## 标准库


## faq
**藏头，摆尾**  
1. `struct st`最后一个成员类型为`class type data[]`，[]里为空，申请内存大小为`sizeof(st)+sizeof(class type)*n`，再转st，即可用st->data访问

2. 可以申请`sizeof(st)+n`,就可以用 &st-n 访问内存头，这个可以叫藏头，上面那个叫摆尾。

**sizeof**  
`struct st *stptr = malloc(sizeof(*stptr))`，这样初始化有个好处，重命名时改动的少。
sizeof是编译时确定的：
```cpp
sizeof(void); // 在vc中是0，gcc中是1,c++中编译通不过。

struct empty{};
sizeof(struct empty); // =1(c++)，gcc中对c语言返回0

sizeof("12345");    // =6，相当于char[6];
sizeof("12345"+1);  // 相当于sizeof(char*)
sizeof(int*);       // 是4或8，64位机都认为是8
```

**EOF**  
输入结束，文件结束