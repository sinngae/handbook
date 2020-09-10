# Golang CI
```sh
# 获取golangci
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