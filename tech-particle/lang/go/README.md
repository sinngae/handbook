# 概述
Go语言具备良好的特性、丰富的library库、便捷的工具集，非常适合服务器端开发。

## Go默认环境变量
+ GOROOT Go的安装目录;$GOROOT/bin需要配置到$PATH中；
+ GOPATH 指定开发工作区，存放源代码、测试文件、库文件、二进制文件等。换开发工作区，需要重新修改该变量。
+ GOBIN 开发程序编译后二进制命令的安装目录（go install的目标路径）
+ GOOS
+ GOARCH

## go tools kit
```bash
go env #查看go相关环境变量
go run test.go # 执行脚本
go build test.go # 生成二进制程序
go get -u -v xxx.com/abc/ab # 获取go library到$GOPATH
```
+ 代码高亮
+ 自动完成列表(使用gocode)[github.com/nsf/gocode]
+ 代码片段
+ 快速提示函数定义(使用godef)[github.com/rogpeppe/godef]
+ 跳转定义(使用godef)
+ 查找引用(使用go-find-references)[github.com/lukehoban/go-find-references]
+ 代码大纲(使用go-outline)[github.com/lukehoban/go-outline]
+ 重命名(使用gorename)[golang.org/x/tools/cmd/gorename]
+ 保存时编译
+ 代码格式化goreturns[sourcegraph.com/sqs/goreturns] gofmt
+ Imports管理 gopkgs
+ 调试(使用delve)
+ 变量命名规范golint[github.com/golang/lint/golint ]
+ 方法和类的签名帮助信息godoc
+ 工作区符号搜索go-symbols
+ 保存时编译测试go-build gotest
+ 错误检查errcheck[github.com/kisielk/errcheck]
+ 显示文件函数结构体等定义gotags[github.com/jstemmer/gotags]，需要结合tagbar
+ 自动导入包goimports[github.com/bradfitz/goimports]

### 包依赖管理工具godep govender
govendor INSTALL & USAGE
```bash
cd $GOPATH/src && mkdir project-test && cd project-test
go get -u -v github.com/kardianos/govendor # 
govendor init

# 将GOPATH中本工程会用到的依赖包自动移动到vendor目录中
govendor add +external
govendor add +e

# go helper
# go get 添加包到gopath
go get -u -v github.com/a/b
```

## Windows+VSCode配置Go环境
+ 安装VSCode、Go
+ 设置环境变量GOBIN=D:\Go、GOROOT=C:\Go GOPATH=D:\Go;D:\GoProjects
+ VSCode安装
