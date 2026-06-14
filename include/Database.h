#pragma once

#include <string>
#include "sqlite3.h"

// Handles all database operations
class Database
{
private:
    sqlite3* db;

public:
    // Constructor / Destructor
    Database();
    ~Database();

    // Database connection
    bool connect(const std::string& dbName);
    bool createTables();

    // Student management
    bool addStudent(
        const std::string& name,
        const std::string& course,
        int faceLabel
    );

    std::string getStudentName(int faceLabel);

    // Attendance management
    bool isAttendanceMarkedToday(int faceLabel);
    bool markAttendance(int faceLabel);

    // Registration utilities
    int getNextLabel();

    // Attendance reports
    void showAttendance();
    bool exportAttendanceCSV();

    void close();
};