#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <fstream>
using namespace cv;
using namespace std;

//利用傅里叶变换，对图片进行隐藏水印
vector<Mat> allPlanes;  //用于存放原图片的多通道

Mat splitSrc(Mat mat);
Mat antitransformImage(Mat complexImage, vector<Mat> allPlanes);
//剪切和象限变换
void shiftDFT(Mat image) {
    image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols / 2;
    int cy = image.rows / 2;

    Mat q0 = Mat(image, Rect(0, 0, cx, cy));
    Mat q1 = Mat(image, Rect(cx, 0, cx, cy));
    Mat q2 = Mat(image, Rect(0, cy, cx, cy));
    Mat q3 = Mat(image, Rect(cx, cy, cx, cy));

    Mat tmp = Mat();
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

//为加快傅里叶变换的速度，对要处理的图片尺寸进行优化
Mat optimizeImageDim(Mat image) {
    // init
    Mat padded;

    // get the optimal rows size for dft
    int addPixelRows = getOptimalDFTSize(image.rows);
    // get the optimal cols for dft
    int addPixelCols = getOptimalDFTSize(image.cols);
    // apply the optimal cols and rows size to the image
    copyMakeBorder(image, padded, 0, addPixelRows - image.rows, 0,
                   addPixelCols - image.cols, BORDER_CONSTANT, Scalar::all(0));
    return padded;
}

//获取DFT图像
Mat createOptimizedMagnitude(Mat complexImage) {
    // init
    vector<Mat> newPlanes;
    Mat mag = Mat();

    // split the complex image in two planes
    split(complexImage, newPlanes);

    // compute the magnitude
    magnitude(newPlanes[0], newPlanes[1], mag);

    // move to a logaritnmic scale
    add(Mat::ones(mag.size(), CV_32F), mag, mag);
    log(mag, mag);

    // optionally reorder the 4 quadrants of the magnitude image
    shiftDFT(mag);

    // normalize the magnitude image for the visualization since both JavaFx
    // and Opencv need images with value between 0 and 255
    // convertt back to CV_8UC1
    mag.convertTo(mag, CV_8UC1);
    normalize(mag, mag, 0, 255, NORM_MINMAX, CV_8UC1);
    return mag;
}

Mat getTransformImageWithText(Mat image) {
    // planes
    vector<Mat> planes;
    Mat complexImage = Mat();

    // optimize the dimensionn of the loaded image
    Mat padded = splitSrc(image);
    padded.convertTo(padded, CV_32F);

    // prepare the image planes to obtain the complex image
    planes.push_back(padded);
    planes.push_back(Mat::zeros(padded.size(), CV_32F));

    // prepare a complex image for performing the dft
    merge(planes, complexImage);
    dft(complexImage, complexImage);

    // optimize the image resulting from the dfft operation
    Mat magnitude = createOptimizedMagnitude(complexImage);
    planes.clear();
    return magnitude;
}

//打水印，先将原图进行离散傅里叶变换到频域，加上水印后再通过离散傅里叶逆变换到空间域恢复图片
Mat transformImageWithText(Mat image, string waterMarkText, Point point,
                           double fontSize, Scalar scalar) {
    Mat complexImage = Mat();
    vector<Mat> planes;

    // optimize the dimension of the loaded image
    // Mat padded = optimizeImageDim(image);
    Mat padded = splitSrc(image);
    padded.convertTo(padded, CV_32F);

    planes.push_back(padded);
    planes.push_back(Mat::zeros(padded.size(), CV_32F));

    // preparae a complex image for performing the dft
    merge(planes, complexImage);

    // dft
    dft(complexImage, complexImage);
    cout << complexImage.type() << endl;
    putText(complexImage, waterMarkText, point, FONT_HERSHEY_DUPLEX, fontSize,
            scalar, 2);
    flip(complexImage, complexImage, -1);
    putText(complexImage, waterMarkText, point, FONT_HERSHEY_DUPLEX, fontSize,
            scalar, 2);
    flip(complexImage, complexImage, -1);
    //计算幅度
    createOptimizedMagnitude(complexImage);
    return antitransformImage(complexImage, allPlanes);
}

//逆傅里叶计算，还原图片
Mat antitransformImage(Mat complexImage, vector<Mat> allPlanes) {
    Mat invDFT = Mat();
    idft(complexImage, invDFT, DFT_SCALE | DFT_REAL_OUTPUT, 0);

    Mat restoredImage = Mat();
    invDFT.convertTo(restoredImage, CV_8U);
    if (allPlanes.size() == 0) {
        allPlanes.push_back(restoredImage);
    } else {
        allPlanes[0] = restoredImage;
    }
    Mat lastImage = Mat();
    merge(allPlanes, lastImage);
    return lastImage;
}

Mat splitSrc(Mat mat) {
    // 1,优化图片的尺寸
    mat = optimizeImageDim(mat);
    split(mat, allPlanes);
    Mat padded = Mat();
    if (allPlanes.size() > 1) {
        for (int i = 0; i < allPlanes.size(); i++) {
            if (i == 0) {
                padded = allPlanes[i];
                break;
            }
        }
    } else {
        padded = mat;
    }
    return padded;
}
int main(int argc, char* argv[]) {

    if (argc < 3) {
        cout << "./opencvtest inputFile enc/dec " << endl;
        return -1;
    }
    string control = argv[2];
    if (control == "enc") {
        // load image
        cout << "进入编码" << endl;
        Point point(50, 100);
        Scalar scalar(0, 0, 0, 0);
        // Mat img1 = imread(argv[1], IMREAD_GRAYSCALE);
        Mat img1 = imread(argv[1], IMREAD_COLOR);
        Mat outImage =
            transformImageWithText(img1, "helios", point, 2.0, scalar);

        imwrite("/opt/testopencv/image/dst/outimg.bmp", outImage);
        //Mat waterImage = getTransformImageWithText(outImage);
        //imwrite("/opt/testopencv/image/dst/watermark.jpg", waterImage);
    } else if (control == "dec") {
        cout << "解码" << endl;
        //注意当设置图片的格式是png的时候，可通过这种方法来获取水印，但是设置为jpg则不行
        // Mat img1 = imread(argv[1], IMREAD_GRAYSCALE);
        Mat img1 = imread(argv[1], IMREAD_COLOR);
        Mat waterImage = getTransformImageWithText(img1);
        imwrite("/opt/testopencv/image/dst/watermark1.png", waterImage);
    }
    return 0;
}
