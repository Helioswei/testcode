g++ -std=c++11 -g -I${DEPS}/video/include -L${DEPS}/video/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswscale -Wl,-rpath=${DEPS}/video/lib  test.cpp -o test
