Redis Eviction
----
# redis的淘汰策略
Redis 的淘汰策略（Eviction Policy）决定了当内存达到 maxmemory 上限时，Redis 如何选择并删除键来释放空间。

## 8 种淘汰策略
- 策略	作用范围	淘汰逻辑	适用场景
+ noeviction	—	不淘汰任何数据，直接返回写操作错误	不允许数据丢失的场景（默认策略）
+ allkeys-lru	所有键	淘汰最近最少使用的键	通用缓存，符合局部性原理
+ allkeys-lfu	所有键	淘汰使用频率最低的键	需要区分热点与冷门数据的缓存
+ volatile-lru	仅有过期时间的键	淘汰最近最少使用的键	部分数据需要持久保留，部分可淘汰
+ volatile-lfu	仅有过期时间的键	淘汰使用频率最低的键	同上，但更注重访问频率
+ allkeys-random	所有键	随机淘汰	所有键访问概率均匀分布
+ volatile-random	仅有过期时间的键	随机淘汰	带过期时间的键访问均匀
+ volatile-ttl	仅有过期时间的键	淘汰剩余生存时间最短（即将过期）的键	希望尽快释放即将过期的数据

## 核心概念
- LRU vs LFU
+ LRU（Least Recently Used）：看"多久没访问"，适合时间局部性强的场景（如用户最近浏览的商品）。
+ LFU（Least Frequently Used）：看"访问次数多少"，适合长期热点识别（如热门文章），但存在"历史包袱"问题（过去很热门但现在已经过时的数据）。

## 配置方式
```bash
# redis.conf
maxmemory 256mb
maxmemory-policy allkeys-lru
```
或运行时动态修改：
```bash
CONFIG SET maxmemory-policy allkeys-lfu
```

## 近似实现
Redis 使用的是近似 LRU/LFU 而非精确实现：
+ 每个键对象头部有一个 24 位的时钟字段，记录最后一次访问时间。
+ 采样（默认随机取 5 个键）并淘汰其中最久未访问/频率最低的，而非全局排序。
+ 通过 maxmemory-samples 配置采样数量（增大更精确但消耗更多 CPU）。

## 选择建议
+ 通用缓存：allkeys-lru（最常用）
+ 区分冷热数据：allkeys-lfu（Redis 4.0+）
+ 混合存储（部分必须持久）：volatile-lru 或 volatile-lfu，并确保重要键不设置过期时间
+ 临时数据为主：volatile-ttl，让 Redis 优先清理快过期的数据

## 注意事项
+ noeviction 是 Redis 3.0 之前的默认策略，3.0 后部分版本默认改为 noeviction，生产环境务必显式配置。
+ 只有 volatile-* 策略会考虑过期时间，如果所有键都没设置 TTL，这些策略效果等同于 noeviction。
+ 淘汰策略只会在写入操作触发内存检查时执行，纯粹的读取不会触发淘汰。

# 随机淘汰的底层实现
Redis 的随机淘汰策略（allkeys-random / volatile-random）的"随机"非常直接——**不采样、不比较、不排序，直接从哈希表里随机抽一个键就淘汰**。

## 具体实现
从源码层面看，当触发淘汰时，Redis 会执行：
```c
de = dictGetRandomKey(dict);
bestkey = dictGetKey(de);
```
这里和 LRU/LFU 有本质区别：LRU/LFU 会采样 maxmemory-samples 个键（默认 5 个）放入淘汰池比较，而 ***random 策略直接调用 dictGetRandomKey 一次，抽到一个就删**。

### dictGetRandomKey 怎么随机？
Redis 的字典底层是哈希表（hash table），随机选键的过程是：
+ 随机选一个桶（bucket）：在哈希表数组中随机生成一个索引。
+ 处理冲突：如果该桶为空，会重新随机或顺序找下一个非空桶。
+ 随机选节点：如果该桶有链表（哈希冲突），在该链表中随机选一个节点返回。
+ 也就是说，随机性来自哈希表级别的均匀随机，每个键被抽中的概率在理想情况下是均等的。

## 两种 random 策略的区别
- 策略	随机范围	底层字典
+ allkeys-random	所有键	server.db[i].dict（主键空间）
+ volatile-random	仅设置了过期时间的键	server.db[i].expires（过期字典）

## 关键特点
不依赖 maxmemory-samples：这个参数只对 LRU/LFU/ttl 策略有意义，random 策略不需要它。
+ O(1) 复杂度：随机抽一个键是常数时间，比 LRU/LFU 的采样+排序快得多。
+ 真正的"瞎删"：不区分热点冷点，热门键和冷门键被删的概率完全一样。

## 为什么生产环境很少用？
因为随机淘汰不保护热点数据。一个刚被高频访问的键，和一个一年没人碰的键，被踢出去的概率是一样的。所以 random 策略只适用于所有键价值完全均等、或者完全不在乎丢什么数据的场景。