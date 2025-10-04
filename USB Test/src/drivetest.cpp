#include "include/drivetest.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <chrono>
#include <windows.h>

using namespace std;

size_t sectorsCount;
string testFilePath;
CONSOLE_SCREEN_BUFFER_INFO csbi;

extern HANDLE hConsole;
extern bool isLangRu;
extern int defCol, errCol, selCol;
extern DISK_GEOMETRY dg;


/*
* @brief Write test part
*
* @param
* int selectedDrive: Selected drives number in drives[];
* 
* char* drives: Array with drive letters in it;
* 
* int selectedTest: selected test number (1 - write, 2 - write and read)
*
* @return Returns logical true if test completed successfully or false if not
*/
bool DriveTestWritePart(int selectedDrive, char* drives, int selectedTest) {

    GetConsoleScreenBufferInfo(hConsole, &csbi);        //Calculating console width
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    DWORD bytesPerSector = GetSectorSize(selectedDrive, drives);        //Getting bytes per sector count

    int slowCount = 0, verySlowCount = 0, critCount = 0, badCount = 0;
    int badSectorsRowCount = 0;     //Counts bad sectors in a row to detect fake drive capacity

    dg.BytesPerSector = GetSectorSize(selectedDrive, drives);       //Getting one sector size in bytes

    string driveLetter(1, drives[selectedDrive]);  //Getting drive letter
    testFilePath = driveLetter + ":\\usb-test";     //Forming path to test file

    ULARGE_INTEGER availableBytes = GetDrivesInfo(drives, selectedDrive, false);        //Gets how many bytes are available to user on the selected drive
    unsigned long long availableMegabytes = (availableBytes.QuadPart / (1024ULL * 1024ULL));        //Gets how many Megabytes are available to user on the selected drive

    sectorsCount = static_cast<size_t>(availableBytes.QuadPart / dg.BytesPerSector);        //Counting sectors by dividing capacity (bytes) by one sector size

    MsgBeforeTest();        //Displays sector health types and other information before testing

    system("pause");
    system("cls");

    MsgBeforeTest();    //Displays sector status types and other information before testing on a blank screen


    //Starting write test
    vector<char> buffer(dg.BytesPerSector);     //Initializing buffer with size of one sector

    ofstream file(testFilePath, ios::binary);       //Opening file
    if (!file) {
        SetConsoleTextAttribute(hConsole, errCol);
        (isLangRu) ? cerr << "Ошибка при создании файла\n" : cout << "Failed to create file\n";
        return false;
    }

    auto startWrite = chrono::high_resolution_clock::now();     //Write speed

    //Writing data on disk, catching errors, counting time of one operation
    for (size_t sector = 0, i = 0, j = 0, currentChar = 0; sector < sectorsCount; ++sector, ++i, ++j) {
        memset(buffer.data(), 0, dg.BytesPerSector);        //Filling the buffer with zeros
        memcpy(buffer.data(), &sector, sizeof(sector));     //Filling the buffer with 'sector' data

        //Writing data on disk and counting operation time
        auto start = chrono::high_resolution_clock::now();
        file.write(buffer.data(), dg.BytesPerSector);
        file.flush();
        auto end = chrono::high_resolution_clock::now();

        auto writeDuration = chrono::duration_cast<chrono::milliseconds>(end - start).count(); 


        //If 1000 sectors in a row are bad, then most likely the actual disk capacity has run out and is significantly less than stated
        if (badSectorsRowCount > 1000) {        

            //Calculating the real capacity by subtracting the currently available bytes from the bytes available at the start of the test.
            unsigned long long realCapacity = (availableMegabytes - ((GetDrivesInfo(drives, selectedDrive, false).QuadPart) / (1024ULL * 1024ULL)));

            MsgFakeCapacity(realCapacity);      //Displaying information about fake capacity

            return false;
        }


        if (!file) {        //Bad sector if writing fails
            SetConsoleTextAttribute(hConsole, (15 << 4) | 0);
            cout << "#";
            badCount++;
            currentChar++;
            badSectorsRowCount++;       //Count bad sectors in row
        }

        if (writeDuration > 2000) {      //Critically slow sector if write duration is more than 2 seconds
            SetConsoleTextAttribute(hConsole, (4 << 4) | 4);
            cout << "#";
            critCount++;
            currentChar++;
            badSectorsRowCount = 0;

            continue;
        }

        if (writeDuration > 250) {        //Very slow sector if write duration is more than 250 ms
            SetConsoleTextAttribute(hConsole, (6 << 4) | 6);
            cout << "#";
            verySlowCount++;
            currentChar++;
            badSectorsRowCount = 0;

        } 
        
        if (writeDuration > 75 && writeDuration < 250) {      //Slow sector if write duration is more than 100 ms
            SetConsoleTextAttribute(hConsole, (8 << 4) | 8);
            cout << "#";
            slowCount++;
            currentChar++;
            badSectorsRowCount = 0;

            continue;
        } else {

            if (i >= 5000) {        //Displays every 5000-th healthy sector
                SetConsoleTextAttribute(hConsole, (15 << 4) | 15);       //Sector is healthy if it isn't slow or bad
                cout << "#";
                badSectorsRowCount = 0;
                currentChar++;
                i = 0;
            }
            
        }

        if (currentChar >= consoleWidth) {      //Start a new line if the number of characters equals the console width
            SetConsoleTextAttribute(hConsole, defCol);
            cout << "\n";
            currentChar = 0;
        }

    }

    //Write speed
    auto endWrite = chrono::high_resolution_clock::now();
    auto writeTime = (chrono::duration_cast<chrono::milliseconds>(endWrite - startWrite).count()) / 1000.0;     //Write time in seconds
    double writeSpeed = availableMegabytes / writeTime;
    writeSpeed = std::round(writeSpeed * 10.0) / 10.0;

    SetConsoleTextAttribute(hConsole, (0 << 4) | 10);        //End of the write test
    (isLangRu) ? cout << "\nТест записи завершен.\n\n" : cout << "\nWrite test finished.\n\n";


    //If selected test is 2 (write, read, and comparison test), then it will be executed
    if (selectedTest == 2) { DriveTestReadPart(selectedDrive, drives, slowCount, verySlowCount, critCount, badCount, writeSpeed, availableMegabytes);
    } else {
        MsgWriteTestResults(slowCount, verySlowCount, critCount, badCount, writeSpeed);     //Or else displaying write test results
        system("pause");
    }

    return true;
}


