#pragma once

#include "driveinfo.hpp"
#include "messages.hpp"
#include "lang.hpp"
#include "json_utils.hpp"

bool driveTestWritePart(int selectedDrive, std::vector<char>& drives, int selectedTest);
bool driveTestReadPart(int selectedDrive, std::vector<char>& drives, unsigned long long okCountWrite, int slowCountWrite, int verySlowCountWrite, int critCountWrite, int badCountWrite, double writeSpeed, unsigned long long availableMegabytes);
