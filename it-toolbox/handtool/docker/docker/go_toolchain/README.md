# go toolchain

## protoc
protoc 命令来自 https://github.com/google/protobuf （已被 google.golang.org/protobuf 替代），用于生成序列化和反序列化的代码，无go相关代码。

protoc-gen-go插件 来自于 https://github.com/golang/protobuf/protoc-gen-go ， 可以产生go相关代码， 除上述序列化和反序列化代码之外， 还增加了一些通信公共库。

grpc 来自于 https://github.com/grpc/grpc-go , protoc和protoc-gen-go这两个工具都不在其中，所以玩grpc的时候，少不了protoc和protoc-gen-go.

编译方法分别为：
```shell
protoc --go_out=./go1/ ./proto/my.proto  
protoc --go_out=plugins=grpc:./go2/  ./proto/my.proto
```
### --go_out: protoc-gen-go: plugins are not supported; use 'protoc --go-grpc_out=...' to generate gRPC

## python proto
```shell
pip install grpcio-tools==1.4.0
# 生成文档
go get -u github.com/pseudomuto/protoc-gen-doc/cmd/protoc-gen-doc
```
``` protobuf
# go-proto-validators 是非官方扩展，全局扩展字段可能出现冲突
#   所以fork最新版本到代码库后，修改原协议字段号如下
extend google.protobuf.FieldOptions {
  optional FieldValidator field = 650200;
}
extend google.protobuf.OneofOptions {
  optional OneofValidator oneof = 650210;
}
```

## 引用
https://developers.google.com/protocol-buffers/docs/gotutorial

https://github.com/golang/protobuf/releases#v1.4-generated-code

https://pkg.go.dev/github.com/golang/protobuf