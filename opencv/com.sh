#!/bin/bash

g++ -std=c++11 -g -D_GLIBCXX_USE_CXX11_ABI=0 -I$DEPS/opencv/include/opencv4/ -L$DEPS/opencv/lib64 -lopencv_core -lopencv_imgcodecs -lopencv_highgui  -lopencv_imgproc -lopencv_videoio  -Wl,-rpath=$DEPS/opencv/lib64 -Wl,-rpath=$DEPS/video/lib video.cpp -o opencvtest
