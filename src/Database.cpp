#include "Database.h"

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>

Database::Database() //constructor
{
    db = nullptr;
}

Database::~Database()
{
    close();
}

// Connect to SQLite database
bool Database::connect(const std::string& dbName)
{
    if (sqlite3_open(dbName.c_str(), &db) != SQLITE_OK)
    {
        std::cout << "Database connection failed!\n";
        return false;
    }

    std::cout << "Database connected successfully!\n";
    std::cout << "Connected to DB: "
        << dbName
        << std::endl;

    return true;
}

bool Database::createTables()
{
    const char* studentsTable =
        "CREATE TABLE IF NOT EXISTS students("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "course TEXT,"
        "face_label INTEGER UNIQUE"
        ");";

    const char* attendanceTable =
        "CREATE TABLE IF NOT EXISTS attendance("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "student_id INTEGER,"
        "date TEXT,"
        "time TEXT,"
        "FOREIGN KEY(student_id) REFERENCES students(id)"
        ");";

    char* errMsg = nullptr;

    if (sqlite3_exec(
        db,
        studentsTable,
        nullptr,
        nullptr,
        &errMsg) != SQLITE_OK)
    {
        std::cout << "Students Table Error: "
            << errMsg << std::endl;

        sqlite3_free(errMsg);
        return false;
    }

    if (sqlite3_exec(
        db,
        attendanceTable,
        nullptr,
        nullptr,
        &errMsg) != SQLITE_OK)
    {
        std::cout << "Attendance Table Error: "
            << errMsg << std::endl;

        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "Tables created successfully!\n";

    return true;
}

bool Database::addStudent(
    const std::string& name,
    const std::string& course,
    int faceLabel)
{
    std::string sql =
        "INSERT INTO students "
        "(name, course, face_label) VALUES ('" +
        name + "','" +
        course + "'," +
        std::to_string(faceLabel) +
        ");";

    char* errMsg = nullptr;

    if (sqlite3_exec(
        db,
        sql.c_str(),
        nullptr,
        nullptr,
        &errMsg) != SQLITE_OK)
    {
        std::cout << "Insert Error: "
            << errMsg << std::endl;

        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "Student Added Successfully!\n";

    return true;
}

// Get student name using face label
std::string Database::getStudentName(int faceLabel)
{
    std::string sql =
        "SELECT name FROM students WHERE face_label = " +
        std::to_string(faceLabel) + ";";

    sqlite3_stmt* stmt;
    std::string name = "Unknown";

    if (sqlite3_prepare_v2(
        db,
        sql.c_str(),
        -1,
        &stmt,
        nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            name = reinterpret_cast<const char*>(
                sqlite3_column_text(stmt, 0));
        }
    }

    sqlite3_finalize(stmt);

    return name;
}

bool Database::isAttendanceMarkedToday(int faceLabel)
{
    std::cout << "Checking attendance for label: "
        << faceLabel
        << std::endl;

    std::string sql =
        "SELECT attendance.id "
        "FROM attendance "
        "JOIN students ON attendance.student_id = students.id "
        "WHERE students.face_label = ? "
        "AND attendance.date = date('now','localtime');";

    sqlite3_stmt* stmt;
    bool found = false;

    if (sqlite3_prepare_v2(
        db,
        sql.c_str(),
        -1,
        &stmt,
        nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, faceLabel);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            found = true;
        }
    }

    sqlite3_finalize(stmt);

    std::cout << "Attendance Found = "
        << found
        << std::endl;

    return found;
}

// Insert attendance record
bool Database::markAttendance(int faceLabel)
{
    std::string getStudentSql =
        "SELECT id FROM students WHERE face_label = ?;";

    sqlite3_stmt* stmt;
    int studentId = -1;

    if (sqlite3_prepare_v2(
        db,
        getStudentSql.c_str(),
        -1,
        &stmt,
        nullptr) == SQLITE_OK)
    {
        sqlite3_bind_int(stmt, 1, faceLabel);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            studentId =
                sqlite3_column_int(stmt, 0);
        }
    }

    sqlite3_finalize(stmt);
    std::cout << "studentId = "
        << studentId
        << std::endl;
    if (studentId == -1)
    {
        std::cout << "Student ID not found!\n";
        return false;
    }

    time_t now = time(nullptr);
    tm localTime;

    localtime_s(&localTime, &now);

    char dateBuffer[20];
    char timeBuffer[20];

    strftime(
        dateBuffer,
        sizeof(dateBuffer),
        "%Y-%m-%d",
        &localTime);

    strftime(
        timeBuffer,
        sizeof(timeBuffer),
        "%H:%M:%S",
        &localTime);

    std::string insertSql =
        "INSERT INTO attendance "
        "(student_id, date, time) "
        "VALUES (?, ?, ?);";

    if (sqlite3_prepare_v2(
        db,
        insertSql.c_str(),
        -1,
        &stmt,
        nullptr) != SQLITE_OK)
    {
        std::cout << "Insert prepare failed!\n";
        std::cout << sqlite3_errmsg(db)
            << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, studentId);
    sqlite3_bind_text(
        stmt, 2,
        dateBuffer,
        -1,
        SQLITE_STATIC);

    sqlite3_bind_text(
        stmt, 3,
        timeBuffer,
        -1,
        SQLITE_STATIC);

    int result = sqlite3_step(stmt);

    if (result != SQLITE_DONE)
    {
        std::cout << "SQLite Error: "
            << sqlite3_errmsg(db)
            << std::endl;
    }

    bool success =
        (result == SQLITE_DONE);

    sqlite3_finalize(stmt);

    std::cout << "markAttendance() success = "
        << success
        << std::endl;

    return success;
}
int Database::getNextLabel()
{
    std::string sql =
        "SELECT MAX(face_label) FROM students;";

    sqlite3_stmt* stmt;

    int label = 1;

    if (sqlite3_prepare_v2(
        db,
        sql.c_str(),
        -1,
        &stmt,
        nullptr) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            if (sqlite3_column_type(stmt, 0)
                != SQLITE_NULL)
            {
                label =
                    sqlite3_column_int(
                        stmt,
                        0) + 1;
            }
        }
    }

    sqlite3_finalize(stmt);

    return label;
}


// Display all attendance records
void Database::showAttendance()
{
    std::string sql =
        "SELECT students.name, "
        "attendance.date, "
        "attendance.time "
        "FROM attendance "
        "JOIN students "
        "ON attendance.student_id = students.id "
        "ORDER BY attendance.date DESC, attendance.time DESC;";

        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(
            db,
            sql.c_str(),
            -1,
            &stmt,
            nullptr) == SQLITE_OK)
        {
            std::cout
                << "\n===== ATTENDANCE =====\n";

            while (sqlite3_step(stmt)
                == SQLITE_ROW)
            {
                std::cout
                    << reinterpret_cast<
                    const char*>(
                        sqlite3_column_text(
                            stmt,
                            0))
                    << " | ";

                std::cout
                    << reinterpret_cast<
                    const char*>(
                        sqlite3_column_text(
                            stmt,
                            1))
                    << " | ";

                std::cout
                    << reinterpret_cast<
                    const char*>(
                        sqlite3_column_text(
                            stmt,
                            2))
                    << std::endl;
            }
        }

        sqlite3_finalize(stmt);
}


// Export attendance records to CSV file
bool Database::exportAttendanceCSV()
{
    std::ofstream file(
        "attendance.csv"
    );

    if (!file.is_open())
    {
        return false;
    }

    file << "Name,Date,Time\n";

    std::string sql =
        "SELECT students.name, "
        "attendance.date, "
        "attendance.time "
        "FROM attendance "
        "JOIN students "
        "ON attendance.student_id = students.id "
        "ORDER BY attendance.date DESC, attendance.time DESC;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(
        db,
        sql.c_str(),
        -1,
        &stmt,
        nullptr) == SQLITE_OK)
    {
        while (sqlite3_step(stmt)
            == SQLITE_ROW)
        {
            file
                << reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 0))
                << ",";

            file
                << reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 1))
                << ",";

            file
                << reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, 2))
                << "\n";
        }
    }

    sqlite3_finalize(stmt);

    file.close();

    return true;
}

void Database::close()
{
    if (db != nullptr)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}