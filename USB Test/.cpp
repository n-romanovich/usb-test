#include <iostream>
#include <ctime>
#include <fstream>
#include <windows.h>
#include "driveinfo.h"

using namespace std;

int main()
{
	int drivesCount = getDrivesCount();
	char* drives = getDrives(drivesCount);

	for (int i = 0; i < drivesCount; i++)
	{
		cout << drives[i];
	}
}