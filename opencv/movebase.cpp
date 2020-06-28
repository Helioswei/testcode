#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    // 9,帧间差分法,三帧发
    //VideoCapture capture("/root/iptest.mp4");
    VideoCapture capture("/root/huaqian.mp4");
    if (!capture.isOpened()) {
        cout << "Could not open video" << endl;
        return -1;
    }
    //视频的写入
    VideoWriter writer;
    Size videoSize(capture.get(CAP_PROP_FRAME_WIDTH),
                   capture.get(CAP_PROP_FRAME_HEIGHT));
    double rate = capture.get(CAP_PROP_FPS);
    writer.open("/root/test.mp4", capture.get(CAP_PROP_FOURCC), rate, videoSize);
    Mat firstImage, previousImage, resultImage1, resultImage2, resultImage;
    for (;;) {
        Mat currentImage;
        if (!capture.read(currentImage)) break;  //说明已经读取到了最后一帧
        if (firstImage.empty()) {
            firstImage = currentImage;
            continue;
        }
        if (previousImage.empty()) {
            previousImage = currentImage;
            continue;
        }
        //计算前两帧的差值
        absdiff(previousImage, firstImage, resultImage1);
        //计算第三帧与前一帧的差值
        absdiff(currentImage, previousImage, resultImage2);
        //图像与运算
        bitwise_and(resultImage1, resultImage2, resultImage);
        string resuName = "/root/res/" +
                          to_string(capture.get(CAP_PROP_POS_MSEC) / 1000) +
                          ".jpg";
        // imwrite(resuName, resultImage);
        //将结果转换为灰度图
        cvtColor(resultImage, resultImage, COLOR_BGR2GRAY);
        //二值化，像素
        threshold(resultImage, resultImage, 20, 255.0, THRESH_BINARY);
        //去除图像额噪声，先膨胀再腐蚀
        dilate(resultImage, resultImage, Mat());  //膨胀
        erode(resultImage, resultImage, Mat());   //腐蚀
        cout << capture.get(CAP_PROP_POS_MSEC)/1000 << endl;
        string imgName = "/root/res/" + to_string(capture.get(CAP_PROP_POS_MSEC)/1000) + ".jpg";
        imwrite(imgName, resultImage);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(resultImage, contours, hierarchy, RETR_EXTERNAL,
                     CHAIN_APPROX_SIMPLE, Point(0,0));
        vector<vector<Point>> contoursPoly(contours.size());
        vector<Rect> boundRect(contours.size());
        for (int i = 0; i < contours.size(); i++){
            approxPolyDP(Mat(contours[i]), contoursPoly[i], 3, true);
            boundRect[i] = boundingRect(Mat(contoursPoly[i]));
        }
        for (int i = 0; i < contours.size(); i++) {
            //忽略小轮廓
            if (contourArea(contours[i]) < 300) continue;

            //计算轮廓的边界框，在当前帧中画出边框
            RNG rng(12345);
            Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255),
                                  rng.uniform(0, 255));
            rectangle(currentImage, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
        }
        //将帧保存下来
        writer.write(currentImage);
        firstImage = previousImage;
        previousImage = currentImage;
    }
    capture.release();
    writer.release();

    return 0;
}

