#include "FaceTrainer.h"

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>

#include <filesystem>
#include <vector>
#include <iostream>

namespace fs = std::filesystem;

// Train LBPH face recognition model
bool FaceTrainer::trainModel()
{
    std::vector<cv::Mat> images;
    std::vector<int> labels;

    std::string datasetPath =
        "C:\\Users\\Lenovo\\source\\repos\\FaceRecognitionAttendance\\images";

    if (!fs::exists(datasetPath))
    {
        std::cout << "Images folder not found!\n";
        return false;
    }

    // Load all student image folders
    for (const auto& personFolder :
        fs::directory_iterator(datasetPath))
    {
        if (!personFolder.is_directory())
            continue;

        int label =
            std::stoi(
                personFolder.path().filename().string()
            );

        for (const auto& imageFile :
            fs::directory_iterator(personFolder.path()))
        {
            cv::Mat img =
                // Read training image
                cv::imread(
                    imageFile.path().string(),
                    cv::IMREAD_GRAYSCALE
                );

            if (img.empty())
                continue;

            cv::resize(
                img,
                img,
                cv::Size(200, 200)
            );
            // Store image and label
            images.push_back(img);
            labels.push_back(label);
        }
    }

    if (images.empty())
    {
        std::cout << "No training images found!\n";
        return false;
    }
    // Create LBPH recognizer
    auto recognizer =
        cv::face::LBPHFaceRecognizer::create();
    // Train model using all images
    recognizer->train(images, labels);

    recognizer->save(
        "models/face_model.yml"
    );

    std::cout
        << "Training Completed!"
        << std::endl;

    return true;
}