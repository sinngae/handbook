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

### 包依赖管理工具
```bash
# Go 1.11 以前，包管理工具有 govendor/godep
INSTALL & USAGE
cd $GOPATH/src && mkdir project-test && cd project-test
go get -u -v github.com/kardianos/govendor # 
govendor init
#   将GOPATH中本工程会用到的依赖包自动移动到vendor目录中
govendor add +external
govendor add +e
#   go helper
#   go get 添加包到gopath
go get -u -v github.com/a/b

# Go 1.11 以后官方推荐使用vgo
go get -u golang.org/x/vgo
vgo version
vgo install # 读取go.mod文件，安装依赖
vgo build   # 编译项目
vgo run     # 运行项目
vgo get     # github.com/gin-gonic/gin获取依赖包的最新版本
vgo get github.com/gin-gonic/gin@v1.2 # 获取依赖包的指定版本
vgo mod -vendor # 将依赖包直接放在项目的vendor目录里

# Go mod
#  用环境变量GO111MODULE开启或关闭mod支持，有off/on/auto三种模式
#   默认auto，在GOPATH/src外面且根目录有go.mod时，开启模块支持
#   off，go会从GOPATH和vender文件夹中寻找包、下载包
#   on，go会忽略GOPATH和vender文件夹，只根据go.mod下载依赖到GOPATH/pkg/mod；install目录依然是GOPATH/bin；mod cache在GOPATH/pkg/mod/cache
go mod init . # 创建并初始化go.mod，用于管理此项目的依赖包的路径和版本
go mod vendor # 创建vender文件夹，并拷贝依赖包
go list -m -json all # 显示所有import库
#   使用本地依赖包
#       require (
#           mytest v0.0.0
#       )
#       replace (
#           mytest v0.0.0 = ../mytest    
#       )
#
```

## Windows+VSCode配置Go环境
+ 安装VSCode、Go
+ 设置环境变量GOBIN=D:\Go、GOROOT=C:\Go GOPATH=D:\Go;D:\GoProjects
+ VSCode安装
