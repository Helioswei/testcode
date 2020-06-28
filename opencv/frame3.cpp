#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {
    // 9,帧间差分法,三帧差分
    // VideoCapture capture("/root/huaqian.mp4");
    VideoCapture capture("/root/iptest.mp4");
    if (!capture.isOpened()) {
        cout << "Could not open video" << endl;
        return -1;
    }
    Mat firstImage, previousImage, resultImage1, resultImage2, resultImage;
    map<double, float> percents;
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
        //统计两帧相减后的图像数
        float counter = 0;
        float num = 0;
        for (int y = 0; y < resultImage.rows; y++) {
            uchar* data = resultImage.ptr<uchar>(y);  //获取每一行的指针
            for (int x = 0; x < resultImage.cols; x++) {
                num = num + 1;
                if (data[x] == 255)  //访问到像素值
                    counter = counter + 1;
            }
        }
        float p = counter / num;
        percents.insert(pair<double, float>(capture.get(CAP_PROP_POS_MSEC), p));
        string per = to_string(p);
        cout << "counter: " << counter << " num: " << num << " p: " << p
             << endl;
        ofstream myfile;
        myfile.open("/root/test.txt", ios::app);
        myfile << "time: " << capture.get(CAP_PROP_POS_MSEC) / 1000
               << " p: " << per << endl;
        myfile.close();
        // counter ,num, p 分别为有变化的像素点数，总像素点数目，比例
        if (p > 0.25)  //达到阈值的像素数达到一定的数量则保存该图像
        {
            cout << "this frame is keyframe" << endl;
            //获取当前帧的时间
            double frameTime = capture.get(CAP_PROP_POS_MSEC) / 1000;

            string imgName = "/root/test/" + to_string(frameTime) + ".jpg";
            imwrite(imgName, currentImage);
        } else {
            cout << "this frame is not keyframe " << endl;
        }
        firstImage = previousImage;
        previousImage = currentImage;
    }
    capture.release();
    map<double, float>::iterator it;
    float max;
    double mytime;
    for (it = percents.begin(); it != percents.end(); it++) {
        if (it->second > max) {
            max = it->second;
            mytime = it->first;
        }
    }
    cout << "mytime: " << mytime << " max: " << max << endl; 
    return 0;
}

