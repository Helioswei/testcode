#!/bin/bash

g++ -std=c++11 -g -I/usr/local/poco/include  -L/usr/local/poco/lib  -lPocoFoundation -lPocoXML -Wl,-rpath=/usr/local/poco/lib  main.cpp  -o test

if [[ $? == 0 ]];then
    echo 编译成功
    ./test
else
    echo 编译失败
fi
