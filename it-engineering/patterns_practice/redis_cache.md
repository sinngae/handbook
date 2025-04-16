Redis Cache
----
redis + mysql 模式，redis用做缓存，提高mysql的查询性能。

## 缓存击穿问题

## 数据一致性问题


#### 先更新MySQL，再删除redis key，mysql 多次更新，redis命中幻读
延迟二删，增加第二次删除redis key 并延迟200ms/500ms。


