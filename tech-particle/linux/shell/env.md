# env

可使用echo $变量名 查看  
printenv 打印变量名

PATH              |     bin目录
------------------|--------------
LD_LIBRARY_PATH   |     动态库目录
LIBRARY_PATH      |     静态库目录
C_INCLUDE_PATH    |     C头文件目录
CPLUS_INCLUDE_PATH|     C++头文件目录

在/etc/ld.so.conf後添加lib路徑，然後執行ldconfig生效

env             |   含义
----------------|-----------
HOME            |
SHELL           | shell类型
HISTSIZE        | history大小
MAIL            | 邮箱
LANG            | 语言
RANDOM          | 随机数
