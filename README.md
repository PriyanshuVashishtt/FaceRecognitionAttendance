# Face Recognition Attendance System

A Face Recognition Based Attendance Management System developed using **C++**, **OpenCV**, and **SQLite**.

The system automates attendance marking by detecting and recognizing student faces through a webcam and storing attendance records in a database.

---

## Features

* Student Registration
* Automatic Face Image Capture
* Automatic Face Model Training
* Face Detection using Haar Cascade Classifier
* Face Recognition using LBPH Algorithm
* Attendance Marking
* Duplicate Attendance Prevention
* Attendance Record Viewing
* Attendance Export to CSV
* SQLite Database Integration

---

## Technologies Used

* C++
* OpenCV
* SQLite3
* Visual Studio 2022
* STL (Standard Template Library)

---

## Project Structure

```text
FaceRecognitionAttendance
│
├── include/
│   ├── Camera.h
│   ├── Database.h
│   ├── FaceCapture.h
│   ├── FaceDetector.h
│   ├── FaceRecognizer.h
│   ├── FaceTrainer.h
│   └── Registration.h
│
├── src/
│   ├── Camera.cpp
│   ├── Database.cpp
│   ├── FaceCapture.cpp
│   ├── FaceDetector.cpp
│   ├── FaceRecognizer.cpp
│   ├── FaceTrainer.cpp
│   ├── Registration.cpp
│   └── main.cpp
│
├── models/
│   └── haarcascade_frontalface_default.xml
│
├── sqlite/
│   ├── sqlite3.c
│   └── sqlite3.h
│
├── FaceRecognitionAttendance.slnx
├── FaceRecognitionAttendance.vcxproj
└── README.md
```

---

## Working

### Student Registration

1. Enter student details.
2. Create a unique student folder.
3. Capture face images using webcam.
4. Store images locally.
5. Train the face recognition model automatically.

### Attendance Process

1. Open webcam.
2. Detect faces in real time.
3. Recognize registered students.
4. Check if attendance is already marked.
5. Store attendance records in SQLite database.

---

## Database Design

### Students Table

| Column     | Description            |
| ---------- | ---------------------- |
| id         | Student ID             |
| name       | Student Name           |
| course     | Course Name            |
| face_label | Face Recognition Label |

### Attendance Table

| Column     | Description     |
| ---------- | --------------- |
| id         | Attendance ID   |
| student_id | Student ID      |
| date       | Attendance Date |
| time       | Attendance Time |

---

## How to Run

### Requirements

* Visual Studio 2022
* OpenCV
* SQLite3

### Steps

1. Clone the repository.

```bash
git clone https://github.com/PriyanshuVashishtt/FaceRecognitionAttendance.git
```

2. Open the solution in Visual Studio.
3. Configure OpenCV include and library directories.
4. Build the project.
5. Run the application.

---

## Future Improvements

* GUI Interface using Qt
* Deep Learning Based Face Recognition
* Cloud Database Integration
* Multi-Camera Support
* Attendance Analytics Dashboard

---

## Author

**Priyanshu Vashishtt**

B.Tech Student

GitHub: https://github.com/PriyanshuVashishtt

---

## Support

If you found this project useful, consider giving it a star on GitHub.
