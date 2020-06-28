#!/bin/bash

#g++ -std=c++11 -I/usr/local/mysqlpp/include/mysql++ -I/usr/include/mysql/ -L/usr/local/mysqlpp/lib  -lmysqlpp -Wl,-rpath=/usr/local/mysqlpp/lib  main.cpp  -o test
g++ -std=c++11 -I/dev/deps/mysqlpp/include/mysql++ -I/dev/deps/mysql/include -L/dev/deps/mysqlpp/lib  -lmysqlpp -Wl,-rpath=/dev/deps/mysqlpp/lib  main.cpp  -o test

if [[ $? == 0 ]];then
    echo 编译成功
    ./test
else
    echo 编译失败
fi
