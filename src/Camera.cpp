#include "Camera.h"
#include <iostream>


// Open webcam
bool Camera::openCamera(int index)
{
    std::cout
        << "Opening Camera Index "
        << index
        << std::endl;

    cap.open(index);

    std::cout
        << "isOpened = "
        << cap.isOpened()
        << std::endl;

    return cap.isOpened();
}

// Capture frame from webcam
bool Camera::getFrame(cv::Mat& frame)
{
    return cap.read(frame);
}


// Release webcam resources
void Camera::releaseCamera()
{
    if (cap.isOpened())
        cap.release();
}