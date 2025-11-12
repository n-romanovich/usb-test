#include "driveinfo.hpp"
#include "messages.hpp"
#include "lang.hpp"

#pragma once

bool DriveTestWritePart(int selectedDrive, std::vector<char>& drives, int selectedTest);
bool DriveTestReadPart(int selectedDrive, std::vector<char>& drives, int slowCountWrite, int verySlowCountWrite, int critCountWrite, int badCountWrite, double writeSpeed, unsigned long long availableMegabytes);
