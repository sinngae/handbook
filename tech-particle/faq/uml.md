# UML

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

