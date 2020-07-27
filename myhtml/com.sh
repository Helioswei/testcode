#!/bin/bash

g++  -g -I/usr/local/myhtml/include  -L/usr/local/myhtml/lib  -lmyhtml    -Wl,-rpath=/usr/local/myhtml/lib  ./main.cpp  -o test

if [[ $? == 0 ]];then
    echo 编译成功
    ./test
else
    echo 编译失败
fi
