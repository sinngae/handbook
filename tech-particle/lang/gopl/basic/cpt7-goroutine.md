第七章 goroutine
---
Go语言中，程序并发使用两种手段实现，goroutine结合channel、多线程共享内存。
CSP(communicating sequential processes)，顺序通信进程，是一种现代的并发编程模型。
CSP中，值在不同的运行实例（goroutine）间传递（当然，大多数时候、大多数值在单一实例中）。

# 1. goroutines
Go语言中，并发执行单元叫做goroutine。
main函数即在一个单独的goroutine中运行，称为main gorotine，新的goroutine使用go语句创建。

# 如何控制并发
1. 使用sync.WaitGroup实现
```go
wg := sync.WaitGroup{}
ch := make(chan struct{}, count) // ch 控制并发数量
for ...
    wg.Add(1) // 一定是同步地Add计数，不然就可能漏掉
    ch<-struct{}{}
    go func() {
        defer wg.Done()
        // do something
        <-ch
    }

wg.Wait()
```

2. panjf2000/ants(第三方库，池化)
```go
import (
	"log"
	"time"

	"github.com/Jeffail/tunny"
)
func main() {
	pool := tunny.NewFunc(10, func(i interface{}) interface{} {
		log.Println(i)
		time.Sleep(time.Second)
		return nil
	})
	defer pool.Close()

	for i := 0; i < 500; i++ {
		go pool.Process(i)
	}
	time.Sleep(time.Second * 4)
}
```
ants是一个受fasthttp启发的高性能协程池，fasthttp号称是比go原生的net/http快10倍，其原因之一就是采用了各种池化技术， ants相比之前两种协程池，其模型更像是之前接触到的数据库连接池，需要从空余的worker中取出一个来执行任务, 当无可用空余worker的时候再去创建，而当pool的容量达到上线之后，剩余的任务阻塞等待当前进行中的worker执行完毕将worker放回pool, 直至pool中有空闲worker。 ants在内存的管理上做得很好，除了定期清除过期worker(一定时间内没有分配到任务的worker)，ants还实现了一种适用于大批量相同任务的pool, 这种pool与一个需要大批量重复执行的函数锁绑定，避免了调用方不停的创建，更加节省内存。

```go

```