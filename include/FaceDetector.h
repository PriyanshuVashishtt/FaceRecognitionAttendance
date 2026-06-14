#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

// Detects faces using Haar Cascade
class FaceDetector
{
private:
    cv::CascadeClassifier faceCascade;

public:
    // Load Haar Cascade model
    bool loadModel(const std::string& modelPath);

    // Detect faces in image
    std::vector<cv::Rect> detectFaces(
        const cv::Mat& frame
    );
};