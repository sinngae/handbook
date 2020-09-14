# Gopl Tool
```sh
# 查看go工具箱
go tool
```

## test
### Go 单元测试/基准测试/http测试
```sh
# 单元测试
go test [dirs ...]

# 基准测试

# 查看go test详细
go help testflag

# 生成单元测试覆盖率报告
cd /path/to/go-project
go test -v ./... -coverprofile=cover.out
# cover.out解析，见go tool cover

# 生成集成测试覆盖率报告（运行服务器端，运行客户端mock）
GO111MODULE=on go test -v cmd/server/main_test.go cmd/server/main.go -c -covermode=count -coverpkg `go list ./...|tr \"\\n\" \",\"`. -o server.test # 编译二进制test文件
./server.test -test.coverprofile /tmp/cover.out
#  运行客户端mock
```
`go test`命令从_test.go文件中寻找test测试、benchmark基准测试和examples示例函数。
测试函数以TestXxx为函数名，基准测试以BenchmarkXxx为函数名，示例函数以ExampleXxx为函数名。
### 基准测试

### HTTP测试

## cover
```sh
# 查看帮助
go tool cover -help

# 生成html格式
go tool cover -html=cover.out 
```

## tool pprof

## vet
go vet是go tool vet的封装
```sh
# 查看帮助
 go tool vet help

```