# UML

## 按角色组织文档
+ 产品需求视图  
需求规格、业务字典、
+ 开发视图  
系统架构、业务逻辑、代码仓库
+ 测试视图  
测试用例、自动化测试
+ 管理视图  
投入成本、时间排期、配置管理
+ 运维视图  
部署、升级、自动化

## 一、类图
### 1.依赖关系 dependency
use a，一个类依赖于另一个类，A依赖于B，则B为A的函数里的局部变量、方法参数、静态方法调用等  
在A的cpp文件中用到了B  
类图中一般不画，免得过于细致  

### 2.关联关系 association 
has a，单向或双向关系（一般避免双向），A单向关联B，B为A的全局变量  
在A的头文件中A类的成员  

### 3.聚合关系 aggregation 
aggregated of,单向，关联关系的一种，区别在语义上：关联的两个对象是平等的；聚合的两个对象是整体与局部关系。实现上，区别不大。  
A聚合B，A是B的聚合体，A不存在了，B依然在。整体不存在了，部分依然在。

### 4.组合关系 composition 
combination of,强依赖的特殊聚合关系。  
整体不存在，部分也不存在。  
合成关系不能共享。  

### 5.继承关系 inheritance
类实现接口；类继承抽象类；类继承父类  
实现 realization ：类实现接口  
泛化 generalization : is a，类继承抽象类或父类  

## 二、用例图

## 末、UML颜色标准
经典的UML案例往往是分析的结果，却很少有可以把得到或接近优秀结果的分析的方法和步骤介绍出来的。
原来使用UML做统一建模，一般使用每个对象的方括号括起，以标识是类还是接口。
数百种领域模型中，始终有四种主要的类型，称为原型。
UML对原型按下列先后顺序着色：
1. 粉色，moment-interval
时刻/间隔，模型中人、地点或物等的活动，如下单、支付、履约
2. 黄色，role
角色，模型中参与活动的人、地点或物等
3. 蓝色，catalog-entry-like description
描述，一些归类用的、打标签用的叙述
4. 绿色，party, place or thing
物，物理世界的一个个普通对象，角色的具象？

许多人认为有色物体吸引了大脑的模式识别部分，使帮助确定领域模型的各个方面变得容易，尤其是对于建模新手而言。