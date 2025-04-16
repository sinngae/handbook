# ESQL
ESQL，嵌入式SQL，把SQL嵌入到程序设计语言中访问数据库并处理数据。

例如嵌入C语言的ESQL，C语言与ESQL通过**共享变量**来进行传送数据，这样的源程序通过预编译程序转成纯C语言源程序。DBMS提供SQL语言接口，还提供C语言编写的函数，供应用程序调用，一般涉及建立连接/传送SQL/执行SQL/建立游标/返回结果及状态或异常。

在PL/1和C中：`EXEC SQL <sql statement>;`

在COBOL中：`EXEC SQL <sql statement> END-EXEC;`

```c
EXEC SQL INCLUDE SQLCA; // 定义SQL通信区
EXEC SQL BEGIN DECLARE SECTION; // 共享变量说明
    CHAR StuNo(5);
    CHAR ClsNo(3);
    INT Grade;
EXEC SQL END DECLARE SECTION;

main() {
    /* 必须使用游标的sql语句：SELECT多条记录 CURRENT形式的UPDATE CURRENT形式的DELETE */
    EXEC SQL DECLARE C1 CURSOR FOR // 游标定义
        SELECT Sno, Cno, Grade
        FROM SC;
    EXEC SQL OPEN C1;   // 打开游标
    for(;;) {
        EXEC SQL FETCH C1 INTO :StuNo :ClsNo :Grade;    // 推进游标，并读取数据到共享变量
        if (sqlca.sqlcode != SUCCESS) {//判定sqlca状态
            ...
            break;
        }
        printf("%s %s %d", :StuNo, :ClsNo, :Grade);
    }
    EXEC SQL CLOSE C1;  // 关闭游标
}
```

ESQL中变量的数量和类型都是预编译时就确定的，只有值是动态的，这类SQL又称静态SQL。语句、条件、表、查询条件都可变得SQL称为动态SQL。动态SQL更灵活，可以覆盖更多的场景。