# python

## 零、输入/输出 input/print

占位符：%s %d %f %x %%，%s把任意数据类型转为字符串处理。

## 一、基础
### 1.数据类型 变量 计算 转换
整型/浮点型/字符串（str）/字节类型（bytes）/布尔型/空值

布尔值：True/False；布尔运算：and/or/not ><=

空值None是python的特殊值·

列表/字典/自定义数据类型

python是动态语言，即变量本身的类型不固定，仅是一个标记。

python没有常量，只有约定大写变量为常量，但并不完全禁止改动。

```py
“”“
多行注释
”“”
demo = '\t\n\u32'   # 字符类型           
demo = r''          # 字符类型，表示''内部的字符串不转译
demo = b'ABC'       # 字节类型，其每个字符都只占一个字节
# 动态语言：变量就是变量，没有类型及其检查，指向的内容有类型

a = ‘abc’
b = a
a = ‘efg’   # b仍然为‘abc’
# python赋值语句可以理解为取右值的地址给左值？

result = 10 / 3     # result是浮点数，即使恰好整除
result = 10 // 3    # result是整数，//称为地板除
result = 10 % 3     # 取余数

ival = int(demo)    # str转int
demo = str(ival)    # int转str
```
### 2.字符编码
python3中，字符串以Unicode编码。
ord()函数获取字符的整数表示，char()函数吧编码转成对应字符。
可以使用十六进制编写字符，如'\u4e2d\u6587' 中文。

终坚持使用UTF-8编码对str和bytes进行转换，避免乱码。
对代码源文件指定用utf-8编码（编辑器也要设置UTF-8 without BOM编码才行）：
```py
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'abc'.encode('ascii')   # b'abc'
# 纯英文str可以用ASCII编码为bytes，内容是一样的;
# 含有中文的str可以用UTF-8编码为bytes;
# 含有中文的str无法用ASCII编码，因为中文编码的范围超过了ASCII编码的范围，Python会报错。在bytes中，无法显示为ASCII字符的字节，用\x##显示;

b'efg'.decode('ascii')  # 'efg'
# 从网络或磁盘上读取了字节流，读到的数据是bytes
# 要把bytes变为str，用decode()
# 如果bytes中包含无法解码的字节，decode()报错(可用errors='ignore'忽略错误的字节)

len('abc')      # 3，计算字符数量
len('中文'.encode('utf-8')) # 6，一个中文字符常占用3个字节。

'{0}, {1}! {2:.2f}'.format('hi', 'work', 3.14)  # {n} 占位符
# hi, work! 3.14
```

### 3.列表 list 及 元组 tuple
元组 tuple 初始化后不可修改，其他同list，比list更安全，尽量使用tuple
```py
names = ['jim', 'sam', 'wim']   # list类型
names[0]        # 'jim'，确保索引不越界，最后一个是len-1
names[-1]       # 'wim'
len[names]      # 3
names.append('aren')    # 追加
names.insert(1, 'li')   # 插入
names.pop()             # 末尾弹出
names.pop(1)            # 指定弹出

items = ['jim', 23, True, ['another'], names]
items[4][0]     # 'jim'

names = ('jim', 'sam', 'wim')   # tuple类型
items = ()      # 空元组
items = (1)     # 整数1
items = (1,)    # 一元组
items = (1, 2, [3, 4])  # 可变的list的元组
```

### 4.字典 dict 及 集合 set
```py
mydict = {'jim': 23, 'sam': 12, 'wim': 53}
mydict['jim']           # 23
mydict['liz']           # 异常报错
mydict.get('liz')       # None
mydict.get('liz', -1)   # -1
mydict.pop('jim')       # 23, 删除
# dict 内部存放顺序和放入顺序无关
# dict的key必须是不可变对象，比如：list不可作为key
# 内部使用哈希算法计算value的存储位置

myset = set([23, 12, 53])   # set是不重复的key的无序的集合，用一个list初始化
myset.add(1)
myset.remove(12)
```
### 5.python的可变与不可变对象
变量指向对象，可变对象可变，变后还是同一对象；不可变对象一般通过运算得到另一对象；

