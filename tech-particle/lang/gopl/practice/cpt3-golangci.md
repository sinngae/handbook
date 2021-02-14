# 第三章 Golang CI

```sh
# 安装golangci
GO111MODULE=on go get -u github.com/golangci/golangci-lint/cmd/golangci-lint@v1.16.0

# 查看版本
golangci-lint --version

# 运行
golangci-lint run [目录 | 文件名]

# 查看支持的linter
golangci-lint help linters

# 仅运行某个linter
#   -E/--enable     使能某个linter
#   -D/--disable    禁用某个linter
#   --diable-all    禁用所有
golangci-lint run --disable-all -E errcheck

# 导出报告
golangci-lint run --out-format checkstyle ./... > report.xml
```

Enabled by default linters:
* 		deadcode: 发现没有使用的代码
* 		errcheck: 用于检查 go 程序中有 error 返回的函数，却没有做判断检查
*       gosimple: 检测代码是否可以简化
* 		govet (vet, vetshadow): 检查 go 源代码并报告可疑结构，例如 Printf 调用，其参数与格式字符串不一致
* 		ineffassign: 检测是否有未使用的代码、变量、常量、类型、结构体、函数、函数参数等
* 		staticcheck: 提供了巨多的静态检查，检查 bug，分析性能等
* 		structcheck:发现未使用的结构体字段
* 		typecheck: 对 go 代码进行解析和类型检查
* 		unused: 检查未使用的常量，变量，函数和类型
* 		varcheck: 查找未使用的全局变量和常量
Disabled by default linters:
* 		bodyclose: 对 HTTP 响应是否 close 成功检测
* 		dupl: 代码克隆监测工具
* 		gochecknoglobals: 检查 go 代码中是否存在全局变量
* 		goimports: 做所有 gofmt 做的事. 此外还检查未使用的导入
* 		golint: 打印出 go 代码的格式错误
* 		gofmt: 检测代码是否都已经格式化, 默认情况下使用 -s 来检查代码是否简化
* 		…………………………..