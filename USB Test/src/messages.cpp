#include "include/messages.hpp"
#include <windows.h>
#include <iostream>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int defCol = 7, errCol = 14, selCol = 10;	//Console color settings. def - default, err - error, sel - selection; col - color

extern char* drives;
extern bool isLangRu;


/*
* @brief Message to select a drive + select logic
* @return Returns number of selected drive in array drives[], int
*/
int MsgSelectDrive() {		

	int selectedDrive;
	extern int drivesCount;

	(isLangRu) ? cout << "Введите номер съёмного диска, который нужно проверить:" << endl : cout << "Enter the removable drive number to check:" << endl; 		//Message
	cin >> selectedDrive;
	cout << endl;

	if (selectedDrive > drivesCount || cin.fail()) { MsgInputError(); }		//Select logic
	selectedDrive--;		//Array numeration starts from 0, but disk numeration in table starts from 1. Decrement to cover the difference 

	return selectedDrive;
}


/*
* @brief Message appears after input error
*/
void MsgInputError() {

	system("cls");
	SetConsoleTextAttribute(hConsole, errCol);
	(isLangRu) ? cout << "Ошибка! " : cout << "Error!";
	SetConsoleTextAttribute(hConsole, defCol);
	(isLangRu) ? cout << "Введенные данные не корректны." << endl : cout << "The input is not valid." << endl;
	system("pause"); 
	exit(1);
}


/*
* @brief Message for user to select test type + test selection logic
* @param int selectedDrive: Selected drives number in drives[]
* @return Returns selected test, int
*/
int MsgSelectTestType(int selectedDrive) {
	
	int selectedTest;

	if (isLangRu) {		//Russian
		cout << "Номер\t\tТест\t\t\tПодробнее" << endl;		//Message
		cout << "1\t\tWrite     \t\tБыстрый тест с записью данных на накопитель для проверки его настоящего объема,\n\t\t\t\t\tскорости секторов и выявления битых секторов\n\n"
			<< "2\t\tWrite&Read\t\tБолее долгий тест с записью, чтением и сравнением данных\n\t\t\t\t\tдля точной проверки секторов\n\n";
		cout << "Введите номер теста, который хотите провести:\n";
	}
	else {		//English
		cout << "No.\t\tTest\t\t\tAbout" << endl;		//Message
		cout << "1\t\tWrite     \t\tQuick test that writes data to the drive to check its real capacity,\n\t\t\t\t\tmeasure sector speed, and find any bad sectors\n\n"
			<< "2\t\tWrite&Read\t\tExtended test that writes, reads, and compares data to thoroughly\n\t\t\t\t\tcheck all sectors\n\n";
		cout << "Enter the test number you'd like to run:\n";
	}


	cin >> selectedTest;

	if (selectedTest == 1 || selectedTest == 2) {		//Test selection logic
		system("cls");
		GetDrivesInfo(drives, selectedDrive, true);

		if (isLangRu) {		//Russian
			cout << "Номер\t\tТест\t\t\tПодробнее" << endl;		//Coloring selected line (1 or 2)
			if (selectedTest == 1) { SetConsoleTextAttribute(hConsole, selCol); }
			cout << "1\t\tWrite     \t\tБыстрый тест с записью данных на накопитель для проверки его настоящего объема,\n\t\t\t\t\tскорости секторов и выявления битых секторов\n\n";
			SetConsoleTextAttribute(hConsole, defCol);

			if (selectedTest == 2) { SetConsoleTextAttribute(hConsole, selCol); }
			cout << "2\t\tWrite&Read\t\tБолее долгий тест с записью, чтением и сравнением данных\n\t\t\t\t\tдля точной проверки секторов\n\n";
			SetConsoleTextAttribute(hConsole, defCol);
		}
		else {		//English
			cout << "No.\t\tTest\t\t\tAbout" << endl;		//Coloring selected line (1 or 2)
			if (selectedTest == 1) { SetConsoleTextAttribute(hConsole, selCol); }
			cout << "1\t\tWrite     \t\tQuick test that writes data to the drive to check its real capacity,\n\t\t\t\t\tmeasure sector speed, and find any bad sectors\n\n";
			SetConsoleTextAttribute(hConsole, defCol);

			if (selectedTest == 2) { SetConsoleTextAttribute(hConsole, selCol); }
			cout << "2\t\tWrite&Read\t\tExtended test that writes, reads, and compares data to thoroughly\n\t\t\t\t\tcheck all sectors\n\n";
			SetConsoleTextAttribute(hConsole, defCol);
		}
	}
	else { MsgInputError(); }

	return selectedTest;
}


