# makefile


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

wildcard    # 扩展通配符：根据指定通配符，获取文件集合
notdir      # 去除路径
# foreach
```



