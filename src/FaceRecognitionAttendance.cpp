#include <opencv2/opencv.hpp>

int main()
{
    cv::Mat img(400, 600, CV_8UC3);

    cv::imshow("OpenCV Test", img);
    cv::waitKey(0);

    return 0;
}