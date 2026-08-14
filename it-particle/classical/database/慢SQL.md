# 慢SQL定位

+ 使用数据库自带的慢查询日志 slowlog
+ explain 语句分析sql（来自slowlog）
+ 第三方监控工具？datadog
+ 人工分析SQL
    + 缺乏索引
    + 全表扫描
    + 复杂的连接操作
    + 子查询，尽量将子查询改为连接查询或使用临时表
    + 分页查询，避免使用offset，改用where条件
        + offset不用索引，逐步跳过前面的一部分记录非常耗时，尤其是量大之后
+ 检查数据库配置
    + 缓冲区大小
    + 连接数限制
    + 锁设置，锁的等待时间
    + 硬件资源检查，CPU使用率、Mem内存使用率、磁盘IO
+ 数据库数据量
    + 单表数据量过大
    + 单行数据过长
+ 信息系统，
    + 缓存、批量操作、异步处理
    + 定时清理数据、统计数据
    + 事务问题、跨库、排序、分页
    + 分布式ID不合理
    + in 运算


```sql
set global slow_query_log = 'ON';
set global slow_query_time = 1; -- 阈值1秒
set global slow_query_log_file = '/path/to/slowquery.log';

select slow_query_log_file;
```