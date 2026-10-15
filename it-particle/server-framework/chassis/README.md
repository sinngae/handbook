# Chassis
chassis是Go语言的微服务框架，可用于快速开发分布式系统。
chassis被设计成独立于协议的框架，可以把任意协议集成到chassis。chassis可以弹性集成，还可以集成负载均衡、回路打破、限流。
chassis被设计成可服务观测的，开放式的跟踪、自带prometheus。
chassis被设计成模块灵活的，注册、metrics(prometheus API?)、handler链、配置中心等都可用自定义的实现替代。
chassis内置了一些功能，如路由管理、回路破坏、负载均衡、监控等，减少开发工作。
chassis支持istio平台，可用Istio配置控制chassis（虽然从数据层面看，Istio减少了吞吐量、增加了服务延时和cpu占用）。

