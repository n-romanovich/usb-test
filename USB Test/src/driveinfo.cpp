#include "include/driveinfo.hpp"
#include <iostream>
#include <windows.h>

using namespace std;

char drivesRaw[500];		//Unprocessed drives char array
int drivesCount = 0;		//Count of drives connected

extern int defCol, errCol, selCol;		//Console color codes
extern bool isLangRu;

extern HANDLE hConsole;

DISK_GEOMETRY dg;


/*
* @brief Gets drives count and all drive letters
* @return Returns processed array of chars with drive letters in it ({'C', 'D', 'E', 'F'}),    char*[]
*/
char* GetDrives() {		

	GetLogicalDriveStringsA(sizeof(drivesRaw), drivesRaw);		//Getting drives connected and putting data in drivesRaw[]

	for (int i = 0; i < 500; i++)	//Counting drives count
	{
		if (drivesRaw[i] != ':' && drivesRaw[i] != '\\' && drivesRaw[i] != '\0')		//Looking for drive letters
		{
			drivesCount++;		//Increasing drivesCount when drive letter found
		}

		if (drivesRaw[i] == '\0') {		//Looking for end of the drivesRaw[] {... \0, \0}
			++i;

			if (drivesRaw[i] == '\0') {
				break;
			}
			else { --i; }
		}
	}

	char* drivesProcessed = new char[drivesCount]();		//Initializing dynamic char array with 'drivesCount' elements.	{C, D, E, F, G}

	for (int i = 0, j = 0; j < drivesCount; i++)	//Processing drivesRaw[] information and putting it in drivesProcessed[]
	{
		if (drivesRaw[i] != ':' && drivesRaw[i] != '\\' && drivesRaw[i] != '\0')
		{
			drivesProcessed[j] = drivesRaw[i];
			j++;
		}
	}

	return drivesProcessed;
}


/*
* @brief Returns information about available bytes on disc or displays table with information about drives.
* 
* @param 
* char* drives: array with drive letters in it;
* 
* int selectedDrive: selected drives number in drives[];
* 
* bool isTableNeeded:
* When the program starts, an information table with all the data this function
* searches for is displayed on the screen. If displaying the table is not necessary,
* but rather just information about available bytes on disk is required, isTableNeeded should be False.
* 
* @return Returns available bytes count on selected drive if selectedDrive isn't NULL (isn't provided yet)
*/
ULARGE_INTEGER GetDrivesInfo(char* drives, int selectedDrive, bool isTableNeeded)
{
	ULARGE_INTEGER availableBytes, totalBytes;

	SetConsoleTextAttribute(hConsole, defCol);

	if (selectedDrive != -1) {		//If selected drive is provided
		wchar_t driveLetter = drives[selectedDrive];		//Forming drive path
		wchar_t drivePath[4] = { driveLetter, L':' ,L'\\', L'\0' };
		GetDiskFreeSpaceEx(drivePath, &availableBytes, &totalBytes, nullptr);		//Getting information about drive capacity
	}

	if (isTableNeeded) {		//Displaying table if needed

		(isLangRu) ? std::cout << "Номер\tДиск\t\tСвободное место\t\tРазмер диска" << std::endl  : std::cout << "No.\tDrive\t\tFree space\t\tDisk capacity" << std::endl; 

		for (int i = 0; i < drivesCount; i++)	//Displaying table line-by-line
		{
			wchar_t driveLetter = drives[i];
			wchar_t drivePath[4] = { driveLetter, L':' ,L'\\', L'\0' };
			GetDiskFreeSpaceEx(drivePath, &availableBytes, &totalBytes, nullptr);

			if (selectedDrive == i && selectedDrive != -1) { SetConsoleTextAttribute(hConsole, selCol); }		//Looking for a selected drive (if != -1) to color it's line
			cout << i + 1 << "\t" << drives[i] << "\t\t" << availableBytes.QuadPart / (1024ULL * 1024ULL) << " MB   \t\t";
			if (availableBytes.QuadPart < 100ULL*(1024ULL*1024ULL)) { cout << "\t"; }		//If capacity is lower than three symbols adds one tabulation
			cout << totalBytes.QuadPart / (1024ULL * 1024ULL) << " MB" << endl;

			SetConsoleTextAttribute(hConsole, defCol);
		}
		cout << endl << endl;

	}

	if (selectedDrive != -1) { return availableBytes; }
	else { return { 0 }; }
}


/*
* @brief Formats disk by it's number in array drives
* 
* @param 
* int selectedDrive: selected drives number in drives[];
* 
* bool isConfirmationNeeded: if confirmation needed (formatting before the test), message box will be shown
*/
void FormatDisk(int selectedDrive, bool isConfirmationNeeded) {

	extern char* drives;

	string format;

	//Forming a string
	format = "format ";
	format += std::string(1, drives[selectedDrive]);
	format += ": /q /x /y /FS:NTFS";

	system(format.c_str());		//Formating drive
}


/*
* @brief Calculates how many bytes per sector drive has
*
* @param
* int selectedDrive: selected drives number in drives[];
*
* char* drives: array with drive letters in it;
*
* @return Returns DWORD bd.bytesPerSector
*/
DWORD GetSectorSize(int selectedDrive, char* drives) {

	DWORD bytesReturned;

	wstring devicePath = L"\\\\.\\";       //Forms string with disk's path
	devicePath += drives[selectedDrive];
	devicePath += L":";

	HANDLE hDevice = CreateFile(	//Opening disk
		devicePath.c_str(),	    //Drive path
		GENERIC_READ,
		FILE_SHARE_READ |
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL
	);

	if (hDevice == INVALID_HANDLE_VALUE) {      //Unable to open drive
		SetConsoleTextAttribute(hConsole, errCol);
		(isLangRu) ? cout << "Не удалось открыть диск" : cout << "Unable to access drive";
		system("pause"); exit(2);
	}


	if (DeviceIoControl(		//Getting bytes per sector
		hDevice,
		IOCTL_DISK_GET_DRIVE_GEOMETRY,
		NULL, 0,
		&dg, sizeof(dg),
		&bytesReturned,
		NULL)
		) {
	}
	else {
		SetConsoleTextAttribute(hConsole, errCol);
		(isLangRu) ? cout << "Ошибка в получении размера сектора!" : cout << "Unable to determine sector size";
	}

	CloseHandle(hDevice);

	return dg.BytesPerSector;
}