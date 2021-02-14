
# 包依赖管理

## 1. Go Module
Go 1.11 以后官方推荐使用Go Module.

用环境变量GO111MODULE开启或关闭mod支持，有off/on/auto三种模式：
- 默认auto，在GOPATH/src外面且根目录有go.mod时，开启模块支持
- off，go会从GOPATH和vender文件夹中寻找包、下载包
- on，go会忽略GOPATH和vender文件夹，只根据go.mod下载依赖到GOPATH/pkg/mod；

install目录依然是GOPATH/bin；mod cache在GOPATH/pkg/mod/cache

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