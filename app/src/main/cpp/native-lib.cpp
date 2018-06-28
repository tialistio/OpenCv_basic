#include <jni.h>
#include <string>
#include <opencv2/opencv.hpp>


extern "C"

JNIEXPORT void JNICALL Java_com_example_a1_edge_1detection_OpenCVCamera_detectEdges(
        JNIEnv*, jobject /* this */, jlong gray, jlong rgb) {

        const int MEDIAN_BLUR_FILTER_SIZE = 7 ;
        const int LAPLACIAN_FILTER_SIZE = 5;
        const int EDGES_THRESHOLD = 30;
        int repetitions = 5;
        int kSize = 9;
        double sigmaColor = 9;
        double sigmaSpace = 7;

        cv::Mat& edges = *(cv::Mat *) gray;
        cv::medianBlur(edges, edges, MEDIAN_BLUR_FILTER_SIZE);
        cv::Laplacian(edges, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
        cv::Mat mask; cv::threshold(edges, mask, EDGES_THRESHOLD, 255, CV_THRESH_BINARY_INV);

        cv::Mat& src = *(cv::Mat *) rgb;
        cv::cvtColor(src, src, CV_RGBA2RGB);
        cv::Size size = src.size();
        cv::Size smallSize;
        smallSize.width = size.width/4;
        smallSize.height = size.height/4;
        cv::Mat smallImg = cv::Mat(smallSize, CV_8UC3);
        resize(src, smallImg, smallSize, 0,0, CV_INTER_LINEAR);

        cv::Mat tmp = cv::Mat(smallSize, CV_8UC3);

        for(int i=0; i<repetitions;i++){
            bilateralFilter(smallImg, tmp, kSize, sigmaColor, sigmaSpace);
            bilateralFilter(tmp, smallImg, kSize, sigmaColor, sigmaSpace);
        }

        cv::Mat bigImg;
        resize(smallImg, bigImg, size, 0, 0, CV_INTER_LINEAR);
        cv::Mat dst; bigImg.copyTo(dst,mask);
        cv::medianBlur(dst, src, MEDIAN_BLUR_FILTER_SIZE-4);
}
