# HTTP 常见错误码
HTTP 状态码分为 5 类，错误码主要集中在 4xx（客户端错误） 和 5xx（服务器错误） 两类。以下是开发中最常见的错误码及其实际含义：

## 4xx 客户端错误（请求有问题）
状态码	名称	常见场景
+ 400	Bad Request	请求格式错误、参数缺失或类型不匹配（如 JSON 解析失败）
+ 401	Unauthorized	未提供身份验证信息，或 Token 过期/无效（需登录）
+ 403	Forbidden	身份已验证，但无权限访问该资源（如普通用户访问管理员接口）
+ 404	Not Found	请求的资源不存在（URL 拼写错误、记录已被删除）
+ 405	Method Not Allowed	HTTP 方法不支持（如用 POST 访问只支持 GET 的接口）
+ 408	Request Timeout	客户端发送请求耗时过长，服务器等待超时
+ 409	Conflict	请求与当前资源状态冲突（如重复提交、并发修改冲突）
+ 410	Gone	资源曾经存在但已被永久删除（比 404 更明确）
+ 413	Payload Too Large	请求体过大（如上传文件超过服务器限制）
+ 414	URI Too Long	URL 过长（常见于 GET 请求携带过多参数）
+ 422	Unprocessable Entity	请求格式正确，但语义有误（如字段值超出允许范围）
+ 429	Too Many Requests	请求频率过高，触发限流（Rate Limiting）

## 5xx 服务器错误（服务端出问题）
- 状态码	名称	常见场景
+ 500	Internal Server Error	服务器内部未捕获的异常（代码 bug、数据库连接失败等）
+ 501	Not Implemented	服务器不支持该功能（如某个 HTTP 方法未实现）
+ 502	Bad Gateway	网关/代理从上游服务器收到无效响应（如 Nginx → 后端服务挂了）
+ 503	Service Unavailable	服务器暂时过载或维护中（如流量激增、服务重启）
+ 504	Gateway Timeout	网关/代理等待上游服务器响应超时（后端处理太慢）

## 补充：非错误的常见状态码
为了完整理解，以下状态码不是错误，但同样高频出现：
- 状态码	名称	含义
+ 200	OK	请求成功
+ 201	Created	资源创建成功（常用于 POST 请求）
+ 204	No Content	成功但无返回内容（常用于 DELETE）
+ 301	Moved Permanently	永久重定向（SEO 会传递权重）
+ 302	Found	临时重定向
+ 304	Not Modified	缓存有效，无需重新传输资源

快速区分
+ 4xx：检查你的请求（URL、参数、权限、Token）
+ 5xx：联系后端开发人员，或稍后重试
+ 401 vs 403：401 是"你是谁"的问题，403 是"你能做什么"的问题