不变对象的数据不会被修改，多任务环境不需要加锁。

None也是不变对象；
### 6.顺序语句
1. if-elif-else
2. for-in
3. break/continue

```py
if a > 100:
    print('a > 100')
elif a > 10
    print('a > 10')

for item in items:  # items可以是list tuple dict set等
    print(item)

for x in range(10): # 内置range函数，生成[1, 10]整数序列
    print(x)
```

### 7.函数
python有很多内置函数，如abs(num), max(num, ...), 类型转换函数int/float/str/bool。
可以使用help(abs)查看帮助信息。
传入的参数的数量或类型不对，报错TypeError。
函数名是指向函数对象的引用，可以赋值给一个变量。

```py
def myfun(x，y=2):      # y提供默认值
    if not isinstance(x, (int, float)):     # type check
        raise TypeError('bad operand type')
    print(x)
    return x
    #return             # =return None
    # return x, y       # 返回多个值，实际是返回一个tuple类型，只不过省略了括号，且多个值可以被tuple赋值

def myfun(mylist=[]):
# 多次调用myfun()导致默认参数的值就变了，因为默认参数是一个变量对象
# 默认值一般指向不变对象，需要指向可变对象时，使用默认值None
    mylist.append('demo')
    return 

# 如果def myfun(list_or_tuple), 调用可简化省略一层括号；
#   调用时如果入参是list/tuple，前面加*，表示把list/tuple的所有元素作为可变参数传入。

# 如果def myfun(*myvar):            # 可变参数，可以传入0个或任意个参数，myvar当作tuple处理

# 如果def myfun(name, age, **kw):   # name,age是必选参数； kw是关键字参数，函数可以接受更多的参数，以key=value的形式的。

# 如果def myfun(name, age, *, city, job)        # *后的参数是命名关键字参数，如果是*myvar，则不用*做间隔符号
#   命名关键字参数必须传入参数名；必选参数是按位置来的必填参数；缺少*，python无法解析命名关键字参数和必选参数；
#   命名关键字参数常用默认值，简化调用

# 函数参数的顺序必须是必选参数、默认参数、可变参数、命名关键字参数、关键字参数
def myfun(name, age, loc='bj', *args, city, job, **kw):
    return 0
# 通过tuple/dict类型，可调用myfun函数

# 对于任意函数都可以通过def func(*args, **kw)的形式调用；

def myfun():        # 定义一个空函数
    pass            # pass是占位符

if age > 18:
    pass

# python函数允许递归，但要注意栈溢出问题
# 可以通过尾递归解决栈溢出问题，尾递归会被优化成循环处理
#   尾递归要求return语句返回调用本身或不包含表达式的不变对象。
def myfunc(n):
    return myfunc_iter(n, 1)
def myfunc_iter(num, product):
    if num == 1:
        return product
    return myfunc_iter(num - 1, num * product)
```

