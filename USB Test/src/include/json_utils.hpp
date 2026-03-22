#pragma once

#include <string>
#include "json.hpp"
#include "lang.hpp"
#include "messages.hpp"
#include "history.hpp"
#include <string>

using json = nlohmann::json;

bool readData();
bool pushBackData(std::string driveLetter, unsigned long long totalMegabytes, int slowCount, int verySlowCount, int critCount, int critCountRead, int badCount, int badCountRead, double writeSpeed, double readSpeed);
bool pushDrivesArray();
void displayTestData(int selectedTest);
bool exportData();
bool importData();
bool compareData(json jComparisonOne, json jComparisonTwo);
bool eraseData();
std::string getDocumentsPath();

extern const std::string DATA_PATH;