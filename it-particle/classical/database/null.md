# Null值判断，会触发MySQL全表扫描吗
不一定，取决于 MySQL 版本、索引设计 和 数据分布。不能一概而论说"判断 NULL 就会全表扫描"。
## 一、核心结论
表格
条件	IS NULL	IS NOT NULL
MySQL 5.7 及更早	可走索引 ✅	通常全表扫描 ❌
MySQL 8.0.13+	可走索引 ✅	可走索引 ✅（大幅优化）
无索引	必然全表扫描 ❌	必然全表扫描 ❌
有索引但 NULL 占比极高	可能全表扫描 ⚠️	可能全表扫描 ⚠️

## 二、验证示例
1. MySQL 8.0 — IS NULL 走索引
```sql
CREATE TABLE users (
    id INT PRIMARY KEY,
    email VARCHAR(100),
    deleted_at TIMESTAMP NULL,
    INDEX idx_deleted (deleted_at)
);

EXPLAIN SELECT * FROM users WHERE deleted_at IS NULL;
```
执行计划（8.0）：
```plain
+----+-------------+-------+------------+------+---------------+-------------+---------+-------+------+----------+-----------------------+
| id | select_type | table | partitions | type | possible_keys | key         | key_len | ref   | rows | filtered | Extra                 |
+----+-------------+-------+------------+------+---------------+-------------+---------+-------+------+----------+-----------------------+
|  1 | SIMPLE      | users | NULL       | ref  | idx_deleted   | idx_deleted | 5       | const | 100  | 100.00   | Using index condition |
+----+-------------+-------+------------+------+---------------+-------------+---------+-------+------+----------+-----------------------+
```
type = ref：使用了索引
key = idx_deleted：命中索引

2. MySQL 8.0 — IS NOT NULL 也走索引
```sql
EXPLAIN SELECT * FROM users WHERE deleted_at IS NOT NULL;
```
执行计划（8.0.13+）：
```plain
+----+-------------+-------+------------+-------+---------------+-------------+---------+------+------+----------+-----------------------+
| id | select_type | table | partitions | type  | possible_keys | key         | key_len | ref  | rows | filtered | Extra                 |
+----+-------------+-------+------------+-------+---------------+-------------+---------+------+------+----------+-----------------------+
|  1 | SIMPLE      | users | NULL       | range | idx_deleted   | idx_deleted | 5       | NULL | 500  | 100.00   | Using index condition |
+----+-------------+-------+------------+-------+---------------+-------------+---------+------+------+----------+-----------------------+
```
type = range：索引范围扫描
在 5.7 中这条通常会退化为全表扫描（ALL）

## 三、什么情况下会触发全表扫描？
1. 没有合适的索引
```sql
-- email 字段无索引
SELECT * FROM users WHERE email IS NULL;  -- 必然 ALL（全表扫描）
```

2. NULL 值占比过高（索引失效）
如果表中 95% 的 deleted_at 都是 NULL，优化器可能认为走索引不如直接全表扫描快：
```sql
-- 数据倾斜严重时，优化器可能放弃索引
EXPLAIN SELECT * FROM users WHERE deleted_at IS NULL;
-- 可能显示 type = ALL，因为优化器估算全表更快
```

3. 索引无法覆盖查询（回表成本过高）
```sql
-- 索引只有 deleted_at，但 SELECT * 需要回表查所有字段
-- 如果符合条件的行数很多，优化器可能选择全表扫描
SELECT * FROM users WHERE deleted_at IS NULL;
```

## 四、NULL 在索引中的存储原理
InnoDB：NULL 值会被存储在索引的 B+ 树中，占 1 字节（或作为 bitmap 的一部分）
索引前缀：复合索引中，NULL 值可以参与最左前缀匹配
唯一索引：MySQL 允许多个 NULL 值（NULL != NULL），但 8.0.16+ 可通过函数索引实现"唯一非 NULL"

## 五、最佳实践
1. 需要频繁判断 NULL 的字段，务必加索引
```sql
-- 软删除场景，deleted_at 经常被 IS NULL / IS NOT NULL 查询
CREATE INDEX idx_deleted_at ON orders(deleted_at);
```

2. 考虑用特殊值替代 NULL（争议但有效）
```sql
-- 如果业务允许，用 '1970-01-01' 或 0 替代 NULL
-- 好处：避免 NULL 的特殊语义，索引效率更稳定
-- 坏处：失去 NULL 的语义表达能力，需配合 CHECK 约束
```

3. 复合索引中把可能为 NULL 的字段放后面
```sql
-- 如果 status 是 NOT NULL，deleted_at 可为 NULL
CREATE INDEX idx_status_deleted ON orders(status, deleted_at);
-- 这样 status = 1 AND deleted_at IS NULL 可以完美走索引
```

4. 使用 EXPLAIN 验证，不要凭经验
```sql
EXPLAIN FORMAT=JSON SELECT * FROM users WHERE deleted_at IS NOT NULL;
-- 关注 type、key、rows、filtered 字段
```

## 六、一句话总结
IS NULL 和 IS NOT NULL 本身不会触发全表扫描——没有索引、数据极度倾斜、或版本太旧（< 8.0.13）才会。
现代 MySQL 中，只要字段有索引且选择性合理，NULL 判断完全可以高效走索引。