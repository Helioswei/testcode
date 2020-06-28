#!/bin/bash

g++ -std=c++11 -I/usr/local/poco/include -I/usr/include/mysql/ -L/usr/local/poco/lib  -lPocoFoundation -lPocoData -lPocoDataMySQL   -Wl,-rpath=/usr/local/poco/lib  main.cpp  -o test

if [[ $? == 0 ]];then
    echo 编译成功
    ./test
else
    echo 编译失败
fi
