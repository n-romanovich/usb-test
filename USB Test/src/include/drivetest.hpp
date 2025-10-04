#include "driveinfo.hpp"
#include "messages.hpp"
#include "lang.hpp"

#pragma once

bool DriveTestWritePart(int selectedDrive, char* drives, int selectedTest);
bool DriveTestReadPart(int selectedDrive, char* drives, int slowCountWrite, int verySlowCountWrite, int critCountWrite, int badCountWrite, double writeSpeed, unsigned long long availableMegabytes);
