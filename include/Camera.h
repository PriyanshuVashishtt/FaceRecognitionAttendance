#pragma once

#include <opencv2/opencv.hpp>

// Handles webcam operations
class Camera
{
private:
    cv::VideoCapture cap;

public:
    bool openCamera(int index = 0);
    bool getFrame(cv::Mat& frame);
    void releaseCamera();
};