#include <iostream>
#include <windows.h>
#include "include/driveinfo.hpp"
#include "include/drivetest.hpp"
#include "include/messages.hpp"
#include "include/lang.hpp"

using namespace std;

char* drives = GetDrives();		//Char dynamic array. Contains drive letters in {'C', 'D', 'E', 'F'} format

extern bool isLangRu;
extern int drivesCount;

int main()
{
	int selectedDrive;
	int selectedTest;		//Selected test: 1 - Write only, 2 - Write, read, and compare

	GetSystemLanguage();		//Get system language. If it's not 1049 (Russian), switch to English.

	GetDrivesInfo(drives, -1, true);		//Display list of drives, highlighting drive -1 as selected (no drive selected)

	selectedDrive = MsgSelectDrive();		//Display drive selection message + Select logic

	system("cls");
	GetDrivesInfo(drives, selectedDrive, true);		//Display drives list + highlighting selected drive

	selectedTest = MsgSelectTestType(selectedDrive);		//Display list of tests + Select logic

	MsgFormatDisk(selectedDrive);		//Displaying message before formatting the disk + Confirmation logic

	system("cls");
	DriveTestWritePart(selectedDrive, drives, selectedTest);		//Perform write test. If "Write&Read" was selected, the read + compare part starts at the end of this function.

	MsgFormatBeforeExiting();		//Display a message indicating that the disk will be formatted before the program closes
	FormatDisk(selectedDrive, false);		//Format the disk before exiting the program

	return 0;
}