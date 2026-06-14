#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <string>

// Handles face recognition using LBPH
class FaceRecognizer
{
private:
    cv::Ptr<cv::face::LBPHFaceRecognizer> recognizer;

public:
    FaceRecognizer();

    // Load trained model
    bool loadModel(const std::string& modelPath);

    // Recognize face and return label
    int recognizeFace(const cv::Mat& faceImage);
};