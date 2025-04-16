# 符号
字符：人类可阅读的符号，如文字符号、图像符号、格式符号、操作符号、表情符号等（计算机有一些不可见的机器控制符号，每个平台的实现各有不同。）

字符编码：计算机为表示字符、为每一个字符赋予一个唯一的编码。实现这个编码的功能系统称为字符编码集（Coded Character Set, CCS）。

## 常见字符编码
### ASCII
American Standard Code for Information Interchange,所有计算机都统一支持的，计算机间可以交互的字符编码。使用一个字节表达，仅使用后7位。
小写字母比大写字母大20个码。
有33个特殊的控制字符 [表A](!ascii_sepc_A.jpeg) [表B](!ascii_sepc_B.jpeg)。
ASCII

### Unicode
统一字符编码，记录了一个字符集合和其编码，如U+0031表示。

### UTF-8
unicode的编码方案之一，用1～4字节表达。

### GB2312 & GBK & BIG5
GB2312是面向简体中文，BIG5是面向繁体中文。
## Unicode
Unicode还在其发展期，Unicode、GB2312以及BIG5等多种编码共存的状况可能在以后较长的时间内都会持续下去。
### 
### UTF
UTF, Universal Character Set/Unicode Transformation Format。
UTF-8是针对Unicode的一种可变长度编码，兼容ASCII，逐渐成为电子邮件、网页、数据库、编程语言中，优先采用的编码。

## mysql.VARCHAR

## emoji符号
emoji是一批较特殊的符号