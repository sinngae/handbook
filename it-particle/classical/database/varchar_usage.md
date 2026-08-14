# varchar 用法注意事项

## 一、对业务的影响
表格
场景	后果
用户注册/昵称	用户输入合法内容被拦截，体验极差
地址/备注	地址被截断，快递送不到
JSON/URL 存储	数据损坏，解析失败
多语言内容	中文字符占 3 字节（UTF-8），英文占 1 字节，容易误判
表情符号	😀 占 4 字节（UTF-8mb4），VARCHAR(1) 都存不下

## 二、常见误区：VARCHAR 长度 ≠ 性能
很多人以为 VARCHAR(10) 比 VARCHAR(255) 省空间、更快——这是错的：
+ 存储空间：VARCHAR 只存实际内容 + 1~2 字节长度前缀，与声明长度无关
+ 内存使用：排序/临时表时可能按声明长度分配内存（MySQL < 8.0.1），但过短不会优化，反而导致截断
+ 索引限制：InnoDB 单索引最大 767 字节（旧版本）或 3072 字节，这才是限制索引长度的原因
所以不要为了"性能"故意把 VARCHAR 设得很短，该多长就多长。

## 四、设置建议
表格
字段类型	建议长度	说明
用户名/昵称	VARCHAR(50) ~ VARCHAR(100)	考虑国际化
邮箱	VARCHAR(255)	RFC 标准最长 254 字符
手机号	VARCHAR(20)	含国家码、分隔符
地址	VARCHAR(500) 或 TEXT	地址可能很长
URL	VARCHAR(2048)	标准 URL 上限
备注/描述	VARCHAR(500) 或 TEXT	超过 500 用 TEXT
中文内容	按字符数估算 × 1.5 安全系数	不要按字节数反推

## 五、如何安全修改
```sql
-- 1. 先查看当前字段最大实际长度
SELECT MAX(LENGTH(name)) FROM users;

-- 2. 修改字段长度（Online DDL，不锁表）
ALTER TABLE users MODIFY COLUMN name VARCHAR(100) NOT NULL;

-- 3. 如果是大表，建议用 pt-online-schema-change 或 gh-ost 避免锁表
```

## 六、一句话总结
VARCHAR 设置过短不会提升性能，只会导致插入失败或数据静默截断。宁可稍长（如 VARCHAR(255)），也不要刚好卡死业务需求的最大值。