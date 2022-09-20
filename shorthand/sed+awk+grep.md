# sed
```shell
sed '[vim命令]' [file]
sed '1,3d' a.out # 删除1～3行
sed -e '4a\abc' a.out # 第四行插入abc

sed -n '100,200p' [file] # 取100~200行（共101行）
```
# awk
如何指定分隔符：
1. awk的命令选项：-F'分割字符'
2. awk的全局变量：FS (Field Separator)
3. awk默认分隔符：空白字符（包括空格、制表符、换行符）
```shell
# 单字符分割 (两者相同)
awk -F: '{print $1, $7}'
awk 'BEGIN{FS=":";} {print $1, $7}'

## +printf
awk -F'[sep]' '{ printf "ab%scd", $1 }' a.out # 以sep为间隔符，取第一个，拼接字符串 ab[xx]cd

# 多字符分割
echo 'abcdefcxdy'|awk -F cd '{print $1,$2,$3,$4}'
# 多种字符分割
echo 'abcdefcxdy'|awk -F [cd] '{print $1,$2,$3,$4}'

# 特别的
echo 'ab||ef|x|y'|awk -F'\\|\\|' '{print $1,$2,$3,$4}'

# 正则表达式
awk '/[REG]/{action}'
```

# sort+uniq
uniq只能去除相邻重复的字符串
```shell
sort|uniq # 先排序，再去重
```

# grep
```shell
```
# tail & head
```shell
head -100 [file] # 取前100行
tail -100 [file] # 取后100行
```
# more & less
```shell
```