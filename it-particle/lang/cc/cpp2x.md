# C++ 2x
(转)
modules/coroutines/concepts/ranges 这几个都是重磅功能，game changer。

modules不用说，提速编译，简化远古include结构，真正的模块化。然而stl的支持要到c++23。

coroutines的加入，会大幅影响编程思路、方式。更重要的是，会与c++23、c++26的send-receiver、ranges、executor、network、新async、low level file io产生巨大共鸣（共震？），这里会有一个统一的异步设计。然而stl的支持要等到c++23。

concepts好事多磨，磨了20年，终于磨成针了。然而不写template，无视。

ranges又又又提升了一层抽象，操作容器更方便了，语句更花哨了。感觉有点reactive programming的味道？然而stl支持的功能还不够，要等到c++23。

还有其他一些小功能：各种constexpr。std::string也能constexpr，还有什么constexpr dynamic_cast，这魔法真够黑的。来跟我念，动态类型转换的常量表达式，无法想象这是拿来干嘛的。c++已经上天了。

std::format。c++终于有format了。一个语言30年了才有format你敢信。过程好比单身30年，30年啊，stream撸到怀疑人生。什么<iomanip>、setprecision，这设计出来存心找碴的？

operator<=>（飞碟operator，三向比较）。貌似挺有用，只是这符号不是特别喜欢，有向rust靠拢的趋势。

std::span。现在已经有gsl::span了，标准化了比较顺手。大小从index_type换成了size_type，我觉的蛮好的，和stl统一。

char8_t。utf8专属类型，与char不能混用，编译器层面的支持。蛮好的，utf一家整整齐齐了。heterogeneous lookup。
之前map支持transparent comparator，现在unordered_map也有相应功能了，很方便。
坐等c++23 coroutine、executor、network、module的stl实现，还有pattern matching