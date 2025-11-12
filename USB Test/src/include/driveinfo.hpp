#include "lang.hpp"
#include <vector>

#pragma once

std::vector<char> GetDrives();
ULARGE_INTEGER GetDrivesInfo(std::vector<char>& drives, int selectedDrive, bool isTableNeeded);
void FormatDisk(int selectedDrive, bool isConfirmationNeeded);
DWORD GetSectorSize(int selectedDrive, std::vector<char>& drives);