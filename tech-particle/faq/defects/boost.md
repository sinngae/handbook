# 记录
## 2016-10-14 09:48:39
症状：1418-ThisfunctionhasnoneofDETERMINISTIC,NOSQL,orREADSSQLDATAinitsdeclarationandbinaryloggingisenabled(you*might*wanttousethelesssafelog_bin_trust_function_creatorsvariable)

分析：

处理：
## 2016-10-14 10:59:23
症状：terminatecalledafterthrowinganinstanceof'boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<std::runtime_error>>'
what():Regularexpressionistoolarge.

分析：

处理：


## 2016-10-14 11:01:32
a boost bug  
症状：
libboost_system.a(error_code.o):relocationR_X86_64_32against`.rodata.str1.1’cannotbeusedwhenmakingasharedobject;recompilewith-fPIC<br>
/usr/local/lib/libboost_system.a:erroraddingsymbols:Badvalue

分析：

处理：
boost库处理：编辑tools/build/v2/tools/gcc.jam 387行
```
ulesetup-fpic(targets*:sources*:properties*)
{
    locallink=[feature.get-valueslink:$(properties)];
    if$(link)=shared
    {
        localtarget=[feature.get-valuestarget-os:$(properties)];
```
改为
```
ulesetup-fpic(targets*:sources*:properties*)
{
    locallink=[feature.get-valueslink:$(properties)];
    if$(link)=shared||$(link)=static
{
localtarget=[feature.get-valuestarget-os:$(properties)];
```
