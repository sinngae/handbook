"去除vi一致性
set nocompatible
filetype off

" YouCompleteMe 
set rtp+=~/.vim/bundle/YouCompleteMe
" Python Semantic Completion 
let g:ycm_python_binary_path = '/usr/local/bin/python3' 
" C family Completion Path 
let g:ycm_global_ycm_extra_conf='~/.vim/bundle/YouCompleteMe/.ycm_extra_conf.py' 
" 自动补全配置
set completeopt=longest,menu	"让Vim的补全菜单行为与一般IDE一致(参考VimTip1228)
autocmd InsertLeave * if pumvisible() == 0|pclose|endif	"离开插入模式后自动关闭预览窗口
inoremap <expr> <CR>       pumvisible() ? "\<C-y>" : "\<CR>"	"回车即选中当前项
"上下左右键的行为 会显示其他信息
inoremap <expr> <Down>     pumvisible() ? "\<C-n>" : "\<Down>"
inoremap <expr> <Up>       pumvisible() ? "\<C-p>" : "\<Up>"
inoremap <expr> <PageDown> pumvisible() ? "\<PageDown>\<C-p>\<C-n>" : "\<PageDown>"
inoremap <expr> <PageUp>   pumvisible() ? "\<PageUp>\<C-p>\<C-n>" : "\<PageUp>"

"youcompleteme  默认tab  s-tab 和自动补全冲突
"let g:ycm_key_list_select_completion=['<c-n>']
let g:ycm_key_list_select_completion = ['<Down>']
"let g:ycm_key_list_previous_completion=['<c-p>']
let g:ycm_key_list_previous_completion = ['<Up>']
let g:ycm_confirm_extra_conf=0 "关闭加载.ycm_extra_conf.py提示

let g:ycm_collect_identifiers_from_tags_files=1	" 开启 YCM 基于标签引擎
let g:ycm_min_num_of_chars_for_completion=2	" 从第2个键入字符就开始罗列匹配项
let g:ycm_cache_omnifunc=0	" 禁止缓存匹配项,每次都重新生成匹配项
let g:ycm_seed_identifiers_with_syntax=1	" 语法关键字补全
nnoremap <F5> :YcmForceCompileAndDiagnostics<CR>	"force recomile with syntastic
"nnoremap <leader>lo :lopen<CR>	"open locationlist
"nnoremap <leader>lc :lclose<CR>	"close locationlist
inoremap <leader><leader> <C-x><C-o>
"在注释输入中也能补全
let g:ycm_complete_in_comments = 1
"在字符串输入中也能补全
let g:ycm_complete_in_strings = 1
"注释和字符串中的文字也会被收入补全
let g:ycm_collect_identifiers_from_comments_and_strings = 0

nnoremap <leader>jd :YcmCompleter GoToDefinitionElseDeclaration<CR> " 跳转到定义处

" taglist 
"Plugin 'taglist.vim'
set rtp+=~/.vim/bundle/taglist.vim
map <F3> :TlistToggle<CR>
let Tlist_Use_Right_Window=1
let Tlist_Show_One_File=1
let Tlist_Exit_OnlyWindow=1
let Tlist_WinWidt=25

"插件管理器vim bundle
set rtp+=~/.vim/bundle/Vundle.vim
"vundle 初始化
call vundle#begin()
"让vundle管理插件
Plugin 'VundleVim/Vundle.vim'

"其他插件
"Plugin '用户名/插件仓库名'
"Plugin 'Valloric/YouCompleteMe'

Plugin 'nerdtree'
map <F2> :NERDTreeToggle<CR>
let NERDTreeWinSize=25

Plugin 'vim-airline/vim-airline'
"优化界面
"let g:airline#extensions#tabline#enabled=1
"显示颜色
set t_Co=256
set laststatus=2
"powerline字体
let g:airline_powerline_fonts=1
"tabline 
let g:airline#extensions#tabline#enabled=1
let g:airline#extensions#tabline#left_sep=' '
let g:airline#extensions#tabline#left_alt_sep=' '
let g:airline#extensions#tabline#buffer_nr_show=1
"映射切换buff的键位
nnoremap [b :bp<CR>
nnoremap ]b :bn<CR>
"映射<leader>num到num buffer 
map <leader>1 :b 1<CR>
map <leader>2 :b 2<CR>
map <leader>3 :b 3<CR>
map <leader>4 :b 4<CR>
map <leader>5 :b 5<CR>
map <leader>6 :b 6<CR>
map <leader>7 :b 7<CR>
map <leader>8 :b 8<CR>
map <leader>9 :b 9<CR>

Plugin 'vim-airline/vim-airline-themes'
let g:airline_theme='bubblegum'

Plugin 'Raimondi/delimitMate'

"插件结束
call vundle#end()
filetype plugin indent on

"模式自动补全
set wildmenu

""" C++ 等文本编辑
"语法高亮
if has("syntax")
	syntax on
endif
"高亮所在行
set cul
"浅色高亮当前行
autocmd InsertEnter * se cul
"设置行间隔
set linespace=0
"设置退格键可用
set backspace=2
"整词换行
set linebreak
"光标可以从行首或者行末换到另一行
set whichwrap=b,s,<,>,[,]
"鼠标可用
set mouse=a
"显示行号
set number
"设置历史纪录条数
set history=50
"自动写磁盘
set autowrite

"""配色方案
syntax enable
syntax on
"设置背景色
set background=dark
"配色方案
"colorscheme solarized
colorscheme desert

set t_Co=256
"字体
set guifont=Consolas:h11


"tab设置
set tabstop=4
set shiftwidth=4
autocmd FileType c,cpp,h set tabstop=4 | set shiftwidth=4 | set expandtab

"""对齐与自动缩进
"自动对齐
set autoindent
"智能对齐
set smartindent
"c/c++的自动缩进
"autocmd FileType c,cpp,h 
set cinoptions={0,1s,t0,n-2,p2s,(03s,=.5s,>1s,=1s,:1s
"行和段处使用制表符
"set smarttab


" 新建头文件
autocmd BufNewFile *.h,*.hpp,*.sh exec ":call SetTitle()"
" 自定义函数
func SetTitle()
	if &filetype == 'sh'
		call setline(1, "#########################################")
		call append(line("."), "# File Name: ".expand("%"))
		call append(line(".")+1, "# Author: Philo")
		call append(line(".")+2, "# mail: ")
		call append(line(".")+3, "# Created Time: ".strftime("%c"))
		call append(line(".")+4, "########################################")
		call append(line(".")+5, "#!/bin/bash")
		call append(line(".")+6, "")
	else 
		call setline(1, "/************************************************")
		call append(line("."), "  > File Name: ".expand("%"))
		call append(line(".")+1, "  > Author: Philo")
		call append(line(".")+2, "  > Mail: ")
		call append(line(".")+3, "  > Created Time: ".strftime("%c"))
		call append(line(".")+4, "******************************************/")
		call append(line(".")+5, "")
	endif 
	autocmd BufNewFile * normal G
endfunc

"encoding 
set encoding=utf8
set fileencodings=ucs-bom,utf-8,cp936,gb2312
set termencoding=utf8

nmap <F6> a<C-R>=strftime("%Y-%m-%d %H:%M:%S")<CR><Esc>
imap <F6> <C-R>=strftime("%Y-%m-%d %H:%M:%S")<CR>
