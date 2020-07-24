#!/bin/bash
g++ -std=c++11  -I/usr/local/include/webdriverxx/   -L/usr/local/libcurl/lib -lcurl  -lpthread -Wl,-rpath=/usr/local/lib  ./main.cpp  -o test

if [[ $? == 0 ]];then
    echo 编译成功
    ./test
else
    echo 编译失败
fi
