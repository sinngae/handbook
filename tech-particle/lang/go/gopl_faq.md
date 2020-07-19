# gopl faq
trouble shoot记录
## 1.go mod invalid pseudo-version
项目的依赖包更新了，go build报错`invalid pseudo-version`，起因是远程包的时间戳和gomod记录的时间戳对不上。
删除本地go.sum重新生成后解决。
