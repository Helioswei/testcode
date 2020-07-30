#!/bin/bash

#g++  -std=c++11 -g -I/usr/local/amqpcpp/include  -L/usr/local/amqpcpp/lib  -lamqpcpp    -Wl,-rpath=/usr/local/amqpcpp/lib  ./main.cpp  -o test
/usr/bin/g++  -std=c++11 -g -I/usr/local/amqpcpp/include  -L/usr/local/amqpcpp/lib  -lamqpcpp -lpthread -ldl -lev -Wl,-rpath=/usr/local/amqpcpp/lib  ./evpublish.cpp  -o publish
#/usr/bin/g++  -std=c++11 -g -I/usr/local/amqpcpp/include  -L/usr/local/amqpcpp/lib  -lamqpcpp -lpthread -ldl -lev  -Wl,-rpath=/usr/local/amqpcpp/lib  ./evconsume.cpp  -o consume
exit
if [[ $? == 0 ]];then
    echo 编译成功
    ./test
else
    echo 编译失败
fi
