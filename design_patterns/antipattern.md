# 反模式

不良设计主要有四种特征:
1. 不动性: 以这种方式开发的应用程序非常难以重用。
2. 刚性: 以这种方式开发的应用程序, 任何小的修改会导致软件的大多部分必须进行相应的改动。
3. 脆弱性: 当前应用程序的任何更改都会导致现有系统变得非常容易奔溃。
4. 粘滞性: 由于架构层面的修改非常困难, 因此修改必须由开发人员在代码或环境本身中进行。

## 烟囱式设计
常称做孤岛系统，指不能很灵活地适应各种业务场景，如果需要开展新业务，就需要重新实现一遍代码（相当量的部分代码，可能是框架的、中间件等组件）。具有以下问题：
1. 重复建设和维护，进而业务发展创新成本高
2. 系统间数据兼容成本高
3. 不利于业务的沉淀
业务逻辑实现得复杂（非必要的），维护成本越来越高，隔数年就需要重新设计系统。
业务沉淀（互联网用语，指业务

## 