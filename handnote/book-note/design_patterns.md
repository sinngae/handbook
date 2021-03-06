# 设计模式 design pattern
对接口编程，而不是对实现编程;
优先使用对象组合而不是继承;

## 设计模式类型
+ 创建型模式
    + Factory
    + Abstract Factory
    + Singleton
    + Builder
    + Prototype

+ 结构型模式
    + Adapter
    + Bridge
    + Filter/Criteria
    + Composite
    + Decorator
    + Facade
    + Flyweight
    + Proxy

+ 行为型模式 对象和对象的作用
    + Chain of Responsibility
    + Command
    + Interpreter
    + Iterator
    + Mediator
    + Mememto
    + Observer
    + State
    + Null Object
    + Strategy
    + Template
    + Visitor

+ J2EE模式
    + MVC模式
    + Bussiness Delegate
    + Composite Entity
    + Data Access Object 
    + Front Controller
    + Intercepting Filter
    + Service Locator
    + Transfer Object

## 六大原则
1. 开闭原则，反对switch/if-else

对扩展开放，对修改关闭

2. 里氏代换原则，反对不合理的抽象化和泛化

继承即可代换

3. 依赖倒转原则，反对依赖具体业务流程

开闭原则的基础，针对接口编程，依赖于抽象而不依赖于具体

4. 接口隔离原则，反对交叉，反对代码拷贝

5. 迪米特法则 知道最少原则，反对大而全

6. 合成复用原则，反对大量的继承和实现

尽量使用合成复用，而不是继承


## 消息机制 与 观察者模式
事件发生被观察者观察，
观察者订阅事件，
事件发布事件，
事件发生，事件通知观察者们
系统中经常有事件驱动的事务处理，如日切，日切事件是个线程在定时做。
如果有事务需要处理日切判断，则订阅日切事件。
日切事件发生时，通知所有的订阅了的事务。

GUI 与 组合模式
链表中的node中指向下一个node的next
xml解析的element结点包含很多子节点。
图形化用户界面的面板中有多个面板
部分整体的关系。部分和整体是同一对象，拥有统一接口。

数据结构遍历 与 迭代器模式
map、list、vector 都使用了迭代器模式
为了遍历一个聚合对象，设计的对象

资源管理 与 单例模式
数据库访问 数据库单例
文件访问 文件单例
zookeeper访问 zookeeper单例
kafka访问 kafka单例
日志对象
配置文件对象
线程池
消息队列

## 业务流程和测试流程构造不同的实例 工厂模式
kafka 的多种模式，可不可以用工厂模式呢

## 反思 Worse Is Better
```
设计模式用烂了，带来了不必要的系统复杂性，设计模式不是目的
敏捷软件开发力求系统的简洁
Refaction to pattern 指出 重构中使用模式，而不是一开始就使用模式
优秀的抽象往往是稳定的，垃圾的抽象带来系统的复杂度，却很少解决问题
在模式便利和复杂度之间做平衡，在接口的灵活性和简洁之间做平衡
ORM就是个反设计模式的
ORM模型简单、自动代码生成、效率多数情况的多数时间是足够的
ORM 抽象不足，错误的抽象，效率浪费，
映射本身不是目的，sql及结果才是目的
抽象不是增加概念，而是概念总结；抽象反而减少冗余的概念。
```



