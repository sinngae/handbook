Advenced
---
## setnx
`setnx key val` // set if not exist
+ key不存在，设置成功，返回1
+ key存在，设置失败，返回0

`exists key` // job 不存在，返回0；否则返回1；
`get key` // 读取key

1. `setnx lock.demo <current unix time + lock timeout + 1>`，基于当前时间设置过期时间
    + 返回1，获得锁，加锁成功
    + 返回0，正在被其他客户端锁定
2. 加锁失败，`get lock.demo` 查看是否过期
    + 没有过期，过一段时间重试
    + 过期，`getset lock.demo <...>`，新过期时间（返回SET之前的时间戳）
        + 如果旧的时间戳已经过期，表示加锁成功
        + 如果旧时间戳未过期，代表加锁失败，重新等待重试
3. 解锁：`del lock.demo`


## pipeline
批量命令提交，批量结果返回

## 事务
一次性、顺序性、排他性的执行一个队列中的一系列命令
