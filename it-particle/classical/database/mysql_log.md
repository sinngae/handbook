# MySQL有几种日志
MySQL 的日志体系可以从Server 层和存储引擎层两个维度来划分，共有 8 种核心日志：

## 一、Server 层日志（MySQL 通用）
表格
日志	作用	默认状态
错误日志 (Error Log)	记录启动、运行、停止过程中的错误和警告信息	✅ 默认开启
二进制日志 (Binary Log / Binlog)	记录所有数据变更操作（DDL + DML），用于主从复制和数据恢复	❌ 默认关闭
慢查询日志 (Slow Query Log)	记录执行时间超过 long_query_time 的 SQL，用于性能优化	❌ 默认关闭
通用查询日志 (General Query Log)	记录所有客户端连接和执行的 SQL（含 SELECT）	❌ 默认关闭
中继日志 (Relay Log)	主从复制时，从库暂存主库 Binlog 的日志	从库自动开启

## 二、InnoDB 存储引擎层日志
表格
日志	作用	特点
重做日志 (Redo Log)	保证事务的持久性（D），崩溃恢复时重做未落盘的数据	循环写，物理日志，固定大小
回滚日志 (Undo Log)	保证事务的原子性（A），支持 MVCC 和事务回滚	逻辑日志，清理由 Purge 线程负责
DDL 日志 (DDL Log)	记录 DDL 操作过程中的元数据变更，用于崩溃恢复时回滚未完成的 DDL	隐藏日志，通常无需关注

## 三、快速记忆口诀
"错二慢通中，重回 D"
错：错误日志
二：二进制日志
慢：慢查询日志
通：通用查询日志
中：中继日志
重：重做日志
回：回滚日志
D：DDL 日志

## 四、重点对比：Binlog vs Redo Log
表格
维度	Binlog (Server 层)	Redo Log (InnoDB 层)
用途	主从复制、数据恢复	崩溃恢复
记录内容	逻辑 SQL / Row 变更	物理页修改
写入方式	追加写，可无限增长	循环写，固定大小（innodb_log_files_in_group）
事务关系	事务提交后一次性写入	事务执行中持续写入
刷盘策略	sync_binlog 控制	innodb_flush_log_at_trx_commit 控制

## 五、查看日志相关配置
```sql
-- 查看所有日志状态
SHOW VARIABLES LIKE '%log%';

-- 查看 Redo Log 配置
SHOW VARIABLES LIKE 'innodb_log%';

-- 查看 Binlog 状态
SHOW VARIABLES LIKE 'log_bin';
SHOW MASTER STATUS;
```

一句话总结：MySQL 有 8 种日志，Server 层 5 种（错误、Binlog、慢查询、通用查询、中继日志），InnoDB 层 3 种（Redo、Undo、DDL）。日常运维最关注的是 Error Log、Binlog、Slow Log 和 Redo Log。

# 有几个日志文件
MySQL 的日志不是固定"几个文件"，不同日志类型对应的文件数量和形态差异很大。

以下是具体说明：
## 一、各日志对应的文件情况
表格
日志类型	文件形态	典型文件数量	文件命名/位置
错误日志	单个文件	1 个	hostname.err 或自定义路径
二进制日志 (Binlog)	多个滚动文件	动态增长	binlog.000001、binlog.000002…
慢查询日志	单个文件	1 个	hostname-slow.log
通用查询日志	单个文件或表	1 个（文件）或 2 个表	hostname.log 或 mysql.general_log 表
中继日志 (Relay Log)	多个滚动文件	动态增长	relay-log.000001、relay-log.000002…
Redo Log	固定数量循环文件	默认 2 个（可配置）	ib_logfile0、ib_logfile1
Undo Log	表空间内部分段	不单独成文件	位于 ibdata1 或独立的 Undo 表空间文件中
DDL 日志	隐藏文件	1 个	ddl_log.log（通常不可见）

## 二、重点说明"多文件"场景
### 1. Binlog —— 最常见的多文件日志
```bash
# 典型 Binlog 文件列表
binlog.000001   # 第 1 个日志文件
binlog.000002   # 第 2 个日志文件
binlog.000003   # 第 3 个日志文件
binlog.index    # 索引文件，记录所有 Binlog 文件列表
```
单个文件大小上限由 max_binlog_size 控制（默认 1GB）
文件数量随写入量增长，需定期清理（PURGE BINARY LOGS）

### 2. Redo Log —— 固定数量的循环文件
```bash
ib_logfile0     # 第 1 个 Redo Log 文件
ib_logfile1     # 第 2 个 Redo Log 文件
# 数量由 innodb_log_files_in_group 控制（默认 2，建议 2-4）
```
总大小 = innodb_log_file_size × innodb_log_files_in_group
写满后循环覆盖，不会无限增长

### 3. Undo Log —— 无独立文件（特殊）
MySQL 8.0 之前：Undo 数据在 ibdata1 共享表空间中
MySQL 8.0 之后：可配置独立 Undo 表空间（undo_001、undo_002），但仍不是传统意义上的"日志文件"

## 三、查看当前实际日志文件
```sql
-- 查看 Binlog 文件列表及大小
SHOW BINARY LOGS;

-- 查看当前正在写入的 Binlog 文件
SHOW MASTER STATUS;

-- 查看 Redo Log 配置（文件大小和数量）
SHOW VARIABLES LIKE 'innodb_log_file%';
SHOW VARIABLES LIKE 'innodb_log_files_in_group';

-- 查看错误日志路径
SHOW VARIABLES LIKE 'log_error';

-- 查看慢查询日志路径
SHOW VARIABLES LIKE 'slow_query_log_file';
```

## 四、总结
表格
问法	答案
"MySQL 有几种日志？"	8 种（按功能分类）
"MySQL 有几个日志文件？"	不固定：Redo Log 固定 2 个，Binlog/Relay Log 动态多个，其余通常各 1 个
磁盘占用会无限增长的是	Binlog 和 慢查询日志/通用日志（需定期清理）
磁盘占用固定的是	Redo Log（循环覆盖）

如果你是想清理磁盘空间，重点关注 binlog.* 文件和 mysql-slow.log，这两个是最常见的"空间杀手"。