## Gorm
 https://gorm.io/docs/
Open返回一个连接对象，具体连接发生在执行过程
Rows对象需要显示close，否则容易出现goroutine泄漏。正常情况Row.Next()遇到错误和没有记录会close，但是提前退出或者返回，不会close
接收变量为struct，检索到0条记录，会抛出ErrRecordNotFound错误，如果是slice，则不会
更新记录使用map[string]interface, struct只会更新非零值
当使用struct查询时，只会查询那些具有值的字段
检查主键，避免SQL注入