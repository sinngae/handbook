Gin
---

Gin 一个Go的HTTP Web框架，具有类似Martini的API，性能比Martini快40倍。

+ 零分配路由
+ 最快的路由器和框架
+ 单元测试支持 ？

# httprouter
基于radix树的路由，小内存占用。没有反射。

+ Radix树路由，参数化路由、RESTful路由
+ 中间件支持，洋葱模型、可扩展，Logger\Authorization\Gzip\recovery\
+ crash处理（具体为一个中间件），捕获HTTP请求处理中的panic并recover（上报sentry 或者 CAT）
+ Json绑定解绑，marshalbind，统一解析并验证Json，检验非空、检验可空、值域范围、
+ 路由组，可以多层嵌套，性能依然较好
+ 错误管理？
+ 内置渲染、模板渲染，Json\XML\HTML\ ...渲染，提供了统一处理
+ 插件系统，扩展框架（中间件？）
+ binding，绑定和解析，绑定表单数据、uri参数、query、body
+ validator，format、枚举、值域
+ 自定义，HTTP配置（超时、端口、header最大长度）、日志输出格式、中间件、验证器

## Radix树
基数树，

## jsoniter
适用jsoniter编译
`go build -tags=jsoniter`

# 部署
配置
+ Port, 启动Gin服务所监听的TCP端口
+ GIN_MODE, debug/release/test，管理是否输出调试信息，也可在代码中适用gin.SetMode(gin.TestMode)
+ 

# 依赖反转 wire


# 特性

## HTTP2 server推送
用户访问地址后，服务段主动推送静态资源

## JSONP
json with padding，一种为了解决跨域问题的方案，可以跨域读取数据。
利用浏览器同源策略在HTML标签`<script>`的例外（这个标签可以访问不同于当前源的数据并解析），这种模式就是JSONP。jsonp读取应答数据不是json而是任意的javascript，用js直译器执行而不是json解析器解析。

## Multipart/Urlencoded绑定
form表单解析
s
# 洋葱模型
一种中间件架构模型。
层层嵌套，就像在洋葱的截面上划出U型的轨迹一样。
类似的还有：
+ 链式调用模型，中间件按照次序处理；
+ 拦截器模型，类似洋葱，叫法不同；
+ 注册模型，注册到一个中心，由中心的配置管理；
+ AOP 模型，在一个横切面与其他业务分离，洋葱也可以看作AOP
+ 函数链模型，Gin 洋葱内部的 c.Next()
+ 

# 对比

## Echo的独特特性
+ 内置验证功能
+ 错误集中管理
+ HTTP/2支持
+ 中间件更灵活？（参数化配置？）


## Beego的独特特性
+ MVC架构
+ 内置ORM，且支持界面交互？
+ 内置很多强大模块？
+ 正则路由、注解路由等
+ 模板引擎，模板目录、自动渲染、组合嵌套、数据绑定的套件
+ 监控智能？
+ 