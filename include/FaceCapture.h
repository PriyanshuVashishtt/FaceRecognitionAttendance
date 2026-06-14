#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class FaceCapture
{
public:
    bool saveFace(
        const cv::Mat& face,
        const std::string& folder,
        int imageNumber
    );
};