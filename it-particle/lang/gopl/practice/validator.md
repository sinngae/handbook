# validator

validator 字段注意零值，`binding:"required"`会认为0是空值而不通过校验，解决办法是把整数字段改成整数指针类型，指针默认值为nil