#include "FaceCapture.h"

// Save captured face image to student folder
bool FaceCapture::saveFace(
    const cv::Mat& face,
    const std::string& folder,
    int imageNumber)
{
    // Generate image filename
    std::string fileName =
        folder + "/" +
        std::to_string(imageNumber) +
        ".jpg";

    // Save image to disk
    return cv::imwrite(
        fileName,
        face
    );
}