# 用Vim打造成C++超级IDE
  使用Liunx，会很羡慕Xcode那种超级智能的开发环境，总想在Vim用起那些快捷方式，并把如何实现的方式记录下来。本文具体的实践环境为：CentOS 7。
## CentOS yum install vim
$su
>type your pwd
$yum install vim 

## 安装Vundle插件管理
### 安装管理
### 安装插件
下载插件到~/.vim/bundle/<br>
编辑~/.vimrc添加插件项<br>
命令模式执行:PluginInstall<br>

### Installing YouCompleteMe
附见:ycm_extra_conf.py

### Installing Ctags
http://ctags.sourceforge.net/
最简单的方式是下载Source RPM 格式，直接单击就可以安装。

### Installing Cvim
http://www.vim.org/scripts/script.php?script_id=213

安装步骤 ：(详细可以参见：vim /usr/share/vim/vimfiles/doc/templatesupport.txt 官方文档）
+ 1. 解压软件；
+ 2. 进入解压的文件夹，打开终端。& mv 文件名 / /usr/share/vim/vimfiles/子文件夹；
+ 3. 如过移动不成功，则需要权限，输入命令$ su 以后再执行第二步的操作。
+ 4. 设置vimrc 文件 ，$ sudo vim /etc/vimrc 加入 $filetype plugin on

### 安装 OmniCppComplete
下载地址:http://www.vim.org/scripts/script.php?script_id=1520

安装步骤：
+ 1. 解压软件；
+ 2. 进入解压的文件夹，打开终端。& mv 文件名 / /usr/share/vim/vimfiles/子文件夹；
+ 3. 如过移动不成功，则需要权限，输入命令$ su 以后再执行第二步的操作。

详细可以参见：vim /usr/share/vim/vimfiles/doc/omnicppcomplete.txt官方文档

### Installing Autocomplpop
下载地址:http://www.vim.org/scripts/script.php?script_id=213

安装步骤：
+ 1. 解压软件；
+ 2. 进入解压的文件夹，打开终端。& mv 文件名 / /usr/share/vim/vimfiles/ 子文件夹；
+ 3. 如过移动不成功，则需要权限，输入命令$ su 以后再执行第二步的操作。

详细安装步骤：vim  /usr/share/vim/vimfiles/doc/acp.txt 官方文档

### 支持markdown
git clone https://github.com/plasticboy/vim-markdown.git