/*
* @brief Message appears before formatting selected disk + confirmation logic
* @param int selectedDrive: Selected drives number in drives[]
*/
void MsgFormatDisk(int selectedDrive) {

	string confirmation;

	if (isLangRu) {			//Russian
		SetConsoleTextAttribute(hConsole, errCol);		//Message
		cout << "Внимание! ";
		SetConsoleTextAttribute(hConsole, defCol);
		cout << "Программа создана для проверки съемных накопителей. Перед проверкой диск ";
		SetConsoleTextAttribute(hConsole, selCol);
		cout << drives[selectedDrive] << ":\\";
		SetConsoleTextAttribute(hConsole, defCol);
		cout << " будет отформатирован!" << endl << endl << endl;
	}
	else {		//English
		SetConsoleTextAttribute(hConsole, errCol);		//Message
		cout << "Warning! ";
		SetConsoleTextAttribute(hConsole, defCol);
		cout << "The program is designed to test removable drives. Before testing drive ";
		SetConsoleTextAttribute(hConsole, selCol);
		cout << drives[selectedDrive] << ":\\";
		SetConsoleTextAttribute(hConsole, defCol);
		cout << " will be formatted!" << endl << endl << endl;
	}
	
	int formatConfirm;		//Confirmation logic
	if (isLangRu) {		//Russian
		formatConfirm = MessageBox(      // Confirmation logic
			NULL,
			L"Перед началом проверки диск будет отформатирован!\n(т.е. все данные на нём будут стёрты)\n\nПеренесите все данные на другой диск и нажмите OK.",
			L"Внимание!",
			MB_ICONWARNING | MB_OKCANCEL
		);
	}
	else {		//English
		formatConfirm = MessageBox(	
			NULL,
			L"Before starting the test, the drive will be formatted!\n(all data on it will be erased).\n\nTransfer all data to another drive and press OK to continue.",
			L"Warning!",
			MB_ICONWARNING |
			MB_OKCANCEL
		);
	} 

	if (formatConfirm != IDOK) {		

		if (isLangRu) {
			SetConsoleTextAttribute(hConsole, errCol);
			cout << "Отмена теста, форматирование не будет проведено." << endl;
			SetConsoleTextAttribute(hConsole, defCol);
		}
		else {
			SetConsoleTextAttribute(hConsole, errCol);
			cout << "Test cancelled, formatting will not be performed." << endl;
			SetConsoleTextAttribute(hConsole, defCol);
		}


		system("pause");
		exit(3);
	}

	FormatDisk(selectedDrive, true);

}


/*
* @brief Message displays before starting the test
*/
void MsgBeforeTest() {
	SetConsoleTextAttribute(hConsole, 14);
	(isLangRu) ? cout << "После завершения работы программа покажет результаты тестирования\n\n" : cout << "Test results will be shown once the process is finished\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	MsgSectorsInfo();       //Displays information about different sectors health types
}


/*
* @brief Message with information about sector health types
*/
void MsgSectorsInfo() {

	if (isLangRu) {		//Russian
		SetConsoleTextAttribute(hConsole, (15 << 4) | 15);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Здоровый сектор\n";		//Message
		SetConsoleTextAttribute(hConsole, (8 << 4) | 8);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Медленный сектор ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>75 ms)\n";
		SetConsoleTextAttribute(hConsole, (6 << 4) | 6);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Очень медленный сектор ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>250 ms)\n";
		SetConsoleTextAttribute(hConsole, (4 << 4) | 4);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Критически медленный сектор ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>2 s)\n";
		SetConsoleTextAttribute(hConsole, (15 << 4) | 0);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Плохой сектор\n\n";
	}
	else {		//English
		SetConsoleTextAttribute(hConsole, (15 << 4) | 15);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Healthy sector\n";		//Message
		SetConsoleTextAttribute(hConsole, (8 << 4) | 8);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Slow sector ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>75 ms)\n";
		SetConsoleTextAttribute(hConsole, (6 << 4) | 6);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Very slow sector ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>250 ms)\n";
		SetConsoleTextAttribute(hConsole, (4 << 4) | 4);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Critically slow sector ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>2 s)\n";
		SetConsoleTextAttribute(hConsole, (15 << 4) | 0);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Bad sector\n\n";
	}

}