### 8.高级特性
python认为代码越少越好。越少，开发效率才能越高。python的高级特性就是为了少写代码。
```py
# 切片
listobj[1:3]       # 取从下标1开始，到下标3前止的元素切片
listobj[:3]        # 取从下标0开始，到下标3前止的元素切片（0可省略）
listobj[-3:-1]     # 从下标-3开始，到下标-1前止的元素切片
listobj[-3:]       # 从下标-3开始，到最后一个的元素切片
listobj[:10:2]     # 前10个元素，每两个取一个
listobj[::2]       # 所有元素，每两个取一个
listobj[:]         # list复制
(3, 2, 4, 5)[:3]    # tuple做切片，仍为tuple
'abcdefg'[:3]       # 字符串切片，仍为字符串（python没有substring操作，只有切片）

# 迭代
for item in listobj
for key in dictobj
for value in dictobj
for key, value in dictobj.items()
for ch in 'abc'
#   python可迭代对象判定
from collections import Iterable
isinstance(obj, Iterable)   #  返回True则可迭代
#   list转化索引-元素对
for idx, item in enumerate(listobj)
for x, y in [(1, 2), (2, 2), (3, 1)]

# list comprehensions 列表补全表达式
list(range(1, 11))  # 补全为从1到11之前的列表
[x * x for x in range(1, 11)]   # 以for间隔的表达式和循环体，创建list
[x * x for x in range(1, 11) if x % 2 == 0] # 带有if判定
[m + n for m in 'abc' for n in 'ABC']   # 双层for循环
[d for d in os.listdir('.')] # import os，os.listdir可以列出文件和目录
[k + '=' + v for k, v in dictobj.items()]
[s.lower() for s in listobj]
[x if x % 2 == 0 else -x for x in range(1, 11)] # 需要else的时候，if-else前置，反装饰器效果？

# 生成器
#   列表补全表达式占用空间大；生成器在运行时循环中生成
#   生成器就是把列表补全表达式的[]替换成()
g = (x * x for x in range(1, 11))
next(g) # 获得下一个返回值，边界后跑出StopIteration错误，几乎从不用next，而是用for
for item in g   # 生成器可迭代
#   如果一个函数定义中包含关键字yield，就不是不同函数，而是一个生成器
#   如斐波那契数列函数（无法用列表补全表达式）
def fib(_max):
    n, a, b = 0, 0, 1
    while n < _max:
        yield b         # next(gerator_obj)每运行到此，返回一次
        a, b = b, a + b
        n = n + 1
    return 'done'

# 迭代器
#   可迭代对象：集合数据类型（list/tuple/dict/set/str）和生成器
#   集合数据类型不是迭代器，可以被next()函数调用的且返回下一个值的对象称为迭代器iterator
#   迭代器判定
from collections import Iterator
isinstance(obj, Iterator)   #  返回True则是迭代器
#   可迭代对象转迭代器
iter(iterable_obj)
#   迭代器是一个数据流，可以看作是有序序列（不知道其长度），却不是序列对象
#   迭代器可以表达一个无限的数据流，如自然数，但序列对象list是不能存储所有的自然数
```

## 二、函数式编程
funtion programming -> funtional programming，函数式编程是一种抽象程度高的编程范式，纯粹的函数式编程的函数没有变量。
纯函数，输入确定，输出就是确定的。使用变量的函数则不是。
函数式编程甚至可以把函数作为参数或返回值。

python认为越高级语言，越抽象，越贴近计算而不是计算机，执行效率固然低？
### 1.高阶函数
变量可以指向函数，函数的参数接收变量 -> 一个函数接收另一个函数做参数，即高阶函数(high-order-function)。
```py
# e.g.1. map/reduce
#   map(function, iterable, ...)
#   reduce(function, iterable[, initializer])
map(f, [x1, x2, ..., xn]) = [f(x1), f(x2), ..., f(xn)]
#   map把f作用在iterable对象上，返回一个iterator
reduce(f, [x1, x2, x3, ..., xn]) = f(f(...f(f(x1, x2), x3)...), xn)
#   reduce把f递归作用在iterable对象上，返回值
reduce(lambda x, y: x * 10 + y, map(char2num, '12345'))
#   '12345'转整数

# e.g.2. filter
#   filter(function, iterable)
filter(f, [x1, x2, ..., xn]) = [x1, x2, ..., xm]
#   过滤序列
#   质数生成器 -- begin
def _odd_iter():
    n = 1
    while True:
        n = n + 2
        yield n

def _not_divisible(n):
    return labda x: x % n > 0

def primes():
    yield 2
    it = _odd_iter()    # 初始序列
    while True:
        n = next(it)    # 返回下一个数
        yield n
        it = filter(_not_divisible(n), it) # 构造新序列，嵌套的filter

for n in primes():
    if n < 1000:
        print(n)
    else:
        break
#   质数生成器 -- end

# e.g.3. sorted
#   sorted(iterable, cmp=None, key=None, reverse=False)
sorted([x1, x2, ..., xn], key=f) = iterable-obj-sorted-by-f
```

