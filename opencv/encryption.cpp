#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
//#include <stdlib.h>
using namespace cv;
using namespace std;

//对图片进行置乱
Mat createChaoticImage(int row, int col, float init) {
    //定义一个数组
    //常用的混沌系统Logistic
    // f(n+1) = 1-2*f(n)^2  -1 << f(n) << 1
    float chaotic[row * col * 3];
    chaotic[0] = init;
    for (int i = 1; i < row * col * 3; i++)
        chaotic[i] = 1 - 2 * chaotic[i - 1] * chaotic[i - 1];
    int tmp;
    //将混沌序列的值调整为8位像素值区间，即[0,255]
    for (int i = 0; i < row * col * 3; i++) {
        tmp = chaotic[i] * 10000;
        chaotic[i] = tmp % 256;
    }
    //定义一个混沌图像
    Mat chaoticImgT(row, col, CV_8UC3);
    //给混沌图像赋予初始值
    int flag = 0;
    for (int y = 0; y < row; y++) {
        uchar* chaoticImgR = chaoticImgT.ptr<uchar>(y);
        for (int x = 0; x < col; x++) {
            chaoticImgR[3 * x] = chaotic[flag];
            chaoticImgR[3 * x + 1] = chaotic[flag + row * col];
            chaoticImgR[3 * x + 2] = chaotic[flag + 2 * row * col];
            flag = flag + 1;
        }
    }
    return chaoticImgT;
}
//读取混沌初始值用来生成混沌图像
// double t = 0.98;
double t = 0.981;
int addEncryption(string filename) {
    //导入需要加密的图像,异或加密
    // Mat srcImage = imread("/root/image/huaqian.jpg", IMREAD_GRAYSCALE);
    // Mat srcImage = imread(filename, IMREAD_COLOR);
    Mat srcImage = imread(filename);
    //调用函数CreateChaoImage，生成混沌图像
    Mat chaoticImg = createChaoticImage(srcImage.rows, srcImage.cols, t);
    //完成图像异或加密
    Mat dstImage;
    bitwise_xor(srcImage, chaoticImg, dstImage);
    imwrite("./image/dst/encryption.bmp", dstImage);
    cout << "加密后的图片: " << endl <<  dstImage << endl;
    return 0;
}

int getEncryption(string filename) {
    //读入需要解密的图像
    Mat img = imread(filename);
    //调用函数，生成混沌图像
    Mat chaoticImg = createChaoticImage(img.rows, img.cols, t);

    Mat dstImage;
    bitwise_xor(img, chaoticImg, dstImage);
    imwrite("./image/dst/aaaa.jpg", dstImage);
    return 0;
}

//多级哈尔小波
void haar() {
    Mat img = imread("/root/image/test.jpg", IMREAD_GRAYSCALE);
    int height = img.cols;
    int width = img.rows;
    int depth = 3;  //定义分解深度
    int depthCount = 1;

    Mat tmp = Mat::ones(width, height, CV_32FC1);
    Mat wavelet = Mat::ones(width, height, CV_32FC1);
    Mat imgtmp = img.clone();
    imgtmp.convertTo(imgtmp, CV_32FC1);
    while (depthCount <= depth) {
        width = img.rows / depthCount;
        height = img.cols / depthCount;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height / 2; j++) {
                tmp.at<float>(i, j) = (imgtmp.at<float>(i, 2 * j) +
                                       imgtmp.at<float>(i, 2 * j + 1)) /
                                      2;
                tmp.at<float>(i, j + height / 2) =
                    (imgtmp.at<float>(i, 2 * j) -
                     imgtmp.at<float>(i, 2 * j + 1)) /
                    2;
            }
        }
        for (int i = 0; i < width / 2; i++) {
            for (int j = 0; j < height; j++) {
                wavelet.at<float>(i, j) =
                    (tmp.at<float>(2 * i, j) + tmp.at<float>(2 * i + 1, j)) / 2;
                wavelet.at<float>(i + width / 2, j) =
                    (tmp.at<float>(2 * i, j) - tmp.at<float>(2 * i + 1, j)) / 2;
            }
        }
        imgtmp = wavelet;
        depthCount++;
    }

    wavelet.convertTo(wavelet, CV_8UC1);
    imwrite("./image/dst/wavelet.bmp", wavelet);
}
int main(int argc, char* argv[]) {
    string control;
    if (argc == 2) control = argv[1];

    string input = "/root/image/huaqian.jpg";
    string output = "./image/dst/encryption.bmp";
    if (control == "enc") {
        addEncryption(input);
    } else if (control == "dec") {
        getEncryption(output);
    } else {
        cout << "Eg: ./aa dec or ./aa  enc" << endl;
    }
    return 0;
}
