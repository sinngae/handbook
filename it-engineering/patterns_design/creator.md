# 创建型模式

## Factory
## Abstract Factory
## 单例 Singleton

减少程序启动时间和不必要的资源开销
+ 并发场景的安全问题
+ 多次重入的成本

双重校验锁
```C
func once() {
    if inst == nil {
        lock.Lock()
        if inst == nil { // 初始化阶段的多个流程执行到这里
            inst.new()
        }
        lock.unlock()
    }
    return inst
}
```


## Builder
## 原型 Prototype