### 2.返回函数
```py
def lazy_fn(*args): # 定义返回函数
    def fn():
        retval = f(args)
        return retval
    return fn
fun = lazy_fn(1, 2, 3)  # 每次调用返回一个新函数，即使入参相同
fun()   # 执行

def lazy_fn():
    fns = []
    for i in range(1, 4):
        def f():
            return i*i
        fns.append(f)
    return fns
f1, f2, f3 = lazy_fn()
f1()    # 9
f2()    # 9
f3()    # 9
#   局部变量i被修改成3
#   返回函数不要引用任何循环变量，会发生不符预期的情况
```

### 3.匿名函数
```py
lambda x: x*x
# =
def fn(x)
    return x*x
```

### 4.装饰器
```py
def fn():
    pass
fn.__name__ # 'fn'

# 装饰器，返回一个高阶函数（执行了更多）
def log(text):  # 入参装饰
    def decorator(func):   # 装饰器
        @functools.wraps(func)  #   把原始func属性保存，执行后，修复回来（import functools）
        def wrapper(*argc, **kw):
            print('%s'%func.__name__)
            return func(*argc, **kw)
        return wrapper
    return decorator
@log('test')        # 装饰器调用，相当于fn = log('test')(fn)
def fn():
    pass

```

### 5.偏函数
```py
import functools
int2 = functools.partial(int, base=2)   # 返回一个新函数，封装了int函数，并参数base默认为2
max2 = functools.partial(max, 100)
#   偏函数实际是将默认参数打包成**kw/*argc的形式 作为 原函数的参数
```

## 三、模块与包
python里一个py文件就是一个模块。模块提高了代码的可维护性，可扩展性。

编写模块不用考虑与其他模块命名冲突，但尽量避免与内置函数冲突。
python按目录来组织模块，称为包管理。每个包都会有一个__init__.py文件（必须存在，否则就是普通目录），可以是空文件，以目录名为模块名。

模块和包命名要遵守python变量命名规范，并避免和系统模块冲突。
python交互环境中，执行import命令可以查看包是否存在。

模块的搜索路径优先顺序：当前目录 > 内置模块目录 > 第三方模块。
可以通过修改sys.path或设置环境变量PYTHONPATH修改。

模块里（或者py文件里）一般以python脚本声明、utf-8编码声明为第一二行，后续第一个字符串被视为该模块的文档注释。__author__变量用于标记写者。

python的作用域，python约定以'_'开头的为内部变量/函数，其他为公开的。__xxx__形式的为特殊变量，如__doc__是当前模块注释。
### 1.系统模块
```py
import sys
sys.argv    # 第一个元素是模块名，后续为调用参数
sys.path    # 打印模块搜索路径，可以append修改

if __name__ == '__main__'   # 判定当前py文件为运行模块，常用于执行测试
```

### 2.第三方模块
python使用pip做包管理。
python有丰富的第三方库，如图形库Pillow、mysql驱动库、web框架flask、科学计算numpy等。
Anaconda是一个python数据处理和科学计算平台，内置了很多第三方库，且自带python环境。

