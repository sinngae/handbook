interface
---
go interface源码在runtime\iface.go中。
go的接口由两种类型实现iface和eface。
iface是包含方法的接口，存储方法表和数据指针;eface（empty interface）不包含方法，存储类型信息和数据指针。

