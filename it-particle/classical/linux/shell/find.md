# find命令
## man
### -name
`-name pattern`

无路径后缀的文件名（包括文件夹名）按shell模式匹配。一般有以下匹配：
+ 没有斜线(slash /),所以`-name a/b`没有匹配结果，`-path a/b`适用斜线匹配。元字符`'*','?','[]'`匹配`'.'`(最新的版本支持)。
+ -prune，忽略一个文件夹及其下的文件，同样适用于-path
+ 括号，没有特别的意义，尽管bash等shell的shell模式里括号一般有特殊意义。文件名匹配一般使用fnmatch库函数。
+ 使用引号包裹模式，以避免被shell当作扩展。

### -size
-size n[cwbkMG]
文件使用了n个单位的空间，后缀表单位：
+ b    512-byte blocks (默认)
+ c    bytes
+ w    two-byte words
+ k    Kilobytes (1024 bytes)
+ M    Megabytes (1048576 bytes)
+ G    Gigabytes (1073741824 bytes)

The size does not count indirect blocks, but it does count blocks in sparse files that are not actually allocated. Bear in mind that the '%k' and '%b' format specifiers of -printf handle sparse files differently.  The 'b' suffix always denotes 512-byte blocks and never 1 Kilobyte blocks, which is different to the behaviour of -ls.

sparse file 稀疏文件，// TODO:
## sample
```sh
# 查找文件名后缀为.out的文件
find ./ -name "*.out"

# 查找文件大小为/大于/小于2KB的文件
find ./ -size 4 # 可以替换成2048c/2K
find ./ -size +4 # 大于2KB的文件
find ./ -size -4 # 小于2KB的文件
find ./ -size +100K -size -200K # 大于100KB小于200KB的文件
```