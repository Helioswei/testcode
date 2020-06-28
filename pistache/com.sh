#!/bin/bash
g++ -std=c++14 -I/usr/local/pistache/include -I/opt/deps/jsoncpp/include -L/usr/local/pistache/lib64 -lpistache -L/opt/deps/jsoncpp/lib/5.1.0 -ljson_linux-gcc-5.1.0_libmt  -Wl,-rpath=/usr/local/pistache/lib64/ -Wl,-rpath=/usr/local/lib64 -Wl,-rpath=/opt/deps/jsoncpp/lib/5.1.0 restserver.cpp -o pist
#g++ -std=c++14 -I/usr/local/pistache/include  -L/usr/local/pistache/lib64 -lpistache   -Wl,-rpath=/usr/local/pistache/lib64/ -Wl,-rpath=/usr/local/lib64  restserver.cpp -o pist
