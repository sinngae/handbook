# 包依赖管理
参考：https://goproxy.io/zh/docs/introduction.html
## 1. Go Module
Go 1.11 以后官方推荐使用Go Module.

**GO111MODULE**

"=on"，表示开启 GoModule （不使用$GOPATH/src和vender文件夹，而根据go.mod，使用`go get`下载依赖到$GOPATH/pkg/mod）；
"=off"，表示不开启 GoModule ；
"=auto"，表示当前目录有go.mod时，使用 GoModule ；

install目录依然是GOPATH/bin；mod cache在GOPATH/pkg/mod/cache

go mod 配合`go get package[@version]`使用，默认`go get package@latest`；
version可以是 vx.y.z 这种形式的tag，或者是commit的checksum，或者是  master，或者是 latest；
当version是master时，不管package有没有打tag，都会选择master分支的最新commit；
可以在version前使用>，>=，<，<=，表示选取的版本不超过/不低于version，在这个范围内的符合latest条件的版本；
当想要修改package的版本时，只需要go get package@指定的version即可；

**GOPROXY**

用于控制GoMod下载的来源，用于保证构建的确定性和安全性；

go的包可能来自于任何的VCS仓库，私有部分还需要VCS的身份认证。VCS仓库的包可能被删除、修改，是易变的、不确定的。GoMod下载会重定向到GOPROXY配置的缓存仓库，为不同用户、不同时间、相同模块和版本的请求，提供相同的返回，从而保证多次构建结果一致，还可以起到加速下载的效果。

公共的GOPROXY，如JFrog GoCenter（https://gocenter.io），Goproxy.cn（http://goproxy.cn） 都是免费提供给Golang开发者社区的，缓存了大量开源的Go包。

私有的GOPROXY，用于配置在私有设施上存储公共和私有go包的服务。私有VCS仓库中的模块在http://sum.golang.org 的公共校验和数据库中没有条目，所以它们必须被排除在go客户端的检查之外。将GONOSUMDB设置为私有VCS仓库可以实现这一点，并将防止这些私有模块的go get命令由于校验和不匹配而失败。

在这个配置中，您可以确保对私有模块的引用不会“泄漏”，同时还确保了公共模块和私有模块的不可变性和可用性。

需要跨网络访问包的代理配置；国内访问不了http://golang.org/x/；官方提供了http://proxy.golang.org，也不能访问；可用http://goproxy.cn代替；

",direct"是特殊指令符号，用于指示 Go 回源到模块版本的源地址去抓取 (比如 GitHub 等)，当值列表中上一个 Go module proxy 返回 404 或 410 错误时，Go 自动尝试列表中的下一个，遇见 “direct” 时回源，遇见 EOF 时终止并抛出类似 “invalid version: unknown revision...” 的错误。

**GOPRIVATE**

用于访问私有仓库，如企业内部module或托管站点上的private库（否则会报404错误）；
避免私有仓库的请求，请求到公网而泄漏信息。
可以通过GOPROXY访问某些仓库，但不能访问这些仓库的private库；
GOPRIVATE配置通过git config配置的方式访问私有库；

强制go get通过ssh下载，而不是https
```config
[url "git@github.com:"]
    insteadOf = https://github.com/

[url "git@gitlab.com:"]
    insteadOf = https://gitlab.com/
```
GONOPROXY和GONOSUMDB均为none，意味着所有module，不管是公共的还是私有的，都要经过proxy下载，经过sumdb验证；

```bash
# 创建并初始化go.mod，用于管理此项目的依赖包的路径和版本
go mod init .

# 创建vender文件夹，并拷贝依赖包
go mod vendor

# 显示所有import库
go list -m -json all

#   使用本地依赖包
#       require (
#           mytest v0.0.0
#       )
#       replace (
#           mytest v0.0.0 = ../mytest    
#       )
```

## 2. 其他
现在大多项目都是用go module，此章节仅做了解，可以跳过。

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

# vgo
go get -u golang.org/x/vgo
vgo version
vgo install # 读取go.mod文件，安装依赖
vgo build   # 编译项目
vgo run     # 运行项目
vgo get     # github.com/gin-gonic/gin获取依赖包的最新版本
vgo get github.com/gin-gonic/gin@v1.2 # 获取依赖包的指定版本
vgo mod -vendor # 将依赖包直接放在项目的vendor目录里

```