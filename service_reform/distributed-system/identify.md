# 分布式认证

## 单点登录
腾讯开放平台
阿里开放平台
百度开放平台

## OAuth2

## OpenID

## OIDC

## JWT
JWT，Json Web Token，用于创建带有自定义的签名和加密的带有一些声明的JSON格式的数据的一个互联网标准。token一般使用密钥或私钥/公钥做签名。
例如：0.客户端使用凭据登陆；1.认证服务（identity provider）给一个客户端生成了一个“以admin登陆了”声明的token，保存在客户端本地；2.客户端带着该token请求其他服务(service provider)时，被认为是个admin。
JWT依赖其他的基于JSON的标准：JSON Web Signature/JSON Web Encryption。
客户端请求受保护的服务或资源时，发送JWT放在HTTP头的Authorization的Bearer模式。
对于服务器来说，JWT认证是无状态的认证机制。JWT里包含着足够的信息，不需要服务器做多次数据库的查询。
JWT分三部分，Header/Payload/Signature，`token = base64urlEncoding(header) + '.' + base64urlEncoding(payload) + '.' + base64urlEncoding(signature)`。签名signature是使用header里的指定的加密算法、密钥、header和payload的base64urlencode的内容（使用句号分隔）生成的密文。
