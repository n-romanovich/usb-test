#include "driveinfo.hpp"
#include "lang.hpp"

#pragma once

int MsgSelectDrive();
void MsgInputError();
int MsgSelectTestType(int selectedDrive);
void MsgFormatDisk(int selectedDrive);
void MsgBeforeTest();
void MsgSectorsInfo();
void MsgWriteTestResults(int slowCount, int verySlowCount, int critCount, int badCount, double writeSpeed);
void MsgReadTestResults(int slowCountWrite, int verySlowCountWrite, int critCountWrite, int badCountWrite, int critCount, int badCount, double readSpeed, double writeSpeed);
void MsgFakeCapacity(unsigned long long realCapacity);
void MsgFormatBeforeExiting();