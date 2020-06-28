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

//求模运算

int mymod(int number, int mod) { return (number % mod + mod) % mod; }

//小波分解
Mat waveletDecompose(Mat src, int level) {
    int col = src.cols;
    int row = src.rows;

    //对待处理的像素
    Mat tmpSrc = src.clone();
    tmpSrc.convertTo(tmpSrc, CV_32FC1);

    //每次经过小波后产生的图片
    Mat wavelet = Mat::ones(row, col, CV_32FC1);
    for (int m = 0; m < level; m++) {
        //列变换后的像素
        Mat tmp = Mat::ones(row, col, CV_32FC1);

        //水平方向变换
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col / 2; j++) {
                tmp.at<float>(i, j) = (tmpSrc.at<float>(i, 2 * j) +
                                       tmpSrc.at<float>(i, 2 * j + 1)) /
                                      2;
                tmp.at<float>(i, j + col / 2) =
                    (tmpSrc.at<float>(i, 2 * j) -
                     tmpSrc.at<float>(i, 2 * j + 1)) /
                    2;
            }
        }

        //垂直方向变换
        for (int i = 0; i < row / 2; i++) {
            for (int j = 0; j < col; j++) {
                wavelet.at<float>(i, j) =
                    (tmp.at<float>(2 * i, j) + tmp.at<float>(2 * i + 1, j)) / 2;
                wavelet.at<float>(i + row / 2, j) =
                    (tmp.at<float>(2 * i, j) - tmp.at<float>(2 * i + 1, j)) / 2;
            }
        }
        //当进行下一层的小波时，分辨率减半，波后的图片为这个源图片
        tmpSrc = wavelet;
        col = col / 2;
        row = row / 2;
    }
    //返回小波
    return wavelet;
}
//小波还原
Mat waveletRecover(Mat waveImg, int level) {
    //每一个循环把一个级数的小波还原
    for (int m = 0; m < level; m++) {
        //在原图上获取当前级数下的区域图片，会共享内存的
        Mat tmp = waveImg(Rect(0, 0, waveImg.cols / pow(2, level - m - 1),
                               waveImg.rows / pow(2, level - m - 1)));
        //列变换逆
        Mat recoverSrc(tmp.rows, tmp.cols, CV_32FC1);
        for (int i = 0; i < recoverSrc.rows / 2; i++) {
            for (int j = 0; j < recoverSrc.cols; j++) {
                float value1 = tmp.at<float>(i, j);
                float value2 = tmp.at<float>(i + recoverSrc.rows / 2, j);
                recoverSrc.at<float>(2 * i, j) = value1 + value2;
                recoverSrc.at<float>(2 * i + 1, j) = value1 - value2;
            }
        }

        //行变换逆
        Mat tmp2 = recoverSrc.clone();
        for (int i = 0; i < tmp2.rows; i++) {
            for (int j = 0; j < tmp2.cols / 2; j++) {
                float value1 = recoverSrc.at<float>(i, j);
                float value2 = recoverSrc.at<float>(i, j + tmp2.cols / 2);
                tmp2.at<float>(i, 2 * j) = value1 + value2;
                tmp2.at<float>(i, 2 * j + 1) = value1 - value2;
            }
        }
        tmp2.copyTo(tmp);
    }
    Mat original = waveImg.clone();
    // original.convertTo(original, CV_8UC1);
    return original;
}

// N = 6,当N = 186时，可以恢复
Mat Arnold(Mat srcImage, int n) {
    int row = srcImage.rows;
    int col = srcImage.cols;

    Mat dstImage = Mat::ones(row, col, CV_32FC1);
    Mat tmp = srcImage.clone();
    tmp.convertTo(tmp, CV_32FC1);
    for (int i = 0; i < n; i++) {
        for (int x = 0; x < row; x++) {
            for (int y = 0; y < col; y++) {
                int newX = mymod((x + y), row);
                int newY = mymod((x + 2 * y), row);
                dstImage.at<float>(newX, newY) = tmp.at<float>(x, y);
            }
        }
        dstImage.copyTo(tmp);
    }
    dstImage.convertTo(dstImage, CV_8UC1);
    return dstImage;
}

