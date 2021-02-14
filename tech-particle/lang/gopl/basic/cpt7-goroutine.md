# 第七章 goroutine
Go语言中，程序并发使用两种手段实现，goroutine结合channel、多线程共享内存。
CSP(communicating sequential processes)，顺序通信进程，是一种现代的并发编程模型。
CSP中，值在不同的运行实例（goroutine）间传递（当然，大多数时候、大多数值在单一实例中）。

## 1. goroutines
Go语言中，并发执行单元叫做goroutine，相当于线程。
main函数即在一个单独的goroutine中运行，称为main gorotine，新的goroutine使用go语句创建。

## 2. channel