## 四、面向对象
类是从实例抽象出来的模版
```py
# 基础
class Student(object):  # Student类，继承自object
    attr = 'study'      # 类属性，所有实例公用；
    #   stu.attr会优先使用实例属性，再使用类属性；也可以Student.attr访问；应尽量避免类属性与实例属性重名；
    #   通过Student.attr修改类属性？
    def __init__(self, name, score):    # 特殊方法__init__，定义了入参，则创建实例必须满足入参要求
        self.__name = name  # 实例属性，以__为前缀，成为一个私有变量，外部不能直接访问（可通过_Student__name访问）
        self._score = score # 实例属性，以_为前缀，约定为私有变量，但是外部可以访问
        self.attr = 'test'
    def printout(self): # 类的方法
        print('%s: %s' % (self.name, self.score))

sam = Student('sammy', 60)  # 实例，每个实例内存地址不同
sam.test = 'test'   # python允许实例变量绑定任何数据，只和实例有关，和其类无关
sam.__name = '134'  # 是额外绑定的数据，而不是类成员

# 继承 -> 复用、多态
#   达到对扩展开放，对修改封闭
class SeniorStu(Student):
    pass

lizi = Senior('lizi', 54)   # 必须也满足基类实例化入参要求
isinstance(lizi, Student)   # True

def printout(obj):  # 定义了printout的函数的类的实例可以作为入参
    obj.printout()
#   python的多态不要求继承，任何实现了函数要求的实例都可以
#   python：看起来像鸭子，走起路像鸭子，就是鸭子

# type函数，判定对象的类型（及types包，types函数需要import）
type(123)           # class 'int'
type('123')         # class 'str'
type(None)          # type(None) 'NoneType'
type(abs)           # class 'builtin_function_or_method'
type(a)             # class '__main__.Animail'
type(123) == type(456)  # 以下皆True
type(123) == int
type(fn) == types.FunctionType
type(abs) == types.BuiltinFunctionType
type(lambda x: x) == types.LambdaType
type((x for x in range(10))) == types.GeneratorType

# isinstance函数，Base -> Derived，以下默认返回True
isinstance(derived, Base)
isinstance([1, 2, 3], (list, tuple))    # 判定是list或tuple

# dir函数，获得一个对象的所有属性和方法
dir('abc')      # ['__add__', '__class__', ..., '__subclasshook__', 'capitalize', 'casefold', ..., 'zfill']
#   len('abc')内部实际调用对象的__len__()方法，任何一个实现了__len__()的类的实例都可以用len(obj)调用
#   str类型的普通函数这样调用：'AbC'.lower()

# getattr()/setattr()/hasattr()，适用于变量和方法
hasattr(obj, 'mbr')     # 判定obj是否有属性mbr
setattr(obj, 'mbs', 9)  # 设置obj.mbs = 9
getattr(obj, 'mbs')     # 返回obj.mbs的值，不存在则抛出异常AttributeError
getattr(obj, 'mbt', 404)    # 返回obj.mbt，不存在则返回404

# python设计了一系列内置函数，用于剖析对象。如果知道了对象信息，就不要去剖析了。
#   这些函数常用于接口处理入参判定？

# 属性动态绑定与__slots__（属性白名单？）
from types import MethodTypes
obj.mtd = MethodType(fn, obj)       # 实例动态绑定函数属性，仅当前实例可用，obj.mtd()
Obj.mtd = fn                        # 类动态绑定函数属性，所有实例都可使用
class Cls(object):
    __slots__ = ('mbr', 'mtd')
    # 限制类及实例的动态绑定；白名单外的属性动态绑定抛出异常；
    # 对派生的不使用__slots__的子类不起作用
    # 对使用__slots__的子类，取并集
    pass

# 方法转属性访问 @property，保证了参数检查，同时代码简洁
class Stu(object):
    @property
    # python内置装饰器，使得可以stu.score形式访问_score
    #   而且创建了另一个装饰器score.setter，使得可以stu.score = val设值
    #   也可以不用setter，成为只读属性
    def score(self):
        return self._score
    @score.setter
    def score(self, value):
        # check & set
        self._score = value
```
### 1.多重继承
多重继承，又称MinIn，给类增加功能。相比单一继承下去，多重继承更简明。
```py
class Bat(Mammal, FlyableMinIn):
    pass
```
### 2.定制类
+ `__slots__` 属性白名单
+ `__len__` len(obj)调用
+ `__str__` str(obj)调用，print调用
+ `__repr__` 返回对象剖析信息，一般可以`__repr__ = __str__`
+ `__iter__` 支持`for ... in`调用
    + `__next__` 支持下一个，并在结束时返回StopIteration
+ `__getitem__(self, n)` obj[n]操作（`isinstance(n, slice)`判断是否是切片）
    + `__setitem__` 设置
+ `__getattr__(self, attr)` obj.attr/obj.attr()操作（显式优先级更高）
+ `__call__` 支持obj()操作，把obj当作函数；也用作判断一个对象是否是Callable
+ `__enter__` & `__exit__` with操作的对象需要支持的函数

