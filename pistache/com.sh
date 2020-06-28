#!/bin/bash
g++ -std=c++14 -I/usr/local/pistache/include -L/usr/local/pistache/lib64 -lpistache  -Wl,-rpath=/usr/local/pistache/lib64/ -Wl,-rpath=/usr/local/lib64 restserver.cpp -o pist
