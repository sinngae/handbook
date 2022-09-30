# SQL

## basis

### 表的连接/笛卡尔积
```sql
-- 非标准早期表连接
select * from tbl1, tbl2 where tbl1.clm0 = tbl2.clm0;

-- 笛卡尔积
select * from tbl1 inner join tbl2 on tbl1.clm0 = tbl2.clm0;
--  left join/right join/outer join
```

## SQL92


## SQL99