```py
class Router(object):
    def __init__(self, path=''):
        self._path = path
    def __getattr__(self, path):
        return Router('%s/%s', % (self._path, path))
    def __str__(self):
        return self._path
    __repr__ = __str__
Router().api.v3.obj.func    # '/api/v3/obj/func
```
### 3.枚举类
```py
from enum import Enum, unique
Month = Enum('Month', ('Jan', 'Feb', 'Mar', 'Apr'))
Month.Jan   # 'Jan'
for name, member in Month.__member__.items():
    print name, member, member.value    # value是自动赋给成员的int常量

@unique     # 保证没有重复值
class Weekday(Enum):
    Sun = 0
    Mon = 1
    Tue = 2
```
### 4.元类
```py
# type调用
class Hiwork(object):
    pass
print(type(Hiwork)) # class 'type'
Hiwork hw
print(type(hw))     # class 'Hiwork'

def fn(self, name='work'):
    pass
hi = type('Hi', (object,), dict(work=fn))   # 创建Hi class
# python作为动态语言，运行时动态创建类，class定义是基于type调用实现
#   type调用和class定义一样，一般使用class定义

# metaclass
#   metaclass惯例用Metaclass做后缀
#   metaclass是类的模版，从type而不是object派生
class ListMetaClass(type):
    def __new__(cls, name, bases, attrs): 
        # cls - 当前创建的类的对象
        # name - 类的名字
        # bases - 类继承的父类集合
        # attrs - 类的方法集合
        attrs['add'] = lambda self, value: self.append(value)
        return type.__new__(cls, name, bases, attrs)
#   派生类用metaclass关键字指示继承自模版，python解释器在创建派生类时通过元类的__new__创建对象
#   可以通过修改元类的定义（添加新方法，修改旧方法），来修改派生类的定义
#   相比直接修改派生类的属性，或修改基类的属性，动态修改丰富了继承的灵活
class MyList(list, metaclass=ListMetaclass):
    pass
#   MyList就有了add()方法（普通list没有）
#   class继承和元类的区别？object和type的区别？
#   python哲学，一切都是对象，变量、函数、类等等。一切类都是继承自object？都是继承自type？object和type是一体的么？却别是什么
```
```txt
class，metaclass，instance，subclass，base
以下成立：
对任意的A，A是instance（推论：任意class也是instance）
对任意A，存在B，使得B是A的class
A是class 定义为 从A可以创建B（直接或间接），使得B是A的instance
A是B的metaclass 等价于 B是class，且A是B的class（也就是class的class）
A是metaclass等价于A是class，且从A创建的instance B是class

推论：
对任意A是class，存在B使得B是A的metaclass
type的metaclass是type自己

对A和B是class，A是B的base等价于B是A的subclass（这里为了防歧义规定A不是A的subclass，可能与iasubclass不同）
A是B的base，则B不是A的base
A是B的instance当且仅当：B是A的class，或B是A的class的base

object是所有其他class的base（推论：任意A是object的instance，包括object）
推论：没有类是object的base

type是所有其他metaclass的base
推论：object是type的base，type是object的metclass
type是type的class/metaclass
type是type的instance
object是object的instance
type是object的instance
object是type的instance
注意：metaclass的base不一定是metaclass
class与instance，base与subclass只是两种正交的关系，它们通过isinstance相互联系，在Python中并不要求它们有先后顺序
```
真让人头大的python继承和元类

**python允许环形继承，继承是规则，运行时会无限运行。**

[元类最经典的用法是数据库ORM](python-orm.md)

metaclass是python中非常具有魔术性的对象，可以改变类创建时的行为。功能强大，更需谨慎。

