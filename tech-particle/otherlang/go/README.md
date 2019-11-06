# 概述
Go语言具备良好的特性、丰富的library库、便捷的工具集，非常适合服务器端开发。

## go tools
```bash
# 执行脚本
go run test.go

# 生成二进制程序
go build test.go

# 获取go library
go get -u -v xxx.com/abc/ab
```

## Windows+VSCode配置Go环境
+ 安装VSCode、Go
+ 设置环境变量GOBIN=D:\Go、GOROOT=C:\Go GOPATH=D:\Go;D:\GoProjects
+ VSCode安装
代码高亮
自动完成列表(使用gocode)[github.com/nsf/gocode]
代码片段
快速提示函数定义(使用godef)[github.com/rogpeppe/godef]
跳转定义(使用godef)
查找引用(使用go-find-references)[github.com/lukehoban/go-find-references]
代码大纲(使用go-outline)[github.com/lukehoban/go-outline]
重命名(使用gorename)[golang.org/x/tools/cmd/gorename]
保存时编译
代码格式化goreturns[sourcegraph.com/sqs/goreturns] gofmt
Imports管理 gopkgs
调试(使用delve)
变量命名规范golint[github.com/golang/lint/golint ]
方法和类的签名帮助信息godoc
工作区符号搜索go-symbols
保存时编译测试go-build gotest
