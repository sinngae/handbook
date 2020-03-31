# makefile
```makefile
@               # 放在行首，表示不打印此行
```

## 内置环境变量
```makefile
$(MAEFILES) # 默认空
$(MAEFILES_LIST)    # 默认空
$(VPATH)    # 默认空
# 指定依赖文件搜索路径。
# 使用空格或冒号间隔多个目录
# 按顺序从左到右搜索
$(SHELL)    # 默认/bin/sh
$(MAKESHELL)    # 默认空
$(MAKE) # 默认make
$(MAKELEVEL)    # 默认0
$(MAKEFLAGS)    # 默认空
$(MAKECMDGOALS) # 终极目标，默认当前目录同名文件
$(CURDIR)   # 当前目录basename
$(SUFFIXES) # 默认为：.out .a .ln .o .c .cc .C .cpp .p .f .F .m .r .y .l .ym .yl .s .S .mod .sym .def .h .info .dvi .tex .texinfo .texi .txinfo .w .ch .web .sh .elc .el
$(.LIBPATTERNS) # lib%.so lib%.a
```

## 自动化变量
```makefile
# 与shell脚本里的自动化变量有些异同，注意区分

# 以下皆默认以空格分隔。
$@  # 规则的目标文件名。
# 表示规则中的目标文件集。在模式规则中，如果有多个目标，那么"$@"就是匹配于目标中模式定义的集合。
$%  # 规则的目标文件是静态库文件时，代表其成员名：如foo.a(bar.o)，$% => bar.o，$@ => foo.a
$<  # 规则中的第一个依赖文件
$?  # 比目标文件更新的依赖文件列表。如果目标文件是静态库文件，则代表库成员（即.o文件）。显式规则中非常有用。
$^  # 目标的所有的依赖文件列表，并去重。
$+  # 同$^，但不去重。主要用于程序链接时，库的交叉引用场合
$*  # 模式规则和静态模式规则中，目标的模式中%代表的部分（称为茎）。如目标模式是a.%.b，对于文件dir/a.foo.b，$*的值是dir/a.foo。对于构造文件名非常有用。

# 以上自动变量，四个（$@ $< $% $*）在规则中代表文件名，其他三个代表文件名列表。通过这七个加上D/F形成了变种的自动环变量。（可以使用dir或notdir实现同样的功能）
$(@D)   # 目标文件的目录部分（不带最后的/）
$(@F)   # 目标文件的文件名部分
$(*D) $(*F) $(%D) $(%F) $(<D) $(<F) ...
```

