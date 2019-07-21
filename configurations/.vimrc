set nocompatible              " required
filetype off                  " required
" version_1.1



" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()

" let Vundle manage Vundle, required
Plugin 'gmarik/Vundle.vim'


" ===== Add all your plugins here  =====
" (note older versions of Vundle used Bundle instead of Plugin)

Plugin 'Valloric/YouCompleteMe'

" Class structure viewer
Plugin 'majutsushi/tagbar'

" Move like AceJump in Emacs
Plugin 'easymotion/vim-easymotion'

" Color scheme
Plugin 'jnurmine/Zenburn'
Plugin 'altercation/vim-colors-solarized'

" File/folder navigation
Plugin 'scrooloose/nerdtree'
Plugin 'jistr/vim-nerdtree-tabs'

" Super searching
Plugin 'kien/ctrlp.vim'

" Powerful status bar
Plugin 'Lokaltog/powerline', {'rtp': 'powerline/bindings/vim/'}

" Out-of-box python development
"Plugin 'klen/python-mode'
"Plugin 'vim-syntastic/syntastic'
"Plugin 'nvie/vim-flake8'

" Git integration
"Plugin 'tpope/vim-fugitive'
Plugin 'airblade/vim-gitgutter'

" Navigate between tmux and vim seamlessly
Plugin 'christoomey/vim-tmux-navigator'

" ===== All of your Plugins must be added before the following line =====
call vundle#end()            " required

filetype plugin indent on    " required
set encoding=utf-8


" give you the standard four spaces when you hit tab, ensure your line length
" doesn’t go beyond 80 characters
au BufNewFile,BufRead *.py
    \ set tabstop=4     |
    \ set softtabstop=4 |
    \ set shiftwidth=4  |
    "\ set textwidth=79  |
    \ set expandtab     |
    \ set autoindent
    "\ set fileformat=unix

" Autocomplete window goes away when you’re done with it
let g:ycm_autoclose_preview_window_after_completion=1

" F8 key will toggle the Tagbar window
nmap <F8> :TagbarToggle<CR>
let g:tagbar_width=50

" Easymotion: enhance default vim search by named jump and highlighting
":nmap <Space> <Leader><Leader>w
let g:EasyMotion_smartcase = 1
map  / <Plug>(easymotion-sn)
omap / <Plug>(easymotion-tn)
map  n <Plug>(easymotion-next)
map  N <Plug>(easymotion-prev)
map <Leader>j <Plug>(easymotion-j)
map <Leader>k <Plug>(easymotion-k)

" Color. Fix scheme missing problem in tmux+iterm2
colorscheme zenburn
set background=dark
set t_Co=256

" Ignore files in NERDTree
let NERDTreeIgnore=['\.pyc$', '\~$'] "ignore files in NERDTree

" Always use the directory you started vim in to search
let g:ctrlp_working_path_mode = 0

" For pymode or other syntatic plugin?
let python_highlight_all=1
syntax on
set nu
set autoread

"默认配置文件路径"
"let g:ycm_global_ycm_extra_conf = '~/.ycm_extra_conf.py'
""打开vim时不再询问是否加载ycm_extra_conf.py配置"
let g:ycm_confirm_extra_conf=0
set completeopt=longest,menu
"python解释器路径"
"let g:ycm_path_to_python_interpreter='/usr/local/bin/python'
""是否开启语义补全"
let g:ycm_seed_identifiers_with_syntax=1
"是否在注释中也开启补全"
"let g:ycm_complete_in_comments=1
"let g:ycm_collect_identifiers_from_comments_and_strings = 0
""开始补全的字符数"
let g:ycm_min_num_of_chars_for_completion=2
"补全后自动关机预览窗口"
"let g:ycm_autoclose_preview_window_after_completion=1
"" 禁止缓存匹配项,每次都重新生成匹配项"
let g:ycm_cache_omnifunc=0
"字符串中也开启补全"
"let g:ycm_complete_in_strings = 1
""离开插入模式后自动关闭预览窗口"
autocmd InsertLeave * if pumvisible() == 0|pclose|endif
"回车即选中当前项"
"inoremap <expr> <CR>       pumvisible() ? '<C-y>' : '\<CR>'     
""上下左右键行为"
inoremap <expr> <Down>     pumvisible() ? '\<C-n>' : '\<Down>'
inoremap <expr> <Up>       pumvisible() ? '\<C-p>' : '\<Up>'
inoremap <expr> <PageDown> pumvisible() ? '\<PageDown>\<C-p>\<C-n>' : '\<PageDown>'
inoremap <expr> <PageUp>   pumvisible() ? '\<PageUp>\<C-p>\<C-n>' : '\<PageUp>'