## 五、错误、调试和测试
功能性错误，入参合法性校验，磁盘满、网络断开之类的异常。
为了处理他们，python有一套异常处理机制，还有pdb调试，还有测试用例编写。
### try-except-finally机制
```py
try:                                # 捕获区
    val = 10/vint
except ZeroDivisionError as ex:     # 捕获指定错误
    print ex
except Exception as ex:             # 捕获所有的异常，除了ZeroDivisionError会在上面被捕获
    print ex
    raise                           # 继续上抛
    raise ValueError('input error') # 把一种错误类型转另一种
else:                               # 无错误执行
    print 'no error'
finally:                            # 最后，必然执行，即使有return语句
    print 'final'

class FooError(ValueError):         # 自定义类型
    pass
# 只有在必要时才自定义错误，尽量使用python内置错误类型

raise FooError('invalid valid')     # 抛出错误
```

所有的错误都是从BaseException继承，UnicodeError是ValueError的子类。

在适当的位置捕获错误，靠近错误发生的地方捕获处理。

### 调试
```py
# 断言
assert(expr)    # expr=false 会抛出AssertError。
#   `python -O test.py`关闭断言（不是0）。

# logging
import logging
logging.basicConfig(level=logging.INFO, filename='demo.log')
logging.info('hi, log')

# pdb
#   `python -m pdb test.py`，部分操作同gdb，l查看代码，p打印变量
import pdb
pdb.set_trace() # 设置断点
#   pycharm/vscode支持python单步调试，设置断点
```
### 单元测试
mylib_test.py
```py
import unittest
from mylib import lib
class TestLib(unittest.TestCase):
    def test_init(self):        # 以test开头的测试方法，测试的时候被执行
        assertEqual(lib.var, 0) # 常用断言
        assertTrue('')
        with self.assertRaises(KeyError):       # 期待被测试代码抛出指定Error
            val = dict['test']
        pass
    def test_attr1(self):
        pass
# `python -m unittest mylib_test.py`（可批量运行多个单元测试）或
if __name__ == '__main__':  # 正常python脚本
    unittest.main()
# python mylib_test.py

# setUp & tearDown
#   unittest.main()先调用setUp，再执行test_前缀函数，再执行tearDown函数。
#   常用于数据库连接

# doctest也非常有用，略
```

## 六、IO 编程
```py
# 文件读写
#   open函数打开file-like object，如文件、内存字节流、网络流、自定义流等，只要有read函数即可
with open('test.txt', 'b') as f:    # 同try-except-finally一样，'w' - 写；'r' - 读； 'a' - 追加；
    print f.read()

f.read()              # 读取所有
f.readlines()         # 读取所有，返回list，常用于读取配置文件
f.read(size)          # 每次读取最多size字节
f.readline()          # 每次读取一行

open('test.jpg', 'rb')              # 打开二进制文件
open('test.txt', 'r', encoding='gbk')   # 指定编码

f.write('abc')

# StringIO/BytesIO
from io import StringIO, BytesIO
f = StringIO()  # 可像文件一样读写的内存对象，操作str
f.getvalue()    # 获得写入后的内容
f = BytesIO()   # 可像文件一样读写的二进制内存对象，操作bytes
f.write('中文'.encode('utf-8'))

# 文件和目录操作
import os
os.name         # 操作系统类型：posix - unix/macos/linux； nt - windows；
os.uname        # 系统信息
os.environ      # 环境变量
os.environ.get('PATH', 'default info')
os.path.abspath('.')
os.mkdir(apath)
os.rmdir(apath)
ndpath = os.path.join('/users/au', 'work')  # 拼接路径，不要只用str拼接，使用join拼接，可以正确处理不同操作系统的路径分隔符
res = os.path.split('/dir/path/test.txt')   # 获取目录路径和文件名('/dir/path', 'test.txt')
res = os.path.splitext('/dir/path/test.txt')# 获取前缀和后缀('/dir/path/test', '.txt')
os.rename('abc', 'def')
os.remove('abc')
# shutil提供了copyfile函数，可以当作os模块的补充
[x for x in os.listdir('.') if os.path.isdir(x)]    # 当前目录所有文件夹
[x for x in os.listdir('.') if os.path.isfile(x) and os.path.spliext(x)[1]=='.py']  # 当前目录所有.py文件

# 序列化 反序列化
#   python中成为pickling/unpickling，其他语言中称为serialization/marshalling/flattening等
#   内存中的数据是散开的，序列化后（集合起来），可以存储到磁盘或传输到网络（走IO）
import pickle   # 只能用于python，其他语言并不支持的格式
d = dict(name='sam', age=12, score=20)
bstr = pickle.dumps(d)     # b'a string of binary bits'，把任意对象序列化为一个bytes
with open('dump.txt', 'wb') as f:
    pickle.dump(d, f)       # 把对象序列化写入文件
#   反序列化
dictobj = pickle.loads(bstr)
with open('dump.txt', 'rb') as f:
    dictobj = pickle.load(f)
import json # 操作同pickle，但不是所有对象都可以json序列化，比如自定义类型会报错TypeError
class Stu(object):
    pass
def stu2dict(stu):
    return {
        'name': stu.name,
        'age': stu.age,
        'score': stu.score
    }
dictobj = json.dumps(s, default=stu2dict)
dictobj = json.dumps(s, default=lambda obj: obj.__dict__)    # 使用class.__dict__
def dict2stu(d):
    return Stu(d['name'], d['age'], d['score'])
stuobj = json.loads(json_str, object_hook=dict2stu)
```

