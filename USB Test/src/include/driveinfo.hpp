#pragma once

#include "lang.hpp"
#include <vector>

std::vector<char> getDrives();
ULARGE_INTEGER getDrivesInfo(std::vector<char>& drives, int selectedDrive, bool isTableNeeded);
void formatDisk(int selectedDrive, bool isConfirmationNeeded);
DWORD getSectorSize(int selectedDrive, std::vector<char>& drives);
unsigned long long getTotalMegabytes(std::vector<char>& drives, int selectedDrive);