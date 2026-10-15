# JWT、OpenID、OAuth2、SessionID 的关系

一句话先分清定位：
- **SessionID：服务端会话凭证（传统登录）**
- **JWT：一种**令牌格式**（可以用来承载身份信息，不是协议）**
- **OAuth2：授权协议**，解决「A 应用授权 B 应用访问你的资源」
- **OpenID Connect (OIDC)：基于 OAuth2 的**身份认证协议**，解决「登录是谁」**

> 很多人混淆：OAuth 只做**授权**，不做登录；OIDC 在 OAuth2 基础上加了身份认证，返回用户身份。

## 1. SessionID（会话 ID）
传统 Cookie-Session 模式：
1. 用户账号密码登录成功
2. 服务器在**服务端内存 / 数据库**创建会话，生成随机字符串 `SessionID`
3. 返回给浏览器，存入 Cookie
4. 后续每次请求带上 SessionID，服务器查表，识别用户

特点：
✅ 状态保存在**服务端**（有状态）
❌ 分布式多服务器需要 session 共享；移动端、跨域不太友好
❌ SessionID 本身只是随机串，**不带任何用户信息**，必须去服务端查

> 和 JWT 对比：SessionID 是 “钥匙编号”，要去服务器仓库查这个人是谁；JWT 本身袋子里直接装用户信息（签名防篡改）。

## 2. JWT（JSON Web Token）
**是一种令牌格式，不是协议！**
把用户信息（JSON）做 Base64 编码 + 签名，生成一段字符串，可直接传给客户端。
结构：`header.payload.signature`

- payload：存放用户 ID、过期时间等 claims
- signature：防止客户端篡改内容

特点：
✅ **无状态**：服务端不用存会话，拿到 JWT 校验签名就能读取信息
✅ 适合分布式、微服务、第三方接口
⚠️ payload 只是 base64 编码，**不是加密！不能放密码、敏感数据**
⚠️ 一旦签发，没过期前无法主动作废（很难注销，这点不如 session）

> JWT 只是载体：**可以用在 OIDC 里当 ID Token，也可以自己业务登录用，和 OAuth 没有强制绑定**。

## 3. OAuth2（授权框架）

解决**授权问题**：

> 允许第三方应用，在不拿到你的账号密码的前提下，访问你在平台上的资源。

例子：用微信登录网站，网站不需要你的微信账号密码，只获得授权获取你的昵称头像。

OAuth2 核心产出：**Access Token（访问令牌）**

- Access Token 只是访问凭证，**默认不承载用户身份信息**
- Access Token 格式不一定是 JWT，可以是随机字符串（类似 SessionID）

> OAuth2 **本身不做身份认证（不知道你是谁），只管授权**。
> 所以才有 OpenID Connect。

## 4. OpenID Connect（OIDC，OpenID）

**基于 OAuth2 协议之上，增加身份认证能力**。
在 OAuth2 流程基础上，额外返回一个 **ID Token，这个 ID Token 规定格式就是 JWT！**

OIDC 流程（微信 / 支付宝 / 谷歌登录本质就是 OIDC）：

1. 第三方应用跳转到身份平台登录
2. 用户登录同意
3. 身份平台返回：
   - `Access Token`：OAuth 授权令牌，用来拉用户资源
   - `ID Token`：**JWT 格式！**，里面包含用户身份信息（sub 用户唯一标识等），证明「这个人是谁」

✅ OpenID = OAuth2 + 身份认证 + JWT ID Token

# 四者关系总结

1. **SessionID**：传统单体登录方案，服务端存会话，和另外三者不属于一套体系；
2. **OAuth2**：授权协议，产出 Access Token（格式不限，可以是随机串也可以 JWT）；
3. **OpenID Connect(OIDC)**：在 OAuth2 基础上做登录认证，**规定 ID Token 使用 JWT 格式**；
4. **JWT**：只是令牌数据格式。
   - 可以用于 OIDC 的 ID Token
   - 可以用于 OAuth2 的 Access Token
   - 也可以脱离 OAuth/OIDC，自己业务登录替代 SessionID

