---
title: Hi, Yilia
date: 2019-09-30 08:33:34
tags:
 - Hexo
 - yilia
---
Yilia是为hexo 2.4+制作的主题。崇尚简约优雅，以及极致的性能。
本文记录在使用yilia过程中遇到的问题及解决方法。
<!-- more -->

## 1.添加导航菜单项
编辑./themes/yilia/_config.yml的menu内容，关联对应的tags即可

## 2.更多内容


## 3.添加右侧文章目录
> * 修改./themes/yilia/source/main.234bc0.css
> * 使用的是hexo提供的toc函数
> * 修改./themes/yilia/layout/_partial/article.ejs
> * 在` </header> <% } %}`下面添加如下内容:
```html
<!-- 目录内容 -->
<% if (!index && post.toc){ %>
	<p class="show-toc-btn" id="show-toc-btn" onclick="showToc();" style="display:none">
		<span class="btn-bg"></span>
		<span class="btn-text">文章导航</span>
	</p>
	<div id="toc-article" class="toc-article">
		<span id="toc-close" class="toc-close" title="隐藏导航" onclick="showBtn();">×</span>
		<strong class="toc-title">文章目录</strong>
		<%- toc(post.content) %>
	</div>
	<script type="text/javascript">
		function showToc(){
			var toc_article = document.getElementById("toc-article");
			var show_toc_btn = document.getElementById("show-toc-btn");
			toc_article.setAttribute("style","display:block");
			show_toc_btn.setAttribute("style","display:none");
		};
		function showBtn(){
			var toc_article = document.getElementById("toc-article");
			var show_toc_btn = document.getElementById("show-toc-btn");
			toc_article.setAttribute("style","display:none");
			show_toc_btn.setAttribute("style","display:block");
		};
	</script>
<% } %>
<!-- 目录内容结束 -->
```
> * 在需要目录的文章中开头加入:`toc: true`


更多内容:  
[yilia高级配置](http://dongshuyan.com/2019/05/24/hexo%E5%8D%9A%E5%AE%A2%E6%B3%A8%E6%84%8F%E4%BA%8B%E9%A1%B9/)
