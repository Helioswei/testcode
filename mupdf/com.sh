#!/bin/bash
g++ -I/usr/local/helios/include -o testpdf ./testpdf.cpp /usr/local/helios/lib/libmupdf.a \
    /usr/local/helios/lib/libmupdf-third.a -lm 
