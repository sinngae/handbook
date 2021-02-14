# Golang 实践
Go语言具备良好的特性、丰富的library库、便捷的工具集，非常适合服务器端开发。
+ GOROOT Go的安装目录;$GOROOT/bin需要配置到$PATH中；
+ GOPATH 指定开发工作区，存放源代码、测试文件、库文件、二进制文件等。换开发工作区，需要重新修改该变量。
+ GOBIN 开发程序编译后二进制命令的安装目录（go install的目标路径）
+ GOOS
+ GOARCH

## [第一章 包管理](cpt1-pkgmgr.md)
1. Go Module
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

