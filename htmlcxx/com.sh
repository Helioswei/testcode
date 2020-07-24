#!/bin/bash

/usr/bin/g++  -I/usr/local/htmlcxx/include  -L/usr/local/htmlcxx/lib  -lhtmlcxx    -Wl,-rpath=/usr/local/htmlcxx/lib  ./main.cpp  -o test

if [[ $? == 0 ]];then
    echo 编译成功
    ./test
else
    echo 编译失败
fi
