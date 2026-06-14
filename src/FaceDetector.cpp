#include "FaceDetector.h"

// Load Haar Cascade model
bool FaceDetector::loadModel(const std::string& modelPath)
{
    return faceCascade.load(modelPath);
}

// Detect faces in image frame
std::vector<cv::Rect> FaceDetector::detectFaces(const cv::Mat& frame)
{
    std::vector<cv::Rect> faces;

    if (frame.empty())
    {
        return faces;
    }

    cv::Mat gray;

    // Detect faces in image frame
    cv::cvtColor(
        frame,
        gray,
        cv::COLOR_BGR2GRAY
    );

    cv::equalizeHist(
        gray,
        gray
    );

    if (faceCascade.empty())
    {
        return faces;
    }

    // Run Haar Cascade face detection
    faceCascade.detectMultiScale(
        gray,
        faces,
        1.1,
        5,
        0,
        cv::Size(100, 100)
    );

    return faces;
}