## 简单类比

- **SessionID**：去酒店入住，前台登记你的信息，给你一张房卡编号；每次拿编号，前台查档案确认身份（信息存在酒店后台）
- **JWT**：一张**盖章的纸质通行证**，纸上直接写你的姓名身份，盖章防篡改；酒店不用查后台，只要核验印章有效就认你（信息在通行证上）
- **OAuth2**：你授权摄影师，进入酒店房间拍照，摄影师拿授权凭证，**不是登录身份**
- **OIDC(OpenID)**：基于 OAuth 授权，额外发一张盖章通行证（JWT ID Token）证明你本人身份

## 常见组合场景

1. 传统网站登录：账号密码 → SessionID
2. 自研 APP 登录：账号密码 → 返回 JWT（替代 session）
3. 第三方快捷登录（微信登录网站）：OIDC
   - 底层协议：OAuth2
   - 身份凭证：ID Token（JWT）

## 关键区分点

- ❌ OAuth ≠ 登录（OIDC 才是登录）
- ❌ JWT ≠ OAuth，JWT 只是令牌格式
- ❌ SessionID 和 JWT 是两种会话方案，二选一居多

如果你想，我可以画一个极简时序对比：Session 登录流程 vs OIDC（OAuth2+JWT）登录流程。

---

# JWT 是否是 OpenID 的事实标准？
一句话结论:
**OIDC（OpenID Connect，大家口语说的 OpenID）规范里，ID Token 强制规定必须是 JWT；所以对于 OpenID Connect 来说，JWT 是规范标准，不只是 “事实标准”。**

> 
> 注意区分两个名词：
> 
> 
> 1. **OpenID 2.0（旧版）**：老协议，**不是**用 JWT
> 2. **OpenID Connect（OIDC，现在通用的 OpenID）**：基于 OAuth2，**规范强制要求 ID Token = JWT**

---

## 详细拆解

### 1. OIDC Core 规范原文要求

OIDC Core 1.0 明确：

> 
> The ID Token MUST be a JWT.

ID Token 是 OIDC 的核心身份凭证，作用就是证明「用户是谁」。
👉 **这是协议强制要求，不是业界习惯 / 事实标准，是规范标准。**

但是：

- OIDC 里的 **Access Token（访问令牌）规范没有强制 JWT**
Access Token 可以是不透明随机字符串（类似 SessionID），也可以是 JWT（JWT Access Token，很多厂商比如 Keycloak、Auth0 可选开启）。
- 只有 **ID Token 必须是 JWT**。

### 2. 容易混淆：“事实标准” vs “规范标准”

- **事实标准**：规范没强制，但大家都这么做。
- **规范标准**：协议文档写死必须这么实现，不满足就不属于合规 OIDC。

✅ OIDC ID Token：**规范强制 JWT**
✅ OIDC Access Token：可选，JWT 只是一个常用实现（事实标准）

> 
> 旧 OpenID 2.0（2000 年代老协议，现在基本淘汰）：没有 JWT，用 XML，和现在 OIDC 完全两套东西。现在大家提到 OpenID 登录（微信、Google、AzureAD、企业 SSO）全部都是 **OIDC**。

### 3. 和 OAuth2 的对比

OAuth2 本身**完全不规定令牌格式**：
Access Token 可以是任意字符串，可以是 uuid、随机串、JWT，OAuth2 协议不管。所以 OAuth 里 JWT Access Token 只能叫事实标准。

而 OIDC 在 OAuth2 之上叠加身份层，**硬性新增 ID Token，并且强制 JWT**。

## 总结一句话记忆

> 
> OpenID Connect（现代 OpenID）的**ID Token 是规范强制 JWT**；Access Token 不强制，JWT 只是可选实现。旧 OpenID2.0 不用 JWT。

