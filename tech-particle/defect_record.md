# 痛苦压测ffmpeg
2019-10-31<br>
使用ffmpeg做生成视频文件的缩略图，做压测时，对于少量样本，没有泄露，但是当样本扩大到全磁盘的各种格式、来源的视频，一直有内存泄漏，不管怎么调整几个函数的使用。<br>
ffmpeg的api混乱，推荐的用法也是不清楚的。考虑到视频格式的多样、视频文件来源的多样，ffmpeg要处理各种合法的非法的视频文件，稍有异常没有被处理，就会导致缩略图异常、内存泄漏、甚至程序崩溃。对于稳定性要求高的程序，最好把ffmpeg相关部分做成另一个程序服务隔离起来。

# 一点疑问
2018-05-31<br>
挂载文件，一个cpu发指令写，另一个cpu发指令读取，会产生访问冲突么

# 
1.struct 跟在成员后加:1 位域操作，表示这个成员的大小占所定义类型的1bit（:2,2bit），大小不能超过成员类型包含的总bit数。
2.try块中创建的对象，会在异常跳出前析构。
3.后台启动，标准输入和输出会被忽略

# 
2016-10-1409:48:39
## 症状：1418-ThisfunctionhasnoneofDETERMINISTIC,NOSQL,orREADSSQLDATAinitsdeclarationandbinaryloggingisenabled(you*might*wanttousethelesssafelog_bin_trust_function_creatorsvariable)
## 分析：
## 处理：
# 
2016-10-1410:59:23
## 症状：terminatecalledafterthrowinganinstanceof'boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<std::runtime_error>>'
what():Regularexpressionistoolarge.
## 分析：
## 处理：
===========================================================================================================================================
# a boost bug
2016-10-1411:01:32
## 症状：
libboost_system.a(error_code.o):relocationR_X86_64_32against`.rodata.str1.1’cannotbeusedwhenmakingasharedobject;recompilewith-fPIC<br>
/usr/local/lib/libboost_system.a:erroraddingsymbols:Badvalue
## 分析：
## 处理：
boost库处理：编辑tools/build/v2/tools/gcc.jam 387行
"""
ulesetup-fpic(targets*:sources*:properties*)
{
locallink=[feature.get-valueslink:$(properties)];
if$(link)=shared
{
localtarget=[feature.get-valuestarget-os:$(properties)];
"""
改为
"""
ulesetup-fpic(targets*:sources*:properties*)
{
locallink=[feature.get-valueslink:$(properties)];
if$(link)=shared||$(link)=static
{
localtarget=[feature.get-valuestarget-os:$(properties)];
"""

# log4cplus/openssl/protobuf编译
log4cplus库处理
编辑Makefile文件
CFLAGS+=-fPIC
CXXFLAGS+=-fPIC
./configure--prefix=/usr/local/log4cplus/--with-pic

openssl库处理：
./config--prefix=/usr/local/openssl-fPIC
make
makeinstall

protobuf库处理：
编辑Makefile文件
CFLAGS+=-fPIC
CXXFLAGS+=-fPIC
CPPFLAGS+=-fPIC

