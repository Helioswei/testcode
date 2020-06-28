#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
using namespace cv;
using namespace std;

//创建椭圆
void MyEllipse(Mat img, double angle);
//创建圆
void MyFilledCircle(Mat img, Point center);
//创建凸多边形
void MyPolygon(Mat img);
//创建直线
void MyLine(Mat img, Point start, Point end);
#define W 400
int main(int argc, char* argv[]) {
    // 1,混合两个图片g(x)=(1-&)f0(x) + &f1(x)
    // double alpha = 0.5, beta, input;
    // Mat src1, src2, dst;
    // cout << "Simple Linear Blender" << endl;
    // cout << "---------------------" << endl;
    // cout << "*Enter alpha [0-1]:  " << endl;
    // cin >> input;
    // if (input >= 0 && input <= 1) alpha = input;
    // src1 = imread("/root/test.jpg");
    // src2 = imread("/root/test1.jpg");
    // if (src1.empty() || src2.empty()) {
    //     cout << "Error loading src1 or src2" << endl;
    //     return -1;
    // }
    // beta = 1.0 - alpha;
    // addWeighted(src1, alpha, src2, beta, 0.0, dst);
    // imwrite("open.jpg",dst);

    // 2,改变图像的对比度和亮度
    // double alpha = 1.0;  // simple contrast control
    // int beta;            // simple brightness control;
    // string imageName("/root/test.jpg");
    // Mat image = imread(imageName);
    // Mat newImage = Mat::zeros(image.size(), image.type());
    // cout << "Basic Linear Transforms" << endl;
    // cout << "-----------------------" << endl;
    // cout << "*Enter the alpha value[1.0-3.0]:";
    // cin >> alpha;
    // cout << "*Enter the beta value[0-100]:";
    // cin >> beta;
    ////for (int y = 0; y < image.rows; y++) {
    ////    for (int x = 0; x < image.cols; x++) {
    ////        for (int c = 0; c < 3; c++) {
    ////            newImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(
    ////                alpha * (image.at<Vec3b>(y, x)[c] + beta));
    ////        }
    ////    }
    ////}
    ////for 循环等同于下面的函数
    // image.convertTo(newImage, -1, alpha,beta);
    // imwrite("open.jpg", newImage);

    // 3,基本绘图
    // Mat atomImage = Mat::zeros(W, W, CV_8UC3);
    // Mat rookImage = Mat::zeros(W, W, CV_8UC3);
    // MyEllipse(atomImage, 90);
    // MyEllipse(atomImage, 0);
    // MyEllipse(atomImage, 45);
    // MyEllipse(atomImage, -45);
    // MyFilledCircle(atomImage, Point(W / 2, W / 2));

    // MyPolygon(rookImage);
    // rectangle(rookImage, Point(0, 7 * W / 8), Point(W, W), Scalar(0, 255,
    // 255),
    //           FILLED, LINE_8);
    // MyLine(rookImage, Point(0, 15 * W / 16), Point(W, 15 * W / 16));
    // MyLine(rookImage, Point(W / 4, 7 * W / 8), Point(W / 4, W));
    // MyLine(rookImage, Point(W / 2, 7 * W / 8), Point(W / 2, W));
    // MyLine(rookImage, Point(3 * W / 4, 7 * W / 8), Point(3 * W / 4, W));

    // imwrite("atom.jpg", atomImage);
    // imwrite("rook.jpg", rookImage);
    // 4,添加边框到图像
    // Mat src, dst;
    // int top, bottom, left, right;
    // int borderType;
    // RNG rng(12345);
    // string imageName("/root/test.jpg");
    // src = imread(imageName, IMREAD_COLOR);
    // if (src.empty()) {
    //     cout << "No data enterd,please enter the path to an image file" <<
    // endl;
    //     return -1;
    // }
    // top = (int)(0.05 * src.rows);
    // bottom = (int)(0.05 * src.rows);
    // left = (int)(0.05 * src.cols);
    // right = (int)(0.05 * src.cols);
    // dst = src;
    // cout << "*Enter the border value to set border type [0,1]" << endl;
    // cout << "if enter 0,then the border be set a random constant;" << endl;
    // cout << "if enter 1,then the border be set a replicated;" << endl;
    // cin >> borderType;
    // if (borderType == 0)
    //     borderType = BORDER_CONSTANT;
    // else if (borderType == 1)
    //     borderType = BORDER_REPLICATE;
    // else
    //     return -1;
    // Scalar value(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0,
    // 255));
    // copyMakeBorder(src, dst, top, bottom, left, right, borderType, value);
    // imwrite("border.jpg", dst);

    // 5,sobel衍生物，用于图片边缘
    // Mat src, srcGray;
    // int scale = 1, delta = 0, ddepth = CV_16S;
    // string imageName("/root/test.jpg");
    // src = imread(imageName, IMREAD_COLOR);
    // if (src.empty()) {
    //     cout << "Could not load image" << endl;
    //     return -1;
    // }
    // GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
    // //转换为灰度图
    // cvtColor(src, srcGray, COLOR_BGR2GRAY);
    // //创建gradX和gradY矩阵
    // Mat gradX, gradY;
    // Mat absGradX, absGradY;
    // //求X方向的梯度；
    // Sobel(srcGray, gradX, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    // convertScaleAbs(gradX, absGradX);
    // //求Y方向的梯度；
    // Sobel(srcGray, gradY, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    // convertScaleAbs(gradY, absGradY);
    // //合并梯度
    // Mat grad;
    // addWeighted(absGradX, 0.5, absGradY, 0.5, 0, grad);
    // imwrite("grad.jpg", grad);

    // 6,模板匹配
    // Mat img, temp1, result;
    // int matchMethod;
    // //载入原图像和模板块
    // img = imread("/root/image/5.jpg", IMREAD_COLOR);
    // temp1 = imread("/root/image/tmp.jpg", IMREAD_COLOR);
    // if (img.empty() || temp1.empty()) {
    //     cout << "Can't read one of the images" << endl;
    //     return -1;
    // }
    // Mat imgDisplay;
    // img.copyTo(imgDisplay);

    // //创建输出结果的矩阵
    // int resultCols = img.cols - temp1.cols + 1;
    // int resultRows = img.rows - temp1.rows + 1;
    // result.create(resultRows, resultCols, CV_32FC1);
    // //进行匹配和标准化
    // cout << "please enter the matchMethod [0-5]:" << endl;
    // cin >> matchMethod;
    // matchTemplate(img, temp1, result, matchMethod);
    // normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    // //通过函数minMaxLoc定位最匹配的位置
    // double minVal, maxVal;
    // Point minLoc, maxLoc;
    // Point matchLoc;
    // minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    // //对于方法SQDIFF和SQDIFF_NORMED越小的数值代表更高的匹配结果，而对于其他的方法，数值越大匹配越好
    // if (matchMethod == TM_SQDIFF || matchMethod == TM_SQDIFF_NORMED) {
    //     matchLoc = minLoc;
    // } else {
    //     matchLoc = maxLoc;
    //     cout << "matchMethod > 1" << endl;
    // }
    // //标记结果
    // rectangle(imgDisplay, matchLoc,
    //           Point(matchLoc.x + temp1.cols, matchLoc.y + temp1.rows),
    //           Scalar::all(0), 2, 8, 0);
    // rectangle(result, matchLoc,
    //           Point(matchLoc.x + temp1.cols, matchLoc.y + temp1.rows),
    //           Scalar::all(0), 2, 8, 0);

    // imwrite("display.jpg", imgDisplay);
    // imwrite("result.jpg", result);

    // 8,视频操作
    // VideoCapture capture("/root/huaqian.mp4");
    // if (!capture.isOpened()) {
    //    cout << "Could not open video" << endl;
    //    return -1;
    //}
    // int frameWidth = (int)capture.get(CAP_PROP_FRAME_WIDTH);
    // int frameHight = (int)capture.get(CAP_PROP_FRAME_HEIGHT);
    // int frameNumber = capture.get(CAP_PROP_FRAME_COUNT);
    // cout << "frameWidth: " << frameWidth << endl;
    // cout << "frameHight: " << frameHight << endl;
    // cout << "frameNumber: " << frameNumber << endl;
    ////读取帧
    // int count = 0;
    //
    // for (;;) {
    //    Mat frame;
    //    if (!capture.read(frame)) break;
    //    int frameNum = capture.get(CAP_PROP_POS_FRAMES);
    //    double frameTime = capture.get(CAP_PROP_POS_MSEC)/1000;
    //    cout << "frameNum: " << frameNum << endl;
    //    cout << "frameTime: " << frameTime << endl;
    //    string imgName = "/root/test/frame_" + to_string(count) + ".jpg";
    //    imwrite(imgName, frame);
    //    count++;
    //}

    // capture.release();  //释放对象

    // 9,帧间差分法
    VideoCapture capture("/root/huaqian.mp4");
    // VideoCapture capture("/root/iptest.mp4");
    if (!capture.isOpened()) {
        cout << "Could not open video" << endl;
        return -1;
    }
    Mat previousImage, resultImage;
    for (;;) {
        Mat currentImage;
        if (!capture.read(currentImage)) break;  //说明已经读取到了最后一帧
        if (previousImage.empty()) {
            previousImage = currentImage;
            continue;
        }
        //计算当前帧与前帧的不同
        Mat previousImageGray, currentImageGray;
        //将图像从BGR色彩空间转换到GRAY空间,此时previousImage是灰度图
        cvtColor(previousImage, previousImageGray, COLOR_BGR2GRAY);
        cvtColor(currentImage, currentImageGray, COLOR_BGR2GRAY);
        //帧差法，相减
        absdiff(currentImageGray, previousImageGray, resultImage);
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
        cout << "counter: " << counter << " num: " << num << " p: " << p
             << endl;
        // counter ,num, p 分别为有变化的像素点数，总像素点数目，比例
        if (p > 0.65)  //达到阈值的像素数达到一定的数量则保存该图像
        {
            cout << "this frame is keyframe" << endl;
            //获取当前帧的时间
            double frameTime = capture.get(CAP_PROP_POS_MSEC) / 1000;

            string imgName = "/root/test/" + to_string(frameTime) + ".jpg";
            imwrite(imgName, currentImage);
            ofstream myfile;
            myfile.open("/root/test.txt", ios::app);
            myfile << "counter: " << counter << " num: " << num << " p: " << p
                   << endl;
            myfile.close();
        } else {
            cout << "this frame is not keyframe " << endl;
        }
        previousImage = currentImage;
    }
    capture.release();

    return 0;
}