/*
* @brief Read test part
*
* @param
* int selectedDrive: Selected drives number in drives[];
* 
* char* drives: Array with drive letters in it;
* 
* int slowCountWrite, verySlowCountWrite, critCountWrite, badCountWrite: test results from write (previous) test
* 
* double writeSpeed: write speed from previous test to display results correctly
* 
* unsigned long long availableMegabytes: available drive capacity to calculate write and read speed
*
* @return Returns logical true if test completed successfully or false if not
*/
bool DriveTestReadPart(int selectedDrive, char* drives, int slowCountWrite, int verySlowCountWrite, int critCountWrite, int badCountWrite, double writeSpeed, unsigned long long availableMegabytes) {		//Read + comparison test part, returns logical true or false,   BOOL

    GetConsoleScreenBufferInfo(hConsole, &csbi);        //Calculating console width
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    DWORD bytesPerSector = GetSectorSize(selectedDrive, drives);        //Getting bytes per sector count

    int critCount = 0, badCount = 0;

    dg.BytesPerSector = GetSectorSize(selectedDrive, drives);       //Getting one sector size in bytes

    vector<char> buffer(dg.BytesPerSector);     //Initializing buffer with size of one sector


    ifstream file(testFilePath, ios::binary);       //Opening test file
    if (!file) {
        SetConsoleTextAttribute(hConsole, errCol);
        (isLangRu) ? cerr << "Ошибка при открытии файла для проверки\n" : cerr << "Unable to open file for testing\n";
        return false;
    }

    auto startRead = chrono::high_resolution_clock::now();     //Read speed

    //Reading information from test file, comparing it, and counting time
    for (size_t sector = 0, i = 0, currentChar = 0; sector < sectorsCount; ++sector, ++i) {
        auto start = chrono::high_resolution_clock::now();
        file.read(buffer.data(), dg.BytesPerSector);    //Reading 'dg.BytesPerSector' byte and putting it in 'buffer'
        auto end = chrono::high_resolution_clock::now();

        auto readDuration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

        size_t stored_number;
        memcpy(&stored_number, buffer.data(), sizeof(stored_number));       //Putting data from 'buffer' in 'stored_number'

        if (!file || stored_number != sector) {        //Bad sector if reading fails or data is invalid (comparing data)
            SetConsoleTextAttribute(hConsole, (15 << 4) | 0);
            cout << "#";
            currentChar++;
            badCount++;
        }

        if (readDuration > 2000) {     //Critically slow sector if read duration is more than 2 s
            SetConsoleTextAttribute(hConsole, (4 << 4) | 4);
            cout << "#";
            critCount++;
            currentChar++;

            continue;
        }

        if (readDuration > 75 && readDuration < 250) {       //Slow sector if read duration is more than 100 ms
            SetConsoleTextAttribute(hConsole, (8 << 4) | 8);
            cout << "#";
            currentChar++;

            continue;
        }

        if (readDuration > 250) {       //Very slow sector if read duration is more than 250 ms
            SetConsoleTextAttribute(hConsole, (6 << 4) | 6);
            cout << "#";
            currentChar++;

            continue;

        } else {

            if (i > 5000) {     //Displaying every 5000-th healthy sector
                SetConsoleTextAttribute(hConsole, (15 << 4) | 15);        //If sector isn't bad or slow, then it's healthy
                cout << "#";
                currentChar++;

                i = 0;
            }
        }

        if (currentChar >= consoleWidth) {      //Start a new line if the number of characters equals the console width
            SetConsoleTextAttribute(hConsole, defCol);
            cout << "\n";
            currentChar = 0;
        }
    }

    //Read speed
    auto endRead = chrono::high_resolution_clock::now();
    auto readTime = (chrono::duration_cast<chrono::milliseconds>(endRead - startRead).count()) / 1000.0;     //Write time in seconds
    double readSpeed = availableMegabytes / readTime;
    readSpeed = std::round(readSpeed * 10.0) / 10.0;

    SetConsoleTextAttribute(hConsole, 7);       //Read test end

    MsgReadTestResults(slowCountWrite, verySlowCountWrite, critCountWrite, badCountWrite, critCount, badCount, readSpeed, writeSpeed);     //Displaying write and read tests results
    system("pause");

    return true;
}