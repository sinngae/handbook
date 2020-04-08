# C++11专题
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