如果你需要，我可以给一段最简 OIDC ID Token 的 JWT 样例，看里面标准的 claim 字段（iss, sub, aud, exp, iat）。


---

# IAM vs IDM（Identity & Access Management / Identity Management）
一句话：**IAM 是全集；IDM 是 IAM 的其中一部分（身份生命周期管理）**。

> IAM = IDM（身份管理） + AM（Access Management，访问管理）

## 定义
1. **IDM（Identity Management，身份管理）**
**管 “身份本身、账号生命周期”**，偏后台、偏数据、偏流程。
核心能力：

- 员工入职：自动在 AD、OA、ERP 新建账号（账号预配 provisioning）
- 调岗：更新部门、角色、属性
- 离职：回收、禁用所有系统账号（去预配 deprovisioning）
- 统一身份主数据：用户属性、组织、角色、岗位
- 权限账号审计、权限重认证（权限复核）

一句话概括 IDM：**这个人在各个系统里账号的生老病死**。

2. **IAM（Identity and Access Management，身份与访问管理）**
完整大框架，包含两大块：

- IDM：身份生命周期管理（账号增删改、主数据）
- AM（访问管理）：**登录、认证、鉴权**（你前面学的 OAuth/OIDC/JWT/SSO/MFA 都属于 AM）

   AM 核心能力：

   - SSO 单点登录
   - 认证（密码、MFA、OIDC/SAML）
   - 鉴权：登录成功后判断你能不能访问这个应用
   - 会话管理、令牌发放（JWT/Access Token）、访问策略

> 简单比喻
> 
> - IDM：**人事档案管理员**，维护你的身份信息，你入职建档案，离职销档案；
> - AM：**大楼门卫**，校验身份、放行 / 拒绝进入房间；
> - IAM = 人事档案管理员 + 门卫。

## 关系

✅ **IDM ⊂ IAM**
IAM 包含 IDM（身份生命周期）+ AM（访问登录控制）。

> 
> 行业小坑：
> 很多厂商、文档口语里，会把 IAM 和 IDM 混用，把 IAM 简称 IDM；但严格学术 / 产品划分是上面这个。

## 对比表

表格

| 项目 | IDM | IAM |
| --- | --- | --- |
| 全称 | Identity Management | Identity and Access Management |
| 核心关注点 | 身份、账号生命周期（静态 / 变更） | 身份 + 登录认证 + 资源访问控制 |
| 典型场景 | 入职自动建账号、离职回收账号、角色管理、权限审计 | SSO、OIDC、MFA、OAuth、登录鉴权、访问策略 |
| 协议 | SCIM（账号同步） | OIDC、OAuth2、SAML、SCIM |
| 产出物 | 账号、角色、属性 | ID Token (JWT)、Access Token、会话 |

## 和你前面学的 OIDC / JWT 关联

- OIDC、JWT、SSO、MFA 都属于 **IAM 里的 AM（访问管理）模块**
- IDM 负责把用户账号 / 角色推送到 IdP（身份提供商），IdP 再做 OIDC 登录、下发 JWT 令牌。

完整链路例子（企业员工登录 OA）：

1. HR 录入新人信息 → **IDM**自动在 AD、IdP 创建账号，分配角色
2. 用户访问 OA，跳转到 IdP 登录 → **IAM-AM** 执行认证，MFA，OIDC 协议，返回 JWT ID Token
3. OA 校验 JWT，判断角色，允许访问
4. 员工离职 → **IDM**自动禁用账号，撤销全部系统权限

## 补充几个易混名词

- **IGA（Identity Governance & Administration）**：身份治理，IDM 的高级增强版，强审计、风险分析、权限合规，很多现代 IAM 平台内置 IGA。
- **PAM**：特权账号管理（服务器 root、数据库管理员账号），属于 IAM 体系下独立模块。
