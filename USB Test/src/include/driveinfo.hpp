#include "lang.hpp"

#pragma once

char* GetDrives();
ULARGE_INTEGER GetDrivesInfo(char* drives, int selectedDrive, bool isTableNeeded);
void FormatDisk(int selectedDrive, bool isConfirmationNeeded);
DWORD GetSectorSize(int selectedDrive, char* drives);