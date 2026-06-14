#include "FaceRecognizer.h"
#include <iostream>

// Create LBPH face recognizer
FaceRecognizer::FaceRecognizer()
{
    recognizer = cv::face::LBPHFaceRecognizer::create();
}

// Load trained face recognition model
bool FaceRecognizer::loadModel(const std::string& modelPath)
{
    try
    {
        recognizer->read(modelPath);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

// Recognize face and return label
int FaceRecognizer::recognizeFace(const cv::Mat& faceImage)
{
    if (faceImage.empty())
        return -1;

    cv::Mat gray;

    if (faceImage.channels() == 3)
    {


        cv::cvtColor(
            faceImage,
            gray,
            cv::COLOR_BGR2GRAY
        );
    }
    else
    {
        gray = faceImage.clone();
    }

    // Convert image to grayscale
    cv::resize(
        gray,
        gray,
        cv::Size(200, 200)
    );

    int label = -1;
    double confidence = 0;

    // Predict label and confidence
    recognizer->predict(
        gray,
        label,
        confidence
    );

    std::cout
        << "Label: "
        << label
        << " Confidence: "
        << confidence
        << std::endl;


    // Reject unknown faces
    if (confidence > 80)
    {
        return -1;
    }

    return label;

}