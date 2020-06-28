#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>  //for controlling floaat print precision
using namespace cv;
using namespace std;

// ssim取值范围[0,1]，值越大，表示图片失帧越小
double getMSSIM(const Mat& firstImage, const Mat& secondImage);

// PSNR检测
double getPSNR(const Mat& I1, const Mat& I2);
int main(int argc, char* argv[]) {
    // 9,mssim算法比较图片的相似度
    VideoCapture capture("/root/huaqian.mp4");
    // VideoCapture capture("/root/iptest.mp4");
    if (!capture.isOpened()) {
        cout << "Could not open video" << endl;
        return -1;
    }
    Mat previousImage;
    int psnrTriggerValue = 11;
    for (;;) {
        Mat currentImage;
        if (!capture.read(currentImage)) break;  //说明已经读取到了最后一帧
        if (previousImage.empty()) {
            previousImage = currentImage;
            continue;
        }
        //获取当前帧的时间
        double frameTime = capture.get(CAP_PROP_POS_MSEC) / 1000;

        double psnr = getPSNR(previousImage, currentImage);

        if (psnr < psnrTriggerValue && psnr) {
            double p = getMSSIM(previousImage, currentImage);
            ofstream myfile;
            myfile.open("/root/log", ios::app);
            myfile << "time: " << frameTime << " psnr: " << psnr << " p: " << p
                   << endl;
            myfile.close();
            cout << "time: " << frameTime << "  psnr: " << psnr << " p: " << p
                 << endl;
            //if (p < 0.5)  //达到阈值的像素数达到一定的数量则保存该图像
            //{

                string imgName = "/root/test/" + to_string(frameTime) + ".jpg";
                imwrite(imgName, currentImage);
            //}
        }
        previousImage = currentImage;
    }
    capture.release();

    return 0;
}

double getMSSIM(const Mat& firstImage, const Mat& secondImage) {
    const double c1 = 6.5025, c2 = 58.5225;
    int d = CV_32F;
    Mat first, second;
    firstImage.convertTo(first, d);
    secondImage.convertTo(second, d);

    Mat first_2 = first.mul(first);       // first^2
    Mat second_2 = second.mul(second);    // secode^2
    Mat firstSecond = first.mul(second);  // first * second

    Mat mu1, mu2;
    GaussianBlur(first, mu1, Size(11, 11), 1.5);
    GaussianBlur(second, mu2, Size(11, 11), 1.5);
    Mat mu1_2 = mu1.mul(mu1);
    Mat mu2_2 = mu2.mul(mu2);
    Mat mu1Mu2 = mu1.mul(mu2);

    Mat sigmal1_2, sigma2_2, sigma12;
    GaussianBlur(first_2, sigmal1_2, Size(11, 11), 1.5);
    sigmal1_2 -= mu1_2;
    GaussianBlur(second_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
    GaussianBlur(firstSecond, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1Mu2;

    Mat t1, t2, t3;
    t1 = 2 * mu1Mu2 + c1;
    t2 = 2 * sigma12 + c2;
    t3 = t1.mul(t2);
    t1 = mu1_2 + mu2_2 + c1;
    t2 = sigmal1_2 + sigma2_2 + c2;
    t1 = t1.mul(t2);

    Mat ssimMap;
    divide(t3, t1, ssimMap);
    Scalar mssim = mean(ssimMap);
    return (mssim[0] + mssim[1] + mssim[2]) / 3;
}

double getPSNR(const Mat& I1, const Mat& I2) {
    Mat s1;
    absdiff(I1, I2, s1);  //[I1 - I2]
    s1.convertTo(s1, CV_32F);
    s1 = s1.mul(s1);  // |I1 -I2|^2

    Scalar s = sum(s1);                           // sum elements per channel
    double sse = s.val[0] + s.val[1] + s.val[2];  // sum channels
    if (sse <= 1e-10)  // for small values return zero
        return 0;
    else {
        double mse = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}