## 内置特殊目标
```makefile
.PHONY:clean
# .PHONY后的目标是伪目标，即不论文件系统中是否存在该目标文件都无条件执行其所在规则定义的命令。
clean:

.SUFFIXES:.cx .sx   # 把.cx/.sx加入可识别后缀列表
.SUFFIXES:  # 删除所有已定义的可识别后缀
.SUFFIXES:.c .o .h  # 重新定义
# 可识别后缀列表，见[隐含规则](?)
# make命令的SUFFIXE变量用于定义默认的可识别后缀列表

.DEFAULT
# 重建那些没有具体规则（明确规则/隐含规则）的目标。有些目标，被其他规则所依赖，却没有重建它的规则，则使用default处理，一般就是error/warning信息，二般的？

.PRECIOUS
# make命令被中断时，该目标所依赖的文件不会被删除，如中间过程文件。
# 该目标依赖的文件可以是一个模式，如%.o

.SECONDARY
# 该目标的依赖文件作为中间过程文件，不会被自动删除

.DELETE_ON_ERROR
# 如果make执行错误，该目标的依赖文件将被删除

.IGNORE
# 忽略命令执行的错误

.LOW_RESOLUTION_TIME
# 该目标的依赖文件 低分辨率时间戳？

.SILENT
# 不打印

.EXPORT_ALL_VARIABLES
# 将之后所有的变量传递给子make进程

.NOTPARALLEL
# 目标的所有命令串行执行
```
## 内置函数
```makefile
# 文本处理函数
$(subst from,to,str)    # 替换字符串
$(patsubst pattern,replace,str-set)    # 替换通配符
$(strip strint) # 去空字符（空格 tab等不可显字符）
$(findstring find,in)   # in中查找字符串find
$(filter pattern...,text-set)   # 过滤掉不符合通配符的元素
$(filter-out pattern...,text-set)   # 反过滤，过滤出不符合通配符的元素
$(sort list)    # 排序，并去重
$(word N,text)  # 从text中获取第n个单词
$(wordlist s,e,text)    # 从text取出从s到e的子集（闭区间，且从1计数）
$(words text)   # 统计单词数目，最后一个单词：$(word $(words text),text)
$(firstword names...)   # 取首个单词

# 目录处理函数
$(dir names...) # 取目录，并去重：如src/file.a test.txt => src/ ./
$(notdir names...)  # 取文件名：如src/file.a test.txt => file.a test.txt
$(suffix names...)  # 取后缀：如 src/file.a test.txt exp => .a .txt
$(basename names...)    # 取前缀：如src/file.a test.txt exp => src/file test exp
$(addsuffix suffix,names...) # 加后缀：如.c,file.a exp => file.a.c exp.c
$(addprefix prefix,names...) # 加前缀：如src/,file.a exp => src/filea src/exp
$(join list1,list2) # 单词连接：如a b c,.c .o => a.c b.o c
$(wildcard pattern) # 获取当前目录下所有符合pattern通配符的文件名（？单个字符，*多个字符）

# 流程函数
$(foreach var,list,text)    # 循环遍历list中的元素var，做text处理。
#   如：
dirs := a b c
files := $(foreach dir,$(dirs),$(wildcard $(dir)/*))
#   效果同
files := $(wildcard a/* b/* c/*)

$(if condition,then-part[,else-part])   # 如果condition展开非空，则为真，执行then-part；否则，执行else-part；

$(call cmd,param,param,...) # 调用自定义函数
#   如：
reverse=$(2)$(1)
foo=$(call reverse,a,b)
#   如2：从环境变量PATH中，获取第一个路径与ls拼接一起
pathsearch=$(firstword $(wildcard $(addsuffix /$(1),$(subst :, ,$(PATH)))))
LS := $(call pathsearch,ls)
#   如3：对于o map MAKE，分别调用origin
map=$(foreach a,$(2),$(call $(1),$(a)))
o=$(call map,origin,o map MAKE)

$(value var)    # 获得变量的文本值，而不是其展开值
#   如：
FOO=$PATH   # 不是$(PATH)
all:
    @echo $(FOO)    # ATH，$P为空
    @echo $(value FOO)  # $PATH

$(eval func) # eval对参数进行展开，并作为Makefile文件的一部分，被make解析。
#   eval分两次展开其参数？

$(origin var)   # 获取var的信息，如出处
#   返回值有：undefined(未定义)/default(内置定义的)/environment(系统环境变量)/environment override(make -e或makefile文件中存在同名变量)/file(makefile文件中定义)/command line(命令行定义)/override(makefile中使用override定义)/automatic(自动化变量)
#   用于对变量合法性判断

$(shell cmd)    # 与引用(``)相同，其返回值中的换行及回车被替换成单空格。
#   创建进程执行，需要注意效率

# 控制函数
$(error text...)    # 产生致命错误，提示text到用户，退出make的执行
$(warning text...)  # 警告信息，make不退出
```

## 内置标识符
```makefile
# define-endef 定义多个变量？
# 多行定义，类似函数，可被eval展开
define var
@echo $(1)
@echo $(2)
endef
```
```makefile
# 条件判断
ifdef var
endif

ifndef var
endif

ifeq (A, B)
ifeq 'A' 'B'
ifneq (A, B)
ifneq 'A' 'B'
else
endif
```
```makefile
# 包含其他makefile文件（.mk文件）
include file... # 支持通配符
-include file...    # 忽略文件不存在的错误
sinclude file...    # 兼容版-include

# override
#   make命令行里可以定义变量，替代makefile文件里的变量。如果使用了override，则不会被替代。
#   如果使用了override，后续修改都需要用override
#   主要用于实现makefile中可以改变或追加make命令行里的变量
override var=val
override var:=val
override CFLAGS += -g
override var ?= val
override define var
endef

# export
#   make的递归目录过程中传递变量
subsys:
    cd subdir && $(MAKE)
#   或
subsys:
    $(MAKE) -C subdir
#   默认SHELL和MAKEFLAGS是自动传递的，可以使用unexport终止
export var...   # 变量导出，所有的子makefile皆可使用。export会展开变量
unexport var...
export  # 把所有变量导出，最新的版本已不支持该行为，而使用.EXPORT_ALL_VARIABLES来导出
.EXPORT_ALL_VARIABLES:
var1=val1
var2=var2

# vpath
#   与VPATH类似，但更灵活
vpath pattern dir   # 为所有符合pattern的文件指定搜索目录dir，多个目录用冒号分隔
vpath pattern   # 清除为pattern的文件设置的搜索目录
vpath   # 清楚所有设置的搜索目录
```


