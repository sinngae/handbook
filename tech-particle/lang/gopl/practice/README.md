# Golang 实践
Go语言具备良好的特性、丰富的library库、便捷的工具集，非常适合服务器端开发。

**GOROOT**

Go的标准库、编译工具链、标准工具的安装目录；

$GOROOT/bin需要配置到$PATH中；

**GOPATH**

Go的工作目录，可以是多个路径；指定Go编译工具链等工具寻找Go的包、可执行程序的路径；

**GOBIN**

开发程序编译后二进制命令的安装目录（go install的目标路径）

**GOOS**

当前操作系统类型

**GOARCH**

当前操作系统架构

## [第一章 包管理](cpt1-pkgmgr.md)
1. [Go Module](cpt1-pkgmgr.md)
2. 其他


## [第二章 工具箱](cpt2-toolbox.md)
1. go tool
2. go test
    - 单元测试
    - 基准测试
    - HTTP测试
3. go tool cover
4. go tool pprof
5. go vet
* 常见tool

## [第三章 golangci](cpt3-golangci.md)