Mat addWaterPic(Mat srcImage, Mat waterImage, int level) {
    //对原图进行二级小波分解
    Mat waveImg = waveletDecompose(srcImage, level);
    int col = srcImage.cols;
    int row = srcImage.rows;
    //获取小波分解后的LL的区域
    int dstCol = col / pow(2, level);
    int dstRow = row / pow(2, level);
    Mat llImg = waveImg(Rect(0, 0, dstCol, dstRow));
    llImg.convertTo(llImg, CV_32FC1);
    Mat waterSrc = waterImage.clone();
    waterSrc.convertTo(waterSrc, CV_32FC1);

    Mat tmp1 = Mat::ones(dstRow, dstCol, CV_32FC1);
    Mat tmp2 = Mat::ones(dstRow, dstCol, CV_32FC1);
    Mat kk = Mat::ones(dstRow, dstCol, CV_32FC1);
    Mat key = Mat::ones(dstRow, dstCol, CV_32FC1);
    for (int i = 0; i < dstRow; i++) {
        for (int j = 0; j < dstCol; j++) {
            tmp1.at<float>(i, j) =
                llImg.at<float>(i, j) + 0.1 * waterSrc.at<float>(i, j);
            tmp2.at<float>(i, j) = round(tmp1.at<float>(i, j) / 0.25);
            llImg.at<float>(i, j) = tmp2.at<float>(i, j) * 0.25;
            kk.at<float>(i, j) = mymod(llImg.at<float>(i, j), 2);
        }
    }
    bitwise_xor(kk, waterSrc, key);
    imwrite("./image/dst/key.bmp", key);
    Mat dstImage = waveletRecover(waveImg, level);
    return dstImage;
}

Mat getWaterPic(Mat srcImage, int level) {
    //对加水印后的图进行二级小波分解
    Mat waveImg = waveletDecompose(srcImage, level);
    int col = srcImage.cols;
    int row = srcImage.rows;
    //获取小波分解后的LL的区域
    int dstCol = col / pow(2, level);
    int dstRow = row / pow(2, level);
    Mat llImg = waveImg(Rect(0, 0, dstCol, dstRow));
    llImg.convertTo(llImg, CV_32FC1);

    Mat tmp1 = Mat::ones(dstRow, dstCol, CV_32FC1);
    Mat tmp2 = Mat::ones(dstRow, dstCol, CV_32FC1);
    Mat kk = Mat::ones(dstRow, dstCol, CV_32FC1);
    Mat water = Mat::ones(dstRow, dstCol, CV_32FC1);
    for (int i = 0; i < dstRow; i++) {
        for (int j = 0; j < dstCol; j++) {
            tmp1.at<float>(i, j) = round(llImg.at<float>(i, j) / 0.25);
            tmp2.at<float>(i, j) = tmp1.at<float>(i, j) * 0.25;
            kk.at<float>(i, j) = mymod(tmp2.at<float>(i, j), 2);
        }
    }
    Mat key = imread("./image/dst/key.bmp", IMREAD_GRAYSCALE);
    if (key.rows != kk.rows || key.cols != kk.cols){
       resize(key, key, Size(kk.cols, kk.rows)); 
    }
    key.convertTo(key, CV_32FC1);
    bitwise_xor(kk, key, water);
    //对置乱水印进行还原
    Mat waterHuan = Arnold(water, 186);
    return waterHuan;
}

int main(int argc, char* argv[]) {
    // string srcFile = "/root/squa.jpg";
    //string srcFile = "./image/dst/aa.bmp";
    //Mat srcImage = imread(srcFile, IMREAD_GRAYSCALE);
    int N = 6;  //当N=186时可以还原
    if (argc == 2) {
        N = atoi(argv[1]);
    }
    //Mat dstImage1 = Arnold(srcImage, N);
    //imwrite("./image/dst/arnold.jpg", dstImage1);
    //return 0;

    Mat waSrc = imread("/root/squwater.jpg", IMREAD_GRAYSCALE);
    Mat waterSrc = imread("/root/squa.jpg", IMREAD_GRAYSCALE);
    //对水印进行置乱
    Mat water = Arnold(waterSrc, N);
    int level = 2;
    Mat dstImage = addWaterPic(waSrc, water, level);
    imwrite("./image/dst/outimg.jpg", dstImage);
    Mat outImg = imread("./image/dst/crop.jpg");
    Mat aa = getWaterPic(outImg, level);
    imwrite("./image/dst/aa.jpg", aa);

    // Mat wa1 = waveletDecompose(waSrc, level);
    // imwrite("./image/dst/wa1.bmp", wa1);
    // Mat aa = waveletRecover(wa1, level);
    // imwrite("./image/dst/aa.jpg", aa);
    return 0;
}
