# 记录
## 2016-10-14 09:48:39
症状：1418-This function has none of DETERMINISTIC, NOSQL, or READS SQL DATA inits declaration and binary logging is enabled(you* might* want to use the less safe log_bin_trust_function_creators variable)

分析：

处理：
## 2016-10-14 10:59:23
症状：terminate called after throwing an instance of'boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<std::runtime_error>>'
what():Regular expression is too large.

分析：

处理：


## 2016-10-14 11:01:32
a boost bug  
症状：
lib boost_system.a(error_code.o):relocation R_X86_64_32 against`.rodata.str1.1’ cannot be used when making a shared object;recompile with-fPIC<br>
/usr/local/lib/libboost_system.a:error adding symbols:Bad value

分析：

处理：
boost库处理：编辑tools/build/v2/tools/gcc.jam 387行
```cpp
ulesetup-fpic(targets*:sources*:properties*)
{
    locallink = [feature.get-valueslink:$(properties)];
    if $(link)=shared
    {
        localtarget = [feature.get-valuestarget-os:$(properties)];

// 改为
ulesetup-fpic(targets*:sources*:properties*)
{
    locallink = [feature.get-valueslink:$(properties)];
    if$(link)=shared||$(link)=static
{
localtarget=[feature.get-valuestarget-os:$(properties)];
```