void MyEllipse(Mat img, double angle) {
    int thickness = 2;
    int lineType = 8;
    ellipse(img, Point(W / 2, W / 2), Size(W / 4, W / 16), angle, 0, 360,
            Scalar(255, 0, 0), thickness, lineType);
}

void MyFilledCircle(Mat img, Point center) {
    circle(img, center, W / 32, Scalar(0, 0, 255), FILLED, LINE_8);
}

void MyPolygon(Mat img) {
    int lineType = LINE_8;
    Point rookPoints[1][20];
    rookPoints[0][0] = Point(W / 4, 7 * W / 8);
    rookPoints[0][1] = Point(3 * W / 4, 7 * W / 8);
    rookPoints[0][2] = Point(3 * W / 4, 13 * W / 16);
    rookPoints[0][3] = Point(11 * W / 16, 13 * W / 16);
    rookPoints[0][4] = Point(19 * W / 32, 13 * W / 16);
    rookPoints[0][5] = Point(3 * W / 4, 3 * W / 8);
    rookPoints[0][6] = Point(3 * W / 4, W / 8);
    rookPoints[0][7] = Point(26 * W / 40, W / 8);
    rookPoints[0][8] = Point(26 * W / 40, W / 4);
    rookPoints[0][9] = Point(22 * W / 40, W / 4);
    rookPoints[0][10] = Point(22 * W / 40, W / 8);
    rookPoints[0][11] = Point(18 * W / 40, W / 8);
    rookPoints[0][12] = Point(18 * W / 40, W / 4);
    rookPoints[0][13] = Point(14 * W / 40, W / 4);
    rookPoints[0][14] = Point(14 * W / 40, W / 8);
    rookPoints[0][15] = Point(W / 4, W / 8);
    rookPoints[0][16] = Point(W / 4, 3 * W / 8);
    rookPoints[0][17] = Point(13 * W / 32, 3 * W / 8);
    rookPoints[0][18] = Point(5 * W / 16, 13 * W / 16);
    rookPoints[0][19] = Point(W / 4, 13 * W / 16);

    const Point* ppt[1] = {rookPoints[0]};
    int npt[] = {20};
    fillPoly(img, ppt, npt, 1, Scalar(255, 255, 255), lineType);
}

void MyLine(Mat img, Point start, Point end) {

    int thickness = 2;
    int lineType = LINE_8;
    line(img, start, end, Scalar(0, 0, 0), thickness, lineType);
}
