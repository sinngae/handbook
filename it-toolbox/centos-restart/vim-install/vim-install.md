# vim install
Vim安装指南

## 安装Vim
```sh
# 0. 最新的版本的vim需要依赖最新的python3
./configure --prefix=/usr/local/python3/ --enable-optimizations--with-ssl-default-suites=openssl --enable-shared

# 1. 编译vim
git clone https://github.com/vim/vim.git
./configure --enable-multibyte --enable-pythoninterp --with-python-config-dir=/usr/lib64/python2.7/config --enable-python3interp=yes --with-python3-config-dir=/usr/local/python3/lib/python3.7/config-3.7m-x86_64-linux-gnu --prefix=/usr/local/vim8
#./configure --enable-multibyte --enable-python3interp=yes --with-python-command=/usr/bin/python --with-python-config-dir=/usr/lib64/python2.7/config  --with-python3-command=/usr/local/bin/python3  --with-python3-config-dir=/usr/local/lib/python3.7/config-3.7m-x86_64-linux-gnu --prefix=/usr/local
```

## 编辑.vimrc
```.vimrc
在.vimrc中添加以下代码后，重启vim即可实现按TAB产生4个空格：
set ts=4  (注：ts是tabstop的缩写，设TAB宽4个空格)
set expandtab

对于已保存的文件，可以使用下面的方法进行空格和TAB的替换：
TAB替换为空格：
:set ts=4
:set expandtab
:%retab!

空格替换为TAB：
:set ts=4
:set noexpandtab
:%retab!
```
## Vundle + YCM
```sh
# 1. 安装vundle
# 2.编译YouCompleteMe
git clone https://github.com/VundleVim/Vundle.vim.git
git clone https://github.com/Valloric/YouCompleteMe.git
install libncurses5-dev libgnome2-dev libgnomeui-dev libgtk2.0-dev libatk1.0-dev libbonoboui2-dev libcairo2-dev libx11-dev libxpm-dev libxt-dev python-dev python3-dev ruby-dev lua5.1 liblua5.1-dev libperl-dev git
git submodule update --init --recursive
python3 install.py --clang-completer

#   3.clang llvm
```
.vimrc
```.vimrc
" YouCompleteMe
" Python Semantic Completion
let g:ycm_python_binary_path = '/usr/bin/python3'
" C family Completion Path
let g:ycm_global_ycm_extra_conf='~/.ycm_extra_conf.py'
" 跳转快捷键
nnoremap <c-k> :YcmCompleter GoToDeclaration<CR>
nnoremap <c-h> :YcmCompleter GoToDefinition<CR>
nnoremap <c-j> :YcmCompleter GoToDefinitionElseDeclaration<CR>
" 停止提示是否载入本地ycm_extra_conf文件
let g:ycm_confirm_extra_conf = 0
" 语法关键字补全
let g:ycm_seed_identifiers_with_syntax = 1
" 开启 YCM 基于标签引擎
let g:ycm_collect_identifiers_from_tags_files = 1
" 从第2个键入字符就开始罗列匹配项
let g:ycm_min_num_of_chars_for_completion=2
" 在注释输入中也能补全
let g:ycm_complete_in_comments = 1
" 在字符串输入中也能补全
let g:ycm_complete_in_strings = 1
" 注释和字符串中的文字也会被收入补全
let g:ycm_collect_identifiers_from_comments_and_strings = 1
" 弹出列表时选择第1项的快捷键(默认为<TAB>和<Down>)
let g:ycm_key_list_select_completion = ['<C-n>', '<Down>']
" 弹出列表时选择前1项的快捷键(默认为<S-TAB>和<UP>)
let g:ycm_key_list_previous_completion = ['<C-p>', '<Up>']
" 主动补全, 默认为<C-Space> "
let g:ycm_key_invoke_completion = ['<C-Space>']
" 停止显示补全列表(防止列表影响视野), 可以按<C-Space>重新弹出
let g:ycm_key_list_stop_completion = ['<C-y>']
```

