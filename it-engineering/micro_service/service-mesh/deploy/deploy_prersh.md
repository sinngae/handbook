# 一、概括总览
从发布的策略类型上分，可以划分为蓝绿发布、金丝雀发布、分批灰度发布、网关流量灰度等。

从具体实现方案上分，发布方案有 传统的发布策略 GitOps+Jenkins 或 基于K8S Deployment的实现，也有云原生的开源产品 Argo Rollouts 和 FluxCD（二者都是CNCF成熟项目），也有CI/CD商业解决方案 Harness / Codefresh / Spinnaker，还有云厂商的解决方案 如阿里云等。

通过本案从不同角度（使用条件、实现原理、主要流程等）对比不同的灰度发布策略和发布方案。

参考：
- Argo Rollouts https://argoproj.github.io/argo-rollouts/features/bluegreen/
- FluxCD https://fluxcd.io/flagger/usage/deployment-strategies/
- Zadigx选型： https://mp.weixin.qq.com/s/Z2S7IK5pUenUJK2Fe5ApJg
- Codefresh https://codefresh.io/learn/software-deployment/what-is-blue-green-deployment/
- Spinnaker https://spinnaker.io/docs/guides/user/kubernetes-v2/traffic-management/#sample-bluegreen-pipeline
- Harness https://developer.harness.io/docs/continuous-delivery/deploy-srv-diff-platforms/kubernetes/kubernetes-executions/create-a-kubernetes-blue-green-deployment/
- 阿里云发布策略 https://help.aliyun.com/document_detail/153844.html?spm=a2c4g.160071.0.0.6a1d42733fXsuZ
- K8S ABTest https://www.testrail.com/blog/implement-ab-testing-using-kubernetes/#heading-toc-4
- ApiSix + Flagger: https://apisix.apache.org/zh/blog/2022/12/27/apisix-ingress-with-Flagger/

# 二、不同的发布策略对比
详见：应用发布流程补充说明 
# 三、传统的发布方案
简单发布 GitOps + Jenkins
[图片]
- 开发人员创建代码并编写Dockerfile。还需为应用程序创建Kubernetes manifests和Helm Charts。
- 开发人员将代码推送到源代码存储库。
- 源代码存储库使用提交后的钩子触发Jenkins构建。
- Jenkins CI流程将构建Docker镜像和Helm软件包，并将其推送到依赖仓库。
- Jenkins CD程序部署helm charts到k8s cluster。

这个过程是传统行业标准，但是有一些限制：
- 需要将Kubernetes 凭据存储在Jenkins服务器中。由于服务器是共享的，这是折中的做法。
- 尽管可以使用Jenkins创建和更改配置，但无法使用它删除现有资源。例如，如果从存储库中删除清单文件，则kubectl不会将其从服务器中删除。这是自动化GitOps的最大障碍。
基于K8S Deployment + Service的实现
https://kubernetes.io/zh-cn/docs/concepts/cluster-administration/manage-deployment/#canary-deployments
https://phoenixnap.com/kb/kubernetes-canary-deployments
[图片]
#  四、发布方案 -- 开源
1.Argo Rollouts

2.Fluxcd(Flagger)

https://fluxcd.io/flagger/tutorials/

https://developer.aliyun.com/article/748639

Flagger是一个能使运行在k8s体系上的应用发布流程全自动(无人参与)的工具, 它能减少发布的人为关注时间, 并且在发布过程中能自动识别一些风险(例如:RT,成功率,自定义metrics)并回滚.
[图片]

Flagger对应用自动化发布流程进行了很好的抽象, 提供了丰富的扩展机制(webhook, alter, metrics等).

    这些特性比较吸引人，那是否能能直接就能在集团内使用呢?
答案是不行。Flagger要求应用构建在k8s基础上, 例如服务发现机制, 另外要求部署Ingress/service mesh(这两者都具备调整流量策略的能力). 以HSF为例，它的服务发现机制是根据configserver, 服务是面向接口, 而不是应用。

如果不经过一定改造，估计还是无法使用。

另外Flagger也有一些改进的地方(我个人人为):

canary实例在切流过程中的扩缩容是根据HPA(如果配置了)来进行的, HPA扩缩容不及时就会对业务有影响.

改进方案: 可以根据stepWeight的变化动态调整canary的实例数, 这个只针对Canary release.

对于Blue/Green, A/B tesing可以通过webhook提前准备容量.

Flagger正在规划primary和canary流量比较特性，这个似乎和集团Doom干的是同一件事情。未来可以期待一下.
# 五、总结