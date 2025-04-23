DSL查询
----
+ 简单查询
    + doc val查询
    + filter查询
+ 全文查询 match
+ term查询 term
+ 范围查询 range
+ 布尔查询 bool， must should must_not
+ 聚合查询 aggs
    + 平均值、最大值、最小值
    + 分组，时间


排序 sort；分页

## match 

## term


# aggs
+ 指标聚合 metrics， 最大、最小、平均值、求和、计数、统计、百分比
+ 桶聚合 bucket
    + terms\
    + filter\filters，指定一个过滤条件，或分组成多个过滤条件
    + range，指定数值类型字段、指定多个范围
    + histogram，指定数值类型字段、指定间隔
    + date_range\date_histogram
        + fixed_time，时长周期
        + calendar_time，日历周期
    + ip_range
    + geo
    + missing
+ 管道聚合 pipeline（忽略）
+ 矩阵聚合 matrix（忽略）

# 分页

## from size

## scroll id

## 