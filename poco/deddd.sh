#!/bin/bash
#1,检查网络状态
ping www.baidu.com -w 2
if [[ $? != 0 ]];then
	echo "请检查你的网络状态，无法联网"
	exit
fi
#2,下载需要的依赖库
yum install -y git
yum install -y python-devel python3-devel
yum install -y ruby ruby-devel
yum install -y perl-devel perl perl-ExtUtils-ParseXS
yum install -y perl-ExtUtils-XSpp
yum install -y perl-ExtUtils-CBuilder
yum install -y perl-ExtUtils-Embed
yum install -y lua lua-devel luajit luajit-devel
yum install -y libX11-devel
yum install -y gtk2-devel gtk3-ncurses-devel
yum install -y ncurses-devel
yum install -y gcc-c++
#3,下载vim源码
git clone https://github.com/vim/vim.git
#4,进入vim目录进行源码编译
cd ${PWD}/vim
./configure \
--with-features=huge \
--enable-multibyte \
--enable-rubyinterp=yes \
--enable-pythoninterp=yes \
--with-python-config-dir=/usr/lib64/python2.7/config/ \
--enable-perlinterp=yes \
--enable-luainterp=yes \
--enable-gui=gtk2 \
--enable-cscope \
--prefix=/usr \

make
make install
#5,下载vim包管理软件
git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim

