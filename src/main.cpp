#include <iostream>
#include <opencv2/opencv.hpp>

#include "Camera.h"
#include "FaceDetector.h"
#include "FaceRecognizer.h"
#include "Database.h"
#include "Registration.h"

int main()
{
    // Create project objects

    Camera camera;
    FaceDetector detector;
    FaceRecognizer recognizer;
    Database database;

    // Connect to SQLite database
    if (!database.connect(
        "C:\\Users\\Lenovo\\source\\repos\\FaceRecognitionAttendance\\database\\attendance.db"))
    {
        std::cout << "Database Error\n";
        return -1;
    }

    // Create database tables if they don't exist
    if (!database.createTables())
    {
        std::cout << "Table Creation Error\n";
        return -1;
    }

    // Main menu
    Registration registration;

    int choice;

    std::cout
        << "===== FACE ATTENDANCE SYSTEM =====\n\n";

    std::cout
        << "1. Register Student\n";

    std::cout
        << "2. Take Attendance\n";

    std::cout
        << "3. View Attendance\n";

    std::cout
        << "4. Export Attendance CSV\n";

    std::cout
        << "5. Exit\n";

    std::cout
        << "Choice: ";

    std::cin >> choice;

    if (choice == 1)
    {
        registration.registerStudent(database);
        return 0;
    }

    if (choice == 3)
    {
        database.showAttendance();

        system("pause");

        return 0;
    }

    if (choice == 4)
    {
        if (database.exportAttendanceCSV())
        {
            std::cout
                << "attendance.csv created successfully!\n";
        }
        else
        {
            std::cout
                << "CSV export failed!\n";
        }

        system("pause");

        return 0;
    }

    if (choice == 5)
    {
        return 0;
    }

    if (choice != 2)
    {
        return 0;
    }


    // Open webcam for attendance
    bool cameraOpened = camera.openCamera();

    std::cout
        << "cameraOpened = "
        << cameraOpened
        << std::endl;

    if (!cameraOpened)
    {
        std::cout << "Camera Error\n";
        return -1;
    }

    std::cout
        << "Camera opened successfully!"
        << std::endl;

    // Load Haar Cascade face detector
    if (!detector.loadModel(
        "models/haarcascade_frontalface_default.xml"))
    {
        std::cout << "Cascade Error\n";
        return -1;
    }

    // Load trained face recognition model
    if (!recognizer.loadModel(
        "models/face_model.yml"))
    {
        std::cout << "Model Error\n";
        return -1;
    }

    cv::Mat frame;
    std::string attendanceMessage = "";
    int messageFrames = 0;


    // Main attendance loop
    while (true)
    {
        if (!camera.getFrame(frame))
        {
            std::cout
                << "Failed to capture frame!"
                << std::endl;

            continue;
        }

        if (frame.empty())
        {
            std::cout
                << "Empty frame!"
                << std::endl;

            continue;
        }

        // Detect faces in current frame

        auto faces = detector.detectFaces(frame);

        for (auto& face : faces)
        {
            cv::rectangle(
                frame,
                face,
                cv::Scalar(0, 255, 0),
                2
            );

            cv::Mat faceROI = frame(face);


            // Recognize detected face
            int label =
                recognizer.recognizeFace(faceROI);

            std::string text;

            if (label == -1)
            {
                text = "Unknown";
            }
            else
            {
                text = database.getStudentName(label);

                //std::cout << "Recognized: "
                //    << text
                //    << " Label: "
                //    << label
                //    << std::endl;

                bool alreadyMarked =
                    database.isAttendanceMarkedToday(label);

                //std::cout
                //    << "alreadyMarked = "
                //    << alreadyMarked
                //    << std::endl;

                if (!alreadyMarked)
                {
                    std::cout
                        << "Attendance not marked today. Attempting insert..."
                        << std::endl;

                    bool success =
                        database.markAttendance(label);

                    std::cout
                        << "markAttendance() returned: "
                        << success
                        << std::endl;

                    if (success)
                    {
                        attendanceMessage =
                            "Attendance Marked For " + text;

                        messageFrames = 150;
                    }
                    else
                    {
                        attendanceMessage =
                            "Attendance Insert Failed";

                        messageFrames = 150;
                    }
                }
                else
                {
                    //std::cout
                    //    << "Already marked today."
                    //    << std::endl;

                    attendanceMessage =
                        "Already Marked Today";

                    messageFrames = 60;
                }
            }

            cv::putText(
                frame,
                text,
                cv::Point(face.x,
                    face.y - 10),
                cv::FONT_HERSHEY_SIMPLEX,
                0.7,
                cv::Scalar(0, 255, 0),
                2
            );
        }
        if (messageFrames > 0)
        {
            cv::putText(
                frame,
                attendanceMessage,
                cv::Point(20, 50),
                cv::FONT_HERSHEY_SIMPLEX,
                0.8,
                cv::Scalar(0, 255, 255),
                2
            );

            messageFrames--;
        }
        cv::imshow(
            "Face Recognition",
            frame
        );

        if (cv::waitKey(1) == 27)
            break;
    }

    camera.releaseCamera();
    database.close();

    return 0;
}