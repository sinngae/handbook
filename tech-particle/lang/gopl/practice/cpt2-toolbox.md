# 第二章 Golang 工具箱

## 1. go tool
```sh
# 查看go工具箱
go tool
```

## 2. go test
### Go 单元测试
```sh
# 查看go test支持的文件和函数
#   *_test.go
#   TestXxx/BenchworkXxx/ExampleXxx
go help testfunc
# 查看go test详细
go help testflag

# 单元测试
go test [dirs ...]

# 生成单元测试覆盖率报告
cd /path/to/go-project
go test -v ./... -coverprofile=cover.out
# cover.out解析，见go tool cover

# 生成集成测试覆盖率报告（运行服务器端，运行客户端mock）
#   编译二进制test文件
GO111MODULE=on go test -v cmd/server/main_test.go cmd/server/main.go -c -covermode=count -coverpkg `go list ./...|tr \"\\n\" \",\"`. -o server.test
./server.test -test.coverprofile /tmp/cover.out
#  运行客户端mock
```
`go test`命令从_test.go文件中寻找test测试、benchmark基准测试和examples示例函数。
测试函数以TestXxx为函数名，基准测试以BenchmarkXxx为函数名，示例函数以ExampleXxx为函数名。
### 基准测试

### HTTP测试

## 3. go tool cover
```sh
# 查看帮助
go tool cover -help

# 生成html格式
go tool cover -html=cover.out 
```

## 4. go tool pprof

## 5. go vet
go vet是go tool vet的封装，是语法静态分析检查工具，可以检查出package或源代码中的隐含错误，规范项目代码。
一般IDE在每次文件编辑后、编译前、甚至go list ./...(做依赖分析)，都会运行go vet做语法静态分析。
```sh
# 查看可用的分析器、功能选项
 go tool vet help

# 分析某个文件或文件夹
go tool vet [file|file match pattern]
```

## 6. go代码终端调试 -- GDB & Delve
```bash
# GDB 调试 （略）

# Delve
#   安装 https://github.com/go-delve/delve
go get github.com/go-delve/delve/cmd/dlv
```

## * 常见tool
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