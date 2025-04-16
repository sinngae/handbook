go data race
---

Go程序运行时的竞争监测（Data race）

```sh
go test -race ./main
go run -race main.go
go build -race main
go install -race mypkg
```
将会打印处竞态数据和竞争的协程。