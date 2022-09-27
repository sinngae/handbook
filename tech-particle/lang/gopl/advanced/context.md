context
---
context 携带deadline，cancellation signals 和其他的request-scoped的值，穿过API边际和进程之间。

+ 服务的入向请求应该创建一个对应的context，出向的请求应该可以接收一个context。
+ 函数调用链应该传播context，或这使用WithCancel、WithDeadline，WithTimeout、WithValue创建的派生的context。
    + 当一个context被撤销了，所有其派生的context都应该被撤销。
    + CancelFunc 撤销context及其派生的context，并关闭其关联的定时器
+ 不要把context存储到struct中隐式传播，而应该显式地传播（约定）
+ context value仅应用于request范畴的数据，从进程传递到API，而不是函数的参数
+ 同一个context可以传递给不同的协程；它是协程并发安全的。

# 意义与作用
通过约定同一种代码侵入风格，实现请求范畴的值（数据和函数？）和各种业务处理函数的正交效果。
比如打印logger日志，需要有traceId，caller， timestamp等数据，这些只要在context和logger里定义好即可，不用业务处理函数参数列表中逐一增加。

通过WithCancel、WithDeadline，WithTimeout的定时器，实现调用超时控制，而不是依赖于第三方服务。（实际还是会继续运行到结束，但是不影响主流程了）

