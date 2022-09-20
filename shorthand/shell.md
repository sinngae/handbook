# shell
常用shell命令

## 示例
```sh
# go标准库中查找代码
grep -nr "const debug = false" "$(dirname $(which go))/../src"

## 上传下载：rz sz命令
apt install lrzsz
#下载：sz 文件名  上传：rz 文件

```

## shell脚本支持管道读取
```shell
#!/bin/bash
if [ $# -gt 0 ]; then
    exec 0<$1; # 文件输入绑定到标准输入（0 - 标准输入；1 - 标准输出；2 - 标准错误；），$1 是第一个参数
fi

while read line
do
    echo $line;
done<&0 # 从标准输入读取
exec 0<&- # 关闭标准输出
```