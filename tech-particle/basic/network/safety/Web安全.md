# Web安全
随着Web应用的兴起，Web应用受到更多网络攻击，随之而演进了一批Web安全技术。

OWASP，Open Web Application Security Project，开放网络软件安全计划，是为了研究帮助解决网络软件安全的标准/工具/技术的全球协会。其30多个计划中，最知名的是OWASP Top 10(10大网络应用系统安全问题)、WebGoat练习平台、安全PHP/Java/ASP.Net计划等。

OWASP Top 10:
1. XSS，Cross Site Scripting

## CSRF攻击
服务端在收到路由请求时，生成一个随机数，在渲染请求页面时把随机数埋入页面（一般埋入 form 表单内，`<input type="hidden" name="_csrf_token" value="xxxx">`）
服务端设置setCookie，把该随机数作为cookie或者session种入用户浏览器
当用户发送 GET 或者 POST 请求时带上_csrf_token参数（对于 Form 表单直接提交即可，因为会自动把当前表单内所有的 input 提交给后台，包括_csrf_token）
后台在接受到请求后解析请求的cookie获取_csrf_token的值，然后和用户请求提交的_csrf_token做个比较，如果相等表示请求是合法的。


Token 保存在 Session 中。假如 Token 保存在 Cookie 中，用户浏览器开了很多页面。在一些页面 Token 被使用消耗掉后新的Token 会被重新种入，但那些老的 Tab 页面对应的 HTML 里还是老 Token。这会让用户觉得为啥几分钟前打开的页面不能正常提交？
尽量少用 GET。假如攻击者在我们的网站上传了一张图片，用户在加载图片的时候实际上是向攻击者的服务器发送了请求，这个请求会带有referer表示当前图片所在的页面的 url。 而如果使用 GET 方式接口的话这个 URL 就形如： `https://xxxx.com/gift?giftId=aabbcc&_csrf_token=xxxxx`，那相当于攻击者就获取了_csrf_token，短时间内可以使用这个 token 来操作其他 GET 接口。