/*
* @brief Message displays write test results
* @param
* int slowCount, verySlowCount, critCount, badCount: write test results
* 
* double writeSpeed: write speed from write test
*/
void MsgWriteTestResults(int slowCount, int verySlowCount, int critCount, int badCount, double writeSpeed) {
	SetConsoleTextAttribute(hConsole, defCol);
	(isLangRu) ? cout << "\n\n\nРезультаты теста записи:\n" : cout << "\n\n\nWrite test results:\n";
	SetConsoleTextAttribute(hConsole, (8 << 4) | 8);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (>75 ms): " << slowCount << endl;
	SetConsoleTextAttribute(hConsole, (6 << 4) | 6);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (>250 ms): " << verySlowCount << endl;
	SetConsoleTextAttribute(hConsole, (4 << 4) | 4);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (>2 s): " << critCount << endl;
	SetConsoleTextAttribute(hConsole, (15 << 4) | 0);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (Error): " << badCount << endl;
	SetConsoleTextAttribute(hConsole, (2 << 4) | 2);	cout << "\t#";	SetConsoleTextAttribute(hConsole, defCol);	(isLangRu) ? cout << " Заявленный объем диска соответствует действительному\n" : cout << " The reported drive capacity matches the actual size\n";
	cout << "\t ";	(isLangRu) ? cout << " Скорость записи: " : cout << " Write speed: ";	cout << writeSpeed << " MB/s\n\n";
}


/*
* @brief Message displays write test results
* @param
* int slowCountWrite, verySlowCountWrite, critCountWrite, badCountWrite: write test results;
* 
* int critCount, badCount: read test results
* 
* double readSpeed, writeSpeed: write and read speed from tests
*/
void MsgReadTestResults(int slowCountWrite, int verySlowCountWrite, int critCountWrite, int badCountWrite, int critCount, int badCount, double readSpeed, double writeSpeed) {		//Message displays write and read tests results
	SetConsoleTextAttribute(hConsole, 10);
	(isLangRu) ? cout << "\nТест чтения завершен.\n\n\n\n" : cout << "\nRead test finished.\n\n\n\n";
	SetConsoleTextAttribute(hConsole, defCol);

	MsgWriteTestResults(slowCountWrite, verySlowCountWrite, critCountWrite, badCountWrite, writeSpeed);

	(isLangRu) ? cout << "\nРезультаты теста чтения:\n" : cout << "\nRead test results:\n";
	SetConsoleTextAttribute(hConsole, (4 << 4) | 4);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (>2 s): " << critCount << endl;
	SetConsoleTextAttribute(hConsole, (15 << 4) | 0);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (Error): " << badCount << endl;
	cout << "\t ";	(isLangRu) ? cout << " Скорость чтения: " : cout << " Read speed: ";	cout << readSpeed << " MB/s\n\n";
}


/*
* @brief Message appears when the real drive's capacity is less than the manufacturer claims
* @param
* int realCapacity: real capacity of the disk, calculates in drivetest.cpp, DriveTestWritePart()
*/
void MsgFakeCapacity(unsigned long long realCapacity) {		
	if (isLangRu) {		//Russian
		SetConsoleTextAttribute(hConsole, errCol);
		cout << "\n\nПохоже реальный размер вашего накопителя меньше, чем заявляет производитель!";		//Message
		SetConsoleTextAttribute(hConsole, defCol);
		cout << "\n\nЭто определяется большим количеством ошибок записи (> 1000) под ряд.";
		cout << "\nЕсли с подключением все в порядке, то реальный размер вашего накопителя - ";
		SetConsoleTextAttribute(hConsole, selCol);
		cout << realCapacity << " MB.\n\n";
		SetConsoleTextAttribute(hConsole, defCol);
	}
	else {		//English
		SetConsoleTextAttribute(hConsole, errCol);
		cout << "\n\nIt looks like the real capacity of your drive is less than the manufacturer claims!";		//Message
		SetConsoleTextAttribute(hConsole, defCol);
		cout << "\nThis is indicated by a large number of consecutive write errors (>1000).";
		cout << "\nIf the connection is stable, the real capacity of your drive is - ";
		SetConsoleTextAttribute(hConsole, selCol);
		cout << realCapacity << " MB.\n\n";
		SetConsoleTextAttribute(hConsole, defCol);
	}

	system("pause");
}

void MsgFormatBeforeExiting() {
	SetConsoleTextAttribute(hConsole, errCol);
	(isLangRu) ? cout << "\nПожалуйста, подождите. Перед закрытием программа безопасно отформатирует диск...\n\n" : cout << "\nPlease wait. The program is preparing to safely format the disk before closing...\n\n";
	SetConsoleTextAttribute(hConsole, defCol);
}