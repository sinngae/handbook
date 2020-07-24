# Gopl Comment
golint检查注释

```go
// 一般注释
/* 一般注释 */

//Go语言惯例：以下需要添加注释：对外公开的包、函数、常量、变量。
// godoc将包中.go文件中包级别的注释文本生成包的文档

// +build linux,386 // 条件编译标签（单独一行或多行均以// +build开头，和其他代码或注释通过空行隔开）
// +build ignore // 完全忽略架构或操作系统

//go:binary-only-package // 二进制包 $GOPATH/pkg/下搜索，适用于企业核心包，有一定壁垒或敏感的包的发布

//go:generate command argument...
// 同go tool中的generate，执行go generate，会自动检索go代码中这样的命令，并依次执行
// 生成的代码文件带有符合如此正则表达的注释：^// Code generated .* DO NOT EDIT\.$

/*
C code
*/
import "C"
// cgo代码注释
```