## 七、常用的内置模块和第三方模块
### 常用的内置模块
datetime/collections/base64/struct/hashlib/hmac/itertools/contextlib/urllib/XML/HTMLParser
### 常用的第三方模块
Pillow/requests/chardet/psutil

## 八、其他主题
进程线程/协程/异步IO/正则表达式
图形界面/网络编程/电子邮件/访问数据库/web开发/嵌入式？
### 进程线程
```py
import os
pid = os.fork() # 创建子进程，仅unix like
pid = os.getpid()   # 获取pid

from multiprocessing import Process, Pool
p = Process(target=func, args=('test')) # 1.
p.start()
p.join()

pool = Pool(4)  # 默认大小是CPU核数
for i in range(5):
    pool.apply_async(long_time_func, args=(i,))
pool.close()
pool.join()

import subprocess
r = subprocess.call(['nslookup', 'www.python.org'])

import subprocess
print('$ nslookup')
p = subprocess.Popen(['nslookup'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
output, err = p.communicate(b'set q=mx\npython.org\nexit\n')
print(output.decode('utf-8'))
print('Exit code:', p.returncode)
# 相当于
# nslookup
#   set q=mx
#   python.org
#   exit

import threading
threading.current_thread().name #   当前线程名，main默认为MainThread
t = threading.Thread(target=func, name='func')
t.start()
t.join()

lock = threading.Lock()
lock.acquire()
try:
    change(n)
finally:
    lock.release

local_var = threading.local()   # thread局域的全局
```
python启动与CPU核心数量相同的N个线程，在4核CPU上可以监控到CPU占用率仅有102%，即仅使用了一核。用C/C++写相同的死循环，直接可以把全部核心跑满，4核就跑到400%，8核就跑到800%，为什么Python不行呢？

因为Python的线程虽然是真正的线程，但解释器执行代码时，有一个GIL锁：Global Interpreter Lock，任何Python线程执行前，必须先获得GIL锁，然后，每执行100条字节码，解释器就自动释放GIL锁，让别的线程有机会执行。这个GIL全局锁实际上把所有线程的执行代码都给上了锁，所以，多线程在Python中只能交替执行，即使100个线程跑在100核CPU上，也只能用到1个核。

GIL是Python解释器设计的历史遗留问题，通常我们用的解释器是官方实现的CPython，要真正利用多核，除非重写一个不带GIL的解释器。
所以，在Python中，可以使用多线程，但不要指望能有效利用多核。如果一定要通过多线程利用多核，那只能通过C扩展来实现，不过这样就失去了Python简单易用的特点。
Python虽然不能利用多线程实现多核任务，但可以通过多进程实现多核任务。多个Python进程有各自独立的GIL锁，互不影响。