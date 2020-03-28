---
title: Hi, Hexo
date: 2019-09-23 21:26:27
tags:
 - Hexo
---
Hexo是一个个人博客的框架。

[More info](https://www.hexo.com/docs)
<!-- more -->
## 安装Node.js
(略)

## 安装Hexo
> * 指定一个文件夹(如./blog)，作为安装路径
> * 安装：npm install hexo-cli -g
> * 检查版本：hexo -v
> * 初始化文件夹：hexo init
> * 安装需要模块：npm install hexo-generator-json-content --save

## 安装主题yilia
> * cd ./blog/themes/
> * git clone git@github.com:litten/hexo-theme-yilia.git
> * 添加头像图片(如avatar.jpg)到./blog/themes/source/
> * 添加网站logo图片(如favicon.png)到./blog/themes/source/
> * 修改./blog/themes/_config.yml配置项avatar和favicon为avatar.jpg和favicon.png

## 运行起来

```sh
# 新建文档发布
hexo new "My New Post"
# 运行服务
hexo server
# 生成静态文件
hexo generate
# 发布到远程地址
hexo deploy
```


**更多信息:**  
+ [Hexo文档](https://hexo.io/zh-cn/docs/themes)
+ [一个十分详尽的教程](https://godweiyang.com/2018/04/13/hexo-blog/)
+ [Markdown语法](https://www.zybuluo.com/mdeditor)
+ [YAML语法]()
