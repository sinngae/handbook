# Go GC（三色标记并发GC）四大触发时机（源码 `mgc.go` 定义的 3 种自动 + 手动）
> 承接前面GMP：**GC触发检查，发生在堆内存分配的时候（`mallocgc`）；定时触发由 sysmon 负责**，刚好串起刚才聊的sysmon。

## 1. 堆阈值触发（最常见，gcTriggerHeap）
由 `GOGC`（`debug.SetGCPercent`）控制，默认值=100
> 公式：**下一次GC触发阈值 = 上一轮GC结束后的存活堆大小 × (1 + GOGC/100)**

例子：
上一轮GC完成后，存活对象是 100MB，GOGC=100
→ 当堆增长到 `100 × (1+100/100) = 200MB`，触发新一轮GC

- `GOGC=50`：堆涨50%就GC，GC更频繁，内存占用低，CPU开销变大
- `GOGC=200`：堆涨20%才GC，GC次数变少，内存峰值更高
- `GOGC=off`：关闭自动GC，只可以手动 `runtime.GC()`

> 注意：**不是总内存，是live堆**，runtime会动态计算，不是简单固定翻倍；Go1.19+引入`GOMEMLIMIT`，内存硬上限，超过会强制GC。

## 2. 定时强制触发（gcTriggerTime，sysmon负责）
常量 `forcegcperiod = 2 min`
> 如果**连续2分钟，一直没有满足堆阈值，没有跑过GC**，sysmon后台协程强制启动一次GC

场景：程序长期低分配，堆很小，一直达不到GOGC阈值，防止终结器、长期不回收的资源一直不清理。
👉 **这个定时GC，就是sysmon做的！和sysmon抢占G是同一个后台M**

## 3. GOMEMLIMIT 内存限制触发（Go1.19新增）
`GOMEMLIMIT` 设置进程可使用内存上限（包含堆、栈等）。
当runtime预估内存逼近这个上限，**无视GOGC，主动触发GC**，避免OOM。
> 适合容器环境，限制cgroup内存，防止进程被内核kill。

## 4. 手动触发（gcTriggerCycle）
代码主动调用 `runtime.GC()`，阻塞调用G直到本轮GC完成。
- 生产环境一般不要用；仅压测、调试场景使用
- 调用后强制启动一轮完整GC，做标记+清扫

# 源码里判断入口
每次堆分配 `mallocgc` 函数末尾，都会判断 `gcShouldStart()`，检查上面条件，满足就启动GC。

## GC启动时，GMP怎么配合（串联你前面GMP知识）
1. 触发GC时，需要STW（stop the world），**短暂暂停所有P上运行的G**；
2. GC本身会启动多个G（mark worker），绑定P并发标记；
3. 还有辅助GC（mutator协助标记）：分配内存的用户G，如果GC标记没干完，分配的时候顺便帮GC干活；
4. sysmon除了抢占，还负责定时GC、监控GC进度。

# 常见误区
❌ 不是堆内存满了才GC；是**相对上一轮存活堆的增长比例**
❌ GC不是单独OS线程一直跑；GC的mark worker是普通goroutine，由GMP调度
✅ sysmon只做**检查、触发**；真正标记工作交给worker G

## 简单总结一句话
**正常情况：堆内存增长达到GOGC阈值触发；兜底：sysmon每2分钟强制GC；容器环境受GOMEMLIMIT限制；代码可手动runtime.GC()**

如果你想，我继续讲：**GC三色标记、写屏障，还有STW在GC两个阶段分别做了什么**