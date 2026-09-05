企业信息系统工程架构理论
---
企业架构，EA：Enterprise Architecture

# 方法论
+ Zachman, IBM, 1987
    + 6×6 矩阵：6类干系人视角（Planner/Owner/Designer/Builder/Subcontractor/User） × 6个核心问题（What/How/Where/Who/When/Why）
    + 提供架构内容的组织与分类 schema，不提供具体实施过程
    使不同角色的干系人能够使用统一的描述方式进行沟通。 但其局限在于仅提供内容分类方法，缺乏创建架构的具体实施指导。
+ EAP, Enterprise Architecture Planning，企业架构规划,1992
    + 企业架构的规划方法论，偏重动态过程
    + 核心结构，四个层级：①启动 → ②现状分析(As-Is) → ③目标分析(To-Be) → ④实现与整合计划
    + 顺序，数据架构 → 应用架构 → 技术架构（严格顺序）
    + EAP 基于 Zachman 框架发展而来，聚焦其顶层两个层次（业务模型与目标范围），采用业务驱动、数据驱动的自上而下的规划方法。 其目标是确保数据质量、数据可访问性、互操作性及成本控制。 EAP 后被美国联邦政府架构框架（FEAF）直接引用。
+ ```TOGAF，The Open Group Architecture Framework```，The Open Group 基于美国国防部的 TAFIM提出，1995
    + 企业级架构方法论：架构愿景=》业务架构=》信息系统架构=》技术架构
    + 分域，业务架构、应用（应用系统）架构、数据架构、技术架构
    + 核心结构，ADM（Architecture Development Method）：8个阶段迭代循环
    + TOGAF 是目前全球使用最广泛的企业架构框架。其核心 ADM 涵盖从架构愿景、业务架构、信息系统架构、技术架构到实施治理的完整流程。
    + TOGAF 高度抽象且不依赖特定组织形式，任何企业均可根据自身情况裁剪或与其他框架混合使用。
+ DoDAF（Department of Defense Architecture Framework），DoD，2000s
    + 面向国防/政府复杂系统的协同框架，国防专用，多视角模型
    + 核心结构，8个视角（All View, Capability View, Data & Information View, Operational View, Project View, Services View, Standards View, Systems View）
    + DoDAF 最初为军事系统设计，后扩展至航空航天、国防承包商等高安全可靠性行业。 其结构化模型适合管理跨部门的大规模系统集成项目，但在商业环境中可能显得过于繁重。
+ Gartner（Gartner Enterprise Architecture Method, GEAM），Gartner公司
    + 该公司的企业架构实践方法论与咨询服务
    + 强调业务战略与IT战略的对齐，关注价值获取与ROI
    + 非严格意义上的标准化框架，而是基于咨询经验的实践指南
    + Gartner 方法的目标是创建与组织战略目标一致、支持业务目标的企业架构。 它在业务关注度、实践指南和价值获取效率方面表现突出。

- TOGAF，全行业大型企业架构落地
- DoDAF，国防/军工/高安全系统集成	
- Gartner, 业务战略与IT对齐咨询

+ Zachman, 地图，企业架构应该有什么
+ EAP，蓝图规划，自上而下、数据优先、制定从现状到目标的实施计划
+ TOGAF，施工手册，提供完整的、可迭代的、可裁剪的架构开发流程（ADM）
+ Gartner，顾问方案，聚焦业务价值，强调战略对齐与投资回报

在实际应用中，TOGAF 因其开放性和完整性成为企业首选，而 Zachman 常作为内容组织工具与之配合使用。EAP 适合数据治理基础薄弱、需要从头梳理信息架构的组织；DoDAF 则专用于国防及高安全领域；Gartner 方法更适合作为战略咨询的参考框架。