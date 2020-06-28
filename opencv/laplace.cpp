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

//离散二维小波
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

double t = 0.98;  //-1 << t << 1
Mat addEncryption(string filename) {
    //导入需要加密的图像,异或加密
    // Mat srcImage = imread("/root/image/huaqian.jpg", IMREAD_GRAYSCALE);
    // Mat srcImage = imread(filename, IMREAD_COLOR);
    Mat srcImage = imread(filename);
    //调用函数CreateChaoImage，生成混沌图像
    Mat chaoticImg = createChaoticImage(srcImage.rows, srcImage.cols, t);
    //完成图像异或加密
    Mat dstImage;
    bitwise_xor(srcImage, chaoticImg, dstImage);
    // imwrite("./image/dst/encryption.bmp", dstImage);
    return dstImage;
}
Mat getEncryption(string filename) {
    //读入需要解密的图像
    Mat img = imread(filename);
    //调用函数，生成混沌图像
    Mat chaoticImg = createChaoticImage(img.rows, img.cols, t);

    Mat dstImage;
    bitwise_xor(img, chaoticImg, dstImage);
    // imwrite("./image/dst/aaaa.jpg", dstImage);
    return dstImage;
}

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

        //进行列变换
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (i < row / 2) {
                    //求均值
                    tmp.at<float>(i, j) = (tmpSrc.at<float>(2 * i, j) +
                                           tmpSrc.at<float>(2 * i + 1, j)) /
                                          2;
                } else {
                    //相邻差值
                    tmp.at<float>(i, j) =
                        (tmpSrc.at<float>(2 * (i - row / 2), j) -
                         tmpSrc.at<float>(2 * (i - row / 2) + 1, j)) /
                        2;
                }
            }
        }
        //在列变换后的像素二位数组上进行行变换
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                if (j < col / 2) {
                    wavelet.at<float>(i, j) = (tmp.at<float>(i, 2 * j) +
                                               tmp.at<float>(i, 2 * j + 1)) /
                                              2;
                } else {
                    wavelet.at<float>(i, j) =
                        (tmp.at<float>(i, 2 * (j - col / 2)) -
                         tmp.at<float>(i, 2 * (j - col / 2) + 1)) /
                        2;
                }
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
        //先恢复行变换得到列变换
        Mat recoverSrc(tmp.rows, tmp.cols, CV_32FC1);
        for (int i = 0; i < recoverSrc.rows; i++) {
            for (int j = 0; j < recoverSrc.cols; j++) {
                if (j % 2 == 0)
                    recoverSrc.at<float>(i, j) =
                        tmp.at<float>(i, j / 2) +
                        tmp.at<float>(i, j / 2 + recoverSrc.cols / 2);
                else
                    recoverSrc.at<float>(i, j) =
                        tmp.at<float>(i, (j - 1) / 2) -
                        tmp.at<float>(i, (j - 1) / 2 + recoverSrc.cols / 2);
            }
        }
        //从列变换得到原图
        Mat tmp2 = recoverSrc.clone();
        for (int i = 0; i < tmp2.rows; i++) {
            for (int j = 0; j < tmp2.cols; j++) {
                if (i % 2 == 0)
                    tmp2.at<float>(i, j) =
                        recoverSrc.at<float>(i / 2, j) +
                        recoverSrc.at<float>(i / 2 + tmp2.rows / 2, j);
                else
                    tmp2.at<float>(i, j) =
                        recoverSrc.at<float>((i - 1) / 2, j) -
                        recoverSrc.at<float>((i - 1) / 2 + tmp2.rows / 2, j);
            }
        }
        tmp2.copyTo(tmp);
    }
    Mat original = waveImg.clone();
    // original.convertTo(original, CV_8UC1);
    return original;
}

Mat addWaterPic(Mat srcImg, Mat waterImg, int level) {
    int col = srcImg.cols;
    int row = srcImg.rows;
    //获取HH3的分辨率
    int dstCol = col / pow(2, level);
    int dstRow = row / pow(2, level);
    //对水印图片进行缩放
    Mat waterResize;
    resize(waterImg, waterResize, Size(dstCol, dstRow));
    waterResize.convertTo(waterResize, CV_8UC1);
    //二值化处理图片
    threshold(waterResize, waterResize, 127, 1, THRESH_BINARY);
    // ofstream myfile;
    // myfile.open("/root/log", ios::app);
    // myfile << "addWaterPic，水印信息： " << endl << waterResize << endl;
    // cout << "水印图片：" << endl << waterResize << endl;

    //获取原图中的HH3的图片位置
    Mat hh3 = srcImg(Rect(dstCol, dstRow, dstCol, dstRow));
    Mat hh3Tmp;
    hh3.copyTo(hh3Tmp);
    //对hh3的图片保留最高7位，最低位为0，
    Mat t(dstRow, dstCol, CV_8UC1, 254);
    Mat embedSrc;
    t.copyTo(embedSrc);
    //将hh3的图片与t进行按位与运算，只保留最高7位，最低位置0
    hh3Tmp.convertTo(hh3Tmp, CV_8UC1);
    bitwise_and(hh3Tmp, t, embedSrc);
    //将水印图片嵌入到hh3中
    bitwise_or(embedSrc, waterResize, embedSrc);
    embedSrc.convertTo(embedSrc, CV_32FC1);
    embedSrc.copyTo(hh3);
    Mat original = srcImg.clone();
    // original.convertTo(original, CV_8UC1);
    return original;
}
Mat getWaterPic(Mat srcImg, int level) {
    //对加水印后的图片进行多层小波变换
    Mat waveImg = waveletDecompose(srcImg, level);
    //获取hh3的位置
    int col = srcImg.cols;
    int row = srcImg.rows;
    //获取HH3的分辨率
    int dstCol = col / pow(2, level);
    int dstRow = row / pow(2, level);
    //获取原图中的HH3的图片位置
    Mat hh3 = waveImg(Rect(dstCol, dstRow, dstCol, dstRow));
    Mat hh3Tmp = hh3.clone();
    //定义掩码图像，该图像的值为1
    Mat t(dstRow, dstCol, CV_8UC1, 1);
    Mat picImg;
    hh3Tmp.convertTo(hh3Tmp, CV_8UC1);
    bitwise_and(hh3Tmp, t, picImg);
    // ofstream myfile;
    // myfile.open("/root/log1", ios::app);
    // myfile << "getWaterPic，水印信息： " << endl << picImg << endl;
    //二值化水印图像
    threshold(picImg, picImg, 0, 255, THRESH_BINARY);
    return picImg;
}

