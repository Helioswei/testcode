#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
using namespace cv;
using namespace std;


int main(){

    VideoCapture capture("/root/test1.swf");
    if (!capture.isOpened()){
        cout << "Could not open video" << endl;
        return -1;
    }
    int count = 0;
    for(;;){
        Mat frame;
        if (!capture.read(frame)) break;
        string imgName = "/root/test/frame_" + to_string(count) + ".jpg";
        imwrite(imgName, frame);
        count++;
    }

    return 0;
}
