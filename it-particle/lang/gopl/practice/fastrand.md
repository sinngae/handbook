

# go:linkname
```go
//go:linkname FastRand runtime.fastrand
func FastRand() uint32
```
runtime.fastrand和math.Rand都是伪随机数生成器，但不同的是runtime.fastrand是在当前goroutine上下文环境下的，因此在频繁调用过程中不需要加锁，所以，它的性能要比math.Rand要好得多。副作用是？不能并发下使用。