//彩色图片添加数字水印
Mat addWaterToColorImg(Mat srcImg, Mat waterImg, int level) {
    //判读图片是灰度图还是彩图
    vector<Mat> allPlanes;
    split(srcImg, allPlanes);
    Mat padded;
    if (allPlanes.size() > 1) {
        //说明是彩色图
        padded = allPlanes[0];
    } else {
        //说明是灰度图
        padded = srcImg;
    }
    Mat waveImg = waveletDecompose(padded, level);
    Mat pic = addWaterPic(waveImg, waterImg, level);
    Mat soImag = waveletRecover(pic, level);  //还原
    soImag.convertTo(soImag, CV_8UC1);
    Mat lastImg = Mat();
    if (allPlanes.size() > 1) {
        allPlanes[0] = soImag;
        merge(allPlanes, lastImg);
    } else {
        lastImg = soImag;
    }
    return lastImg;
}
//彩色图片获取水印
Mat getWaterFromColorImg(Mat srcImg, int level){
    vector<Mat> allPlanes;
    split(srcImg, allPlanes);
    Mat padded;
    if (allPlanes.size() > 1) {
        //说明是彩色图
        padded = allPlanes[0];
    } else {
        //说明是灰度图
        padded = srcImg;
    }
    Mat soWaterImg = getWaterPic(padded, level);
    return soWaterImg;
} 

int main(int argc, char* argv[]) {
    int level = 3;
    // Mat img = imread("/root/image/test.jpg", IMREAD_GRAYSCALE);
    Mat img = imread("/root/image/test.jpg", IMREAD_COLOR);
    Mat water = imread("/root/image/huaqian.jpg", IMREAD_GRAYSCALE);
    Mat aa = addWaterToColorImg(img, water, level);
    imwrite("./image/dst/aa.jpg", aa);
    Mat bb = imread("./image/dst/aa.jpg", IMREAD_COLOR);
    Mat picImg = getWaterFromColorImg(bb, level);
    imwrite("./image/dst/pic.bmp", picImg);
    return 0;
    // Mat waveletImg = waveletDecompose(img, 3);
    // imwrite("./image/dst/wavelet.bmp", waveletImg);
    // //得到三次小波分解后的图片，对当前图片添加水印
    // Mat recoverImg = waveletRecover(waveletImg, 3);
    // imwrite("./image/dst/recover.bmp", recoverImg);
    // return 0;
    int count = 10;
    int pixMax = 16;
    Mat test(pixMax, pixMax, CV_32FC1);
    for (int i = 0; i < test.rows; i++) {
        for (int j = 0; j < test.cols; j++) {
            test.at<float>(i, j) = count;
            count = count + 2;
        }
    }
    // cout << "源： " << endl << test << endl;
    Mat waveImg = waveletDecompose(img, level);
    imwrite("./image/dst/waveImg.bmp", waveImg);
    // cout << "三次小波变换后的图片：" << endl << waveImg << endl;
    Mat waterImg = imread("/root/image/huaqian.jpg", IMREAD_GRAYSCALE);
    Mat pic = addWaterPic(waveImg, waterImg, level);
    imwrite("./image/dst/pic.bmp", pic);
    // cout << "添加水印后的图片: " << endl << pic << endl;
    Mat soImag = waveletRecover(pic, level);  //还原
    imwrite("./image/dst/so.bmp", soImag);
    // cout << "还原：" << endl << soImag << endl;
    // 对原图进行分辨率的处理
    // Mat resizeImg;
    // resize(soImag, resizeImg, Size(soImag.cols * 0.5, soImag.rows * 0.5));
    //// cout << "分辨率： " << endl << resizeImg << endl;
    // imwrite("./image/dst/resize.bmp", resizeImg);
    //获取水印图片
    // Mat soWaterImg = getWaterPic(resizeImg, level - 1);
    Mat soWaterImg = getWaterPic(soImag, level);
    imwrite("./image/dst/sowater.bmp", soWaterImg);
    // cout << "水印的图片： " << endl << soWaterImg << endl;
}
