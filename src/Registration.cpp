#include "Registration.h"

#include <iostream>
#include <filesystem>

#include "Camera.h"
#include "FaceDetector.h"
#include <opencv2/opencv.hpp>
#include "FaceTrainer.h"


// Register a new student
bool Registration::registerStudent(Database& database)
{
    std::string name;
    std::string course;

    std::cin.ignore();

    std::cout << "Enter Student Name: ";
    std::getline(std::cin, name);

    std::cout << "Enter Course: ";
    std::getline(std::cin, course);

    int label =
        database.getNextLabel();

    std::string folderPath =
        "images/" +
        std::to_string(label);

    std::filesystem::create_directories(
        folderPath
    );

    std::cout
        << "Created Folder: "
        << folderPath
        << std::endl;

    if (database.addStudent(
        name,
        course,
        label))
    {
        std::cout
            << "\nStudent Registered!\n";

        std::cout
            << "Assigned Label: "
            << label
            << std::endl;

        // Open webcam for image collection
        Camera camera;
        FaceDetector detector;

        if (!camera.openCamera())
        {
            std::cout
                << "Camera Error\n";

            return false;
        }

        cv::waitKey(1000);
        cv::Mat testFrame;

        camera.getFrame(testFrame);

        std::cout
            << "Rows = "
            << testFrame.rows
            << " Cols = "
            << testFrame.cols
            << std::endl;

        if (!detector.loadModel(
            "models/haarcascade_frontalface_default.xml"))
        {
            std::cout
                << "Cascade Error\n";

            return false;
        }

        cv::Mat frame;

        int imageCount = 0;

        std::cout
            << "\nLook at the camera...\n";

        while (imageCount < 30)
        {
            if (!camera.getFrame(frame))
            {
                std::cout
                    << "Can't grab frame"
                    << std::endl;

                continue;
            }

            if (frame.empty())
            {
                std::cout
                    << "Empty frame"
                    << std::endl;

                continue;
            }

            auto faces =
                detector.detectFaces(frame);

            for (auto& face : faces)
            {
                cv::rectangle(
                    frame,
                    face,
                    cv::Scalar(0, 255, 0),
                    2
                );

                cv::Mat faceROI =
                    frame(face).clone();

                imageCount++;

                std::string imagePath =
                    folderPath +
                    "/" +
                    std::to_string(imageCount) +
                    ".jpg";

                cv::imwrite(
                    imagePath,
                    faceROI
                );

                std::cout
                    << "Captured "
                    << imageCount
                    << "/30"
                    << std::endl;

                if (imageCount >= 30)
                {
                    break;
                }
            }

            cv::imshow(
                "Registration",
                frame
            );

            if (cv::waitKey(30) == 27)
            {
                break;
            }
        }
        cv::destroyAllWindows();

        camera.releaseCamera();

        std::cout
            << "\nImage Capture Complete!\n";

        std::cout
            << "\nTraining Model...\n";

        // Train recognition model with all students
        if (FaceTrainer::trainModel())
        {
            std::cout
                << "Training Complete!\n";
        }
        else
        {
            std::cout
                << "Training Failed!\n";
        }

        return true;
    }

    return false;
}