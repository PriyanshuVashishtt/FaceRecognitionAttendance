#pragma once

#include "Database.h"

// Handles new student registration
class Registration
{
public:
    bool registerStudent(
        Database& database
    );
};