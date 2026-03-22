#pragma once

#include "driveinfo.hpp"
#include "lang.hpp"
#include "json.hpp"
#include "menu.hpp"

using json = nlohmann::json;

int msgSelectDrive();
void msgInputError(bool isExitingNeeded);
void msgFileOpenError();
int msgSelectTestType(int selectedDrive);
void msgFormatDisk(int selectedDrive);
void msgBeforeTest();
void msgSectorsInfo();
void msgWriteTestResults(unsigned long long okCount, int slowCount, int verySlowCount, int critCount, int badCount, double writeSpeed);
void msgReadTestResults(unsigned long long okCount, int slowCountWrite, int verySlowCountWrite, int critCountWrite, int badCountWrite, int critCount, int badCount, double readSpeed, double writeSpeed);
void msgFakeCapacity(unsigned long long realCapacity);
void msgFormatBeforeExiting();
void msgDisplayTestData(json jDrives, int selectedTest, int customTestNumber);
void msgDisplayShortTestData(json jDrives, int selectedTest);
void msgDisplayTestComparison(json jComparisonOne, json jComparisonTwo);
void msgAbout();