#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
using namespace cv;
using namespace std;

//感知哈希算法比较图片
int aHash (Mat matSrc1, Mat matSrc2);
int main(int argc, char* argv[]) {
    // 9,感知哈希算法比较图片的相似度
    VideoCapture capture("/root/huaqian.mp4");
    // VideoCapture capture("/root/iptest.mp4");
    if (!capture.isOpened()) {
        cout << "Could not open video" << endl;
        return -1;
    }
    Mat previousImage;
    for (;;) {
        Mat currentImage;
        if (!capture.read(currentImage)) break;  //说明已经读取到了最后一帧
        if (previousImage.empty()) {
            previousImage = currentImage;
            continue;
        }

        int p = aHash(previousImage, currentImage);
        //获取当前帧的时间
        double frameTime = capture.get(CAP_PROP_POS_MSEC) / 1000;
        ofstream myfile;
        myfile.open("/root/log", ios::app);
        myfile << "time: " << frameTime << " p: " << p << endl;
        myfile.close();
        cout << "time: " << frameTime << " p: " << p << endl;
        if (p > 25 )  //达到阈值的像素数达到一定的数量则保存该图像
        {
            cout << "this frame is keyframe" << endl;

            string imgName = "/root/test/" + to_string(frameTime) + ".jpg";
            imwrite(imgName, currentImage);
        } else {
            cout << "this frame is not keyframe " << endl;
        }
        previousImage = currentImage;
    }
    capture.release();

    return 0;
}

int aHash (Mat matSrc1, Mat matSrc2)
{
    Mat matDst1, matDst2;
    resize(matSrc1, matDst1, Size(8,8), 0, 0, INTER_CUBIC);
    resize(matSrc2, matDst2, Size(8,8), 0, 0, INTER_CUBIC);

    cvtColor(matDst1, matDst1, COLOR_BGR2GRAY);
    cvtColor(matDst2, matDst2, COLOR_BGR2GRAY);

    int iAvg1 = 0, iAvg2 = 0;
    int arr1[64], arr2[64];

    for (int i = 0; i < 8; i++){
        uchar* data1 = matDst1.ptr<uchar>(i);
        uchar* data2 = matDst2.ptr<uchar>(i);

        int tmp = i * 8;
        for (int j = 0; j < 8; j++){
            int tmp1 = tmp + j;
            arr1[tmp1] = data1[j] / 4 * 4;
            arr2[tmp1] = data2[j] / 4 * 4;

            iAvg1 += arr1[tmp1];
            iAvg2 += arr2[tmp1];
        
        }
    
    }
    iAvg1 /= 64;
    iAvg2 /= 64;

    for (int i = 0; i < 64; i++){
        arr1[i] = (arr1[i] >= iAvg1) ? 1:0;
        arr2[i] = (arr2[i] >= iAvg2)? 1:0;
    
    }

    int iDiffNum = 0;
    for (int i = 0; i < 64; i++){
        if (arr1[i] != arr2[i])
            ++ iDiffNum;
    }

    return iDiffNum;
}
