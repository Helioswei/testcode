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

//最低有效位，保存为bmp格式的才能更好的获取水印
void addBlindWater() {
    // 1，通道分离
    Mat img = imread("/root/image/test.jpg", IMREAD_COLOR);
    // Mat img = imread("/root/image/huaqian.jpg");
    //获取图像的大小
    int row, col;
    row = img.rows;
    col = img.cols;
    vector<Mat> allPlanes;
    split(img, allPlanes);

    //读取水印图片
    // Mat wmImg = imread("/root/image/huaqian.jpg", IMREAD_UNCHANGED);
    Mat wmImg = imread("/root/image/huaqian.jpg", IMREAD_GRAYSCALE);
    //将图像转换为二值图像
    threshold(wmImg, wmImg, 127, 1, THRESH_BINARY);
    //保留最低有效位LSB即可
    //生成掩码图像，其值为254,
    Mat t(row, col, CV_8UC1, 254);
    Mat embedSrc;
    t.copyTo(embedSrc);
    //将bgr[0]与t进行按位与运算，只保留bgr[0]的高7位，最低位置0
    bitwise_and(allPlanes[0], t, embedSrc);
    //将水印图片扩展和原图一样
    //将水印图片嵌入到载体图像的最低位
    copyMakeBorder(wmImg, wmImg, 0, embedSrc.rows - wmImg.rows, 0,
                   embedSrc.cols - wmImg.cols, BORDER_WRAP, Scalar::all(0));
    bitwise_or(embedSrc, wmImg, embedSrc);
    //将含水印图像赋给bgR[0]
    embedSrc.copyTo(allPlanes[0]);
    //组合RGB图像
    merge(allPlanes, embedSrc);
    //完成色彩空间转换
    // cvtColor(embedSrc, embedSrc, COLOR_BGR2RGB);

    //imwrite("./image/dst/embed.bmp", embedSrc);
    imwrite("./image/dst/embed.jpg", embedSrc);
}

void getBindWater() {
    //Mat srcImage = imread("./image/dst/embed.bmp");
    Mat srcImage = imread("./image/dst/embed.jpg");
    //获取图片的大小
    int row, col;
    row = srcImage.rows;
    col = srcImage.cols;

    //分离图像色彩空间
    vector<Mat> allPlanes;
    split(srcImage, allPlanes);

    //定义掩码图像，该图像的值为1,
    Mat t(row, col, CV_8UC1, 1);
    Mat embedSrcExtract;
    t.copyTo(embedSrcExtract);
    //将t与bgr【0】进行按位与计算
    Mat ExtractWM;
    bitwise_and(allPlanes[0], t, ExtractWM);
    Mat t2(row, col, CV_8UC1, 254);
    //将bgr[0]与t2进行按位与运算，实现提前bgr[0]的高7位
    bitwise_and(allPlanes[0], t2, allPlanes[0]);
    merge(allPlanes, embedSrcExtract);
    //提取出来的水印图像，只有0和1两个值，将1转换为255
    threshold(ExtractWM, ExtractWM, 0, 255, THRESH_BINARY);
    imwrite("./image/embedSrcExtract.jpg", embedSrcExtract);
    imwrite("./image/ExtractWM.jpg", ExtractWM);
}
//随机位盲水印
void addRandomBlindWater() {
    Mat srcImage = imread("/root/image/test.jpg");
    //获取图片大小
    int row, col;
    row = srcImage.rows;
    col = srcImage.cols;
    //通道分解
    vector<Mat> allPlanes;
    split(srcImage, allPlanes);

    //读入水印图片
    Mat wmImage = imread("/root/image/huaqian.jpg", IMREAD_UNCHANGED);
    //调整二值水印图像，将其中的值由255调整为1
    threshold(wmImage, wmImage, 127, 1, THRESH_BINARY);
    //保留最低有效位lsb即可
    //生成混沌序列，用来决定水印嵌入在哪一位，混沌序列chaoticInt的值为0-7
    float chaoticF[row * col];
    //混沌系统初始值
    double t;
    t = 0.98;
    chaoticF[0] = t;
    //生成混沌序列
    int i = 0, j = 0;
    for (i = 1; i < row * col; i++)
        chaoticF[i] = 1 - 2.0 * chaoticF[i - 1] * chaoticF[i - 1];
    //生成嵌入位置序列，该序列内的值为[0,7]
   int points = row * col; 
    int chaoticInt[points];
    int mypoints = row * col;

    cout << mypoints << endl;
    for (i = 0; i < row * col; i++) {
        //int b = chaoticF[i] * 10000;
        //int c = abs(b % 8);
        std::cout << i << std::endl;
        //chaoticInt[i] = c;
    }
    //嵌入水印，将将特定位与水印信息进行比较
    // 1，载体图片指定位置为1，将该位的1置为0， 通过将该像素值减去2^n实现
    // 2，载体图片指定位置为0，将该位的0置为1，通过将该像素加上2^n来实现
    int flag = 0;
    int n;
    for (i = 0; i < row; i++)
        for (j = 0; j < col; j++) {
            n = chaoticInt[flag];
            if (((allPlanes[0].at<uchar>(i, j) >> n) % 2) !=
                wmImage.at<uchar>(i, j))

                if (((allPlanes[0].at<uchar>(i, j) >> n) % 2) == 1)
                    allPlanes[0].at<uchar>(i, j) =
                        allPlanes[0].at<uchar>(i, j) - pow(2, n);
                else
                    allPlanes[0].at<uchar>(i, j) =
                        allPlanes[0].at<uchar>(i, j) + pow(2, n);
            flag++;
        }
    //合并通道
    Mat embedSrc;
    merge(allPlanes, embedSrc);
    imwrite("./image/randembedSrc.bmp", embedSrc);
}

int main(int argc, char* argv[]) {
    string control;
    if (argc == 2) control = argv[1];

    if (control == "enc") {
         addBlindWater();
        //addRandomBlindWater();
    } else if (control == "dec") {
         getBindWater();
    } else {
        cout << "Eg: ./aa dec or ./aa  enc" << endl;
    }
    return 0;
}
