#include "include/messages.hpp"
#include <windows.h>
#include <iostream>

using namespace std;


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int defCol = 7, errCol = 14, selCol = 10, menuCol = 15;	//Console color settings. def - default, err - error, sel - selection; menu - menu name color

extern std::vector<char> drives;
extern bool isLangRu;


/*
* @brief Message to select a drive + select logic
* @return Returns number of selected drive in array drives[], int
*/
int msgSelectDrive() {		

	int selectedDrive;
	extern int drivesCount;

	(isLangRu) ? cout << "Введите номер съёмного диска, который нужно проверить:" << endl : cout << "Enter the number of the removable drive to check:" << endl; 		//Message
	cin >> selectedDrive;
	cout << endl;

	if (selectedDrive > drivesCount || cin.fail()) {	//Select logic
		msgInputError(true);
	}
	selectedDrive--;		//Array numeration starts from 0, but disk numeration in table starts from 1. Decrement to cover the difference 

	return selectedDrive;
}


/*
* @brief Message appears after input error
*/
void msgInputError(bool isExitingNeeded) {

	system("cls");
	SetConsoleTextAttribute(hConsole, errCol);
	(isLangRu) ? cout << "Ошибка! " : cout << "Error! ";
	SetConsoleTextAttribute(hConsole, defCol);
	(isLangRu) ? cout << "Введенные данные не корректны." << endl : cout << "The input is not valid." << endl;
	system("pause"); 

	if (isExitingNeeded) { exit(1); }
}


/*
* @brief Message appears after file open error
*/
void msgFileOpenError() {

	system("cls");
	SetConsoleTextAttribute(hConsole, errCol);
	(isLangRu) ? cout << "Ошибка! " : cout << "Error! ";
	SetConsoleTextAttribute(hConsole, defCol);
	(isLangRu) ? cout << "Не удалось открыть файл." << endl : cout << "Unable to open file." << endl;
	system("pause");
}

/*
* @brief Message for user to select test type + test selection logic
* @param int selectedDrive: Selected drives number in drives[]
* @return Returns selected test, int
*/
int msgSelectTestType(int selectedDrive) {
	
	int selectedTest;

	if (isLangRu) {		//Russian
		cout << "Номер\t\tТест\t\t\tПодробнее" << endl;		//Message
		cout << "1\t\tWrite     \t\tБыстрый тест с записью данных на накопитель для проверки его настоящего объема,\n\t\t\t\t\tскорости секторов и выявления битых секторов. \n\n"
			<< "2\t\tWrite&Read\t\tБолее долгий тест с записью, чтением и сравнением данных\n\t\t\t\t\tдля точной проверки секторов. Результаты сохраняются в историю тестов\n\n";
		cout << "Введите номер теста, который хотите провести:\n";
	}
	else {		//English
		cout << "No.\t\tTest\t\t\tAbout" << endl;		//Message
		cout << "1\t\tWrite     \t\tQuick test that writes data to the drive to check its actual capacity,\n\t\t\t\t\tmeasure sector speed, and find any bad sectors\n\n"
			<< "2\t\tWrite&Read\t\tExtended test that writes, reads, and compares data to thoroughly\n\t\t\t\t\tcheck all sectors. Results are saved to the Test History\n\n";
		cout << "Enter the test number you'd like to run:\n";
	}


	cin >> selectedTest;

	if (selectedTest == 1 || selectedTest == 2) {		//Test selection logic
		system("cls");
		
		getDrivesInfo(drives, selectedDrive, true);

		if (isLangRu) {		//Russian
			cout << "Номер\t\tТест\t\t\tПодробнее" << endl;		//Coloring selected line (1 or 2)
			if (selectedTest == 1) { SetConsoleTextAttribute(hConsole, selCol); }
			cout << "1\t\tWrite     \t\tБыстрый тест с записью данных на накопитель для проверки его настоящего объема,\n\t\t\t\t\tскорости секторов и выявления битых секторов\n\n";
			SetConsoleTextAttribute(hConsole, defCol);

			if (selectedTest == 2) { SetConsoleTextAttribute(hConsole, selCol); }
			cout << "2\t\tWrite&Read\t\tБолее долгий тест с записью, чтением и сравнением данных\n\t\t\t\t\tдля точной проверки секторов. Результаты сохраняются в историю тестов\n\n";
			SetConsoleTextAttribute(hConsole, defCol);
		}
		else {		//English
			cout << "No.\t\tTest\t\t\tAbout" << endl;		//Coloring selected line (1 or 2)
			if (selectedTest == 1) { SetConsoleTextAttribute(hConsole, selCol); }
			cout << "1\t\tWrite     \t\tQuick test that writes data to the drive to check its actual capacity,\n\t\t\t\t\tmeasure sector speed, and find any bad sectors\n\n";
			SetConsoleTextAttribute(hConsole, defCol);

			if (selectedTest == 2) { SetConsoleTextAttribute(hConsole, selCol); }
			cout << "2\t\tWrite&Read\t\tExtended test that writes, reads, and compares data to thoroughly\n\t\t\t\t\tcheck all sectors. Results are saved to the Test History\n\n";
			SetConsoleTextAttribute(hConsole, defCol);
		}
	}
	else { 
		msgInputError(false);

		system("cls");
		msgSelectTestType(selectedDrive);
	}

	return selectedTest;
}


/*
* @brief Message appears before formatting selected disk + confirmation logic
* @param int selectedDrive: Selected drives number in drives[]
*/
void msgFormatDisk(int selectedDrive) {

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

	formatDisk(selectedDrive, true);

}


/*
* @brief Message displays before starting the test
*/
void msgBeforeTest() {
	SetConsoleTextAttribute(hConsole, 14);
	(isLangRu) ? cout << "После завершения работы программа покажет результаты тестирования\n\n" : cout << "Test results will be shown once the process is finished\n\n";
	SetConsoleTextAttribute(hConsole, 7);
	msgSectorsInfo();       //Displays information about different sectors health types
}


/*
* @brief Message with information about sector health types
*/
void msgSectorsInfo() {

	if (isLangRu) {		//Russian
		SetConsoleTextAttribute(hConsole, (15 << 4) | 15);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Здоровый сектор ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(OK)\n";		//Message
		SetConsoleTextAttribute(hConsole, (8 << 4) | 8);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Медленный сектор ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>75 ms)\n";
		SetConsoleTextAttribute(hConsole, (6 << 4) | 6);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Очень медленный сектор ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>250 ms)\n";
		SetConsoleTextAttribute(hConsole, (4 << 4) | 4);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Критически медленный сектор ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(>2 s)\n";
		SetConsoleTextAttribute(hConsole, (15 << 4) | 0);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Плохой сектор\n\n";
	}
	else {		//English
		SetConsoleTextAttribute(hConsole, (15 << 4) | 15);   std::cout << "#";   SetConsoleTextAttribute(hConsole, defCol);   std::cout << " - Healthy sector ";	SetConsoleTextAttribute(hConsole, 8);	cout << "(OK)\n";		//Message
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
* unsigned long long okCount: good sectors count
* 
* double writeSpeed: write speed from write test
*/
void msgWriteTestResults(unsigned long long okCount, int slowCount, int verySlowCount, int critCount, int badCount, double writeSpeed) {
	SetConsoleTextAttribute(hConsole, defCol);
	(isLangRu) ? cout << "\n\n\nРезультаты теста записи:\n" : cout << "\n\n\nWrite test results:\n";
	SetConsoleTextAttribute(hConsole, (15 << 4) | 15);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (OK): " << okCount << endl;
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
* unsigned long long okCountWrite: good sectors count
* 
* int critCount, badCount: read test results
* 
* double readSpeed, writeSpeed: write and read speed from tests
*/
void msgReadTestResults(unsigned long long okCountWrite, int slowCountWrite, int verySlowCountWrite, int critCountWrite, int badCountWrite, int critCount, int badCount, double readSpeed, double writeSpeed) {		//Message displays write and read tests results
	SetConsoleTextAttribute(hConsole, 10);
	(isLangRu) ? cout << "\nТест чтения завершен.\n\n\n\n" : cout << "\nRead test finished.\n\n\n\n";
	SetConsoleTextAttribute(hConsole, defCol);

	msgWriteTestResults(okCountWrite, slowCountWrite, verySlowCountWrite, critCountWrite, badCountWrite, writeSpeed);

	(isLangRu) ? cout << "\nРезультаты теста чтения:\n" : cout << "\nRead test results:\n";
	SetConsoleTextAttribute(hConsole, (4 << 4) | 4);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (>2 s): " << critCount << endl;
	SetConsoleTextAttribute(hConsole, (15 << 4) | 0);   cout << "\t#";   SetConsoleTextAttribute(hConsole, defCol);   cout << " (Error): " << badCount << endl;
	cout << "\t ";	(isLangRu) ? cout << " Скорость чтения: " : cout << " Read speed: ";	cout << readSpeed << " MB/s\n\n";
}


/*
* @brief Message appears when the actual drive's capacity is less than the manufacturer claims
* @param
* int actualCapacity: actual capacity of the disk, calculates in drivetest.cpp, driveTestWritePart()
*/
void msgFakeCapacity(unsigned long long actualCapacity) {		
	if (isLangRu) {		//Russian
		SetConsoleTextAttribute(hConsole, errCol);
		cout << "\n\nПохоже реальный размер вашего накопителя меньше, чем заявляет производитель!";		//Message
		SetConsoleTextAttribute(hConsole, defCol);
		cout << "\n\nЭто определяется большим количеством ошибок записи (> 1000) под ряд.";
		cout << "\nЕсли с подключением все в порядке, то реальный размер вашего накопителя - ";
		SetConsoleTextAttribute(hConsole, selCol);
		cout << actualCapacity << " MB.\n\n";
		SetConsoleTextAttribute(hConsole, defCol);
	}
	else {		//English
		SetConsoleTextAttribute(hConsole, errCol);
		cout << "\n\nIt looks like the actual capacity of your drive is less than the manufacturer claims!";		//Message
		SetConsoleTextAttribute(hConsole, defCol);
		cout << "\nThis is indicated by a large number of consecutive write errors (>1000).";
		cout << "\nIf the connection is stable, the actual capacity of your drive is - ";
		SetConsoleTextAttribute(hConsole, selCol);
		cout << actualCapacity << " MB.\n\n";
		SetConsoleTextAttribute(hConsole, defCol);
	}

	system("pause");
}


/*
* @brief Message appears before formatting the drive to end the test
*/
void msgFormatBeforeExiting() {
	(isLangRu) ? cout << "\nПеред выходом в меню программа безопасно отформатирует диск...\n\n" : cout << "\nThe drive will be safely formatted before exiting to the menu...\n\n";
}


/*
* @brief Message displays specific test data
* 
* @param
* json jDrives: JSON array with test data
* 
* int selectedTest: Test's number
* 
* int customTestNumber: if (-1) - function displays (selectedTest + 1), else displays custom number
*/
void msgDisplayTestData(json jDrives, int selectedTest, int customTestNumber) {

	if (isLangRu) {		//Russian
		cout << "===================================\n";

		(customTestNumber == -1) ? cout << "Тест номер " << selectedTest + 1 << endl : cout << "Тест номер " << customTestNumber << endl;	//Display test number

		cout
			<< "Буква диска: " << jDrives[selectedTest]["drive_letter"] << endl
			<< "Ёмкость: " << jDrives[selectedTest]["total_mb"] << " MB" << endl
			<< "\nСектора:\n"
			<< "\tМедленных секторов: " << jDrives[selectedTest]["slow_count"] << endl
			<< "\tОчень медленных секторов: " << jDrives[selectedTest]["very_slow_count"] << endl
			<< "\tКрит. секторов: " << jDrives[selectedTest]["crit_count"] << endl
			<< "\tПлохих секторов: " << jDrives[selectedTest]["bad_count"] << endl
			<< "\n\tКрит. секторов (чтение): " << jDrives[selectedTest]["crit_count_read"] << endl
			<< "\tПлохих секторов (чтение): " << jDrives[selectedTest]["bad_count_read"] << endl
			<< "\nСкорость:\n"
			<< "\tСкорость записи: " << jDrives[selectedTest]["write_speed"] << " MB/s" << endl
			<< "\tСкорость чтения: " << jDrives[selectedTest]["read_speed"] << " MB/s" << endl;
		cout << "===================================\n\n\n";
	}
	else {		//English
		cout << "==============================\n";

		(customTestNumber == -1) ? cout << "Test No. " << selectedTest + 1 << endl : cout << "Test No. " << customTestNumber << endl;		//Display test number

		cout 
			<< "Drive letter: " << jDrives[selectedTest]["drive_letter"] << endl
			<< "Capacity: " << jDrives[selectedTest]["total_mb"] << " MB" << endl
			<< "\nSectors:\n"
			<< "\tSlow sectors: " << jDrives[selectedTest]["slow_count"] << endl
			<< "\tVery slow sectors: " << jDrives[selectedTest]["very_slow_count"] << endl
			<< "\tCrit sectors: " << jDrives[selectedTest]["crit_count"] << endl
			<< "\tBad sectors: " << jDrives[selectedTest]["bad_count"] << endl
			<< "\n\tCrit sectors (read): " << jDrives[selectedTest]["crit_count_read"] << endl
			<< "\tBad sectors (read): " << jDrives[selectedTest]["bad_count_read"] << endl
			<< "\nSpeed:\n"
			<< "\tWrite speed: " << jDrives[selectedTest]["write_speed"] << " MB/s" << endl
			<< "\tRead speed: " << jDrives[selectedTest]["read_speed"] << " MB/s" << endl;
		cout << "==============================\n\n\n";
	}

}


/*
* @brief Message displays test short data
*/
void msgDisplayShortTestData(json jDrives, int selectedTest) {

	if (isLangRu) {
		SetConsoleTextAttribute(hConsole, menuCol);
		cout << "No. " << selectedTest + 1 << ";";
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "  БУКВ: " << jDrives[selectedTest]["drive_letter"] << ";  ЁМК: " << jDrives[selectedTest]["total_mb"] << " MB; МЕДЛ: " << jDrives[selectedTest]["slow_count"] << endl;
	} 
	else {
		SetConsoleTextAttribute(hConsole, menuCol);
		cout << "No. " << selectedTest + 1 << ";";
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "  LTR: " << jDrives[selectedTest]["drive_letter"] << ";  SPACE: " << jDrives[selectedTest]["total_mb"] << " MB; SLOW: " << jDrives[selectedTest]["slow_count"] << endl;
	}	
}

/*
* @brief Displays test comparison
* 
* @param
* json jComparisonOne: JSON array one
* json jCompariosnTwo: JSON array two
* 
*/
void msgDisplayTestComparison(json jComparisonOne, json jComparisonTwo) {
	if (isLangRu) {		//Russian
		cout << "===================================\n";

		cout << "Тест номер 2 (Сравнение) \n";

		cout << "Буква диска: " << jComparisonTwo[0]["drive_letter"];
		if (jComparisonOne[0]["drive_letter"] != jComparisonTwo[0]["drive_letter"]) { SetConsoleTextAttribute(hConsole, 14);	cout << " (?)\n"; }
		else { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "Ёмкость: " << jComparisonTwo[0]["total_mb"] << " MB";
		if ((jComparisonOne[0]["total_mb"] == jComparisonTwo[0]["total_mb"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {

			if ((jComparisonOne[0]["total_mb"] < jComparisonTwo[0]["total_mb"])) { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }

			else { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "\nСектора:\n";
		cout << "\tМедленных секторов: " << jComparisonTwo[0]["slow_count"];
		if ((jComparisonOne[0]["slow_count"] == jComparisonTwo[0]["slow_count"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["slow_count"] > jComparisonTwo[0]["slow_count"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "\tОчень медленных секторов: " << jComparisonTwo[0]["very_slow_count"];
		if ((jComparisonOne[0]["very_slow_count"] == jComparisonTwo[0]["very_slow_count"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["very_slow_count"] > jComparisonTwo[0]["very_slow_count"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "\tКрит. секторов: " << jComparisonTwo[0]["crit_count"];
		if ((jComparisonOne[0]["crit_count"] == jComparisonTwo[0]["crit_count"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["crit_count"] > jComparisonTwo[0]["crit_count"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "\tПлохих секторов: " << jComparisonTwo[0]["bad_count"];
		if ((jComparisonOne[0]["bad_count"] == jComparisonTwo[0]["bad_count"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["bad_count"] > jComparisonTwo[0]["bad_count"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "\n\tКрит. секторов (чтение): " << jComparisonTwo[0]["crit_count_read"];
		if ((jComparisonOne[0]["crit_count_read"] == jComparisonTwo[0]["crit_count_read"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["crit_count_read"] > jComparisonTwo[0]["crit_count_read"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "\tПлохих секторов (чтение): " << jComparisonTwo[0]["bad_count_read"];
		if ((jComparisonOne[0]["bad_count_read"] == jComparisonTwo[0]["bad_count_read"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["bad_count_read"] > jComparisonTwo[0]["bad_count_read"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "\nСкорость:\n";
		cout << "\tСкорость записи: " << jComparisonTwo[0]["write_speed"] << " MB/s";
		if ((jComparisonOne[0]["write_speed"] == jComparisonTwo[0]["write_speed"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["write_speed"] < jComparisonTwo[0]["write_speed"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "\tСкорость чтения: " << jComparisonTwo[0]["read_speed"] << " MB/s";
		if ((jComparisonOne[0]["read_speed"] == jComparisonTwo[0]["read_speed"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["read_speed"] < jComparisonTwo[0]["read_speed"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);


		cout << "===================================\n\n\n";
	}
	else {		//English
		cout << "===================================\n";

		cout << "Test No. 2 (Comparison) \n";

		cout << "Drive letter: " << jComparisonTwo[0]["drive_letter"];
		if (jComparisonOne[0]["drive_letter"] != jComparisonTwo[0]["drive_letter"]) { SetConsoleTextAttribute(hConsole, 14);	cout << " (?)\n"; }
		else { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "Capacity: " << jComparisonTwo[0]["total_mb"] << " MB";
		if ((jComparisonOne[0]["total_mb"] == jComparisonTwo[0]["total_mb"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["total_mb"] < jComparisonTwo[0]["total_mb"])) { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
			else { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\nSectors:\n";
		cout << "\tSlow sectors: " << jComparisonTwo[0]["slow_count"];
		if ((jComparisonOne[0]["slow_count"] == jComparisonTwo[0]["slow_count"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["slow_count"] > jComparisonTwo[0]["slow_count"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\tVery slow sectors: " << jComparisonTwo[0]["very_slow_count"];
		if ((jComparisonOne[0]["very_slow_count"] == jComparisonTwo[0]["very_slow_count"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["very_slow_count"] > jComparisonTwo[0]["very_slow_count"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\tCrit sectors: " << jComparisonTwo[0]["crit_count"];
		if ((jComparisonOne[0]["crit_count"] == jComparisonTwo[0]["crit_count"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["crit_count"] > jComparisonTwo[0]["crit_count"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\tBad sectors: " << jComparisonTwo[0]["bad_count"];
		if ((jComparisonOne[0]["bad_count"] == jComparisonTwo[0]["bad_count"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["bad_count"] > jComparisonTwo[0]["bad_count"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\n\tCrit sectors (read): " << jComparisonTwo[0]["crit_count_read"];
		if ((jComparisonOne[0]["crit_count_read"] == jComparisonTwo[0]["crit_count_read"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["crit_count_read"] > jComparisonTwo[0]["crit_count_read"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\tBad sectors (read): " << jComparisonTwo[0]["bad_count_read"];
		if ((jComparisonOne[0]["bad_count_read"] == jComparisonTwo[0]["bad_count_read"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["bad_count_read"] > jComparisonTwo[0]["bad_count_read"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\nSpeed:\n";
		cout << "\tWrite speed: " << jComparisonTwo[0]["write_speed"] << " MB/s";
		if ((jComparisonOne[0]["write_speed"] == jComparisonTwo[0]["write_speed"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["write_speed"] < jComparisonTwo[0]["write_speed"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\tRead speed: " << jComparisonTwo[0]["read_speed"] << " MB/s";
		if ((jComparisonOne[0]["read_speed"] == jComparisonTwo[0]["read_speed"])) { SetConsoleTextAttribute(hConsole, 8);	cout << " (=)\n"; }
		else {
			if ((jComparisonOne[0]["read_speed"] < jComparisonTwo[0]["read_speed"])) { SetConsoleTextAttribute(hConsole, 10);	cout << " (+)\n"; }
			else { SetConsoleTextAttribute(hConsole, 12);	cout << " (-)\n"; }
		}
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "===================================\n\n\n";
	}
}

/*
* @brief Message displays information about USB Test
*/
void msgAbout() {
	system("title USB TEST - About");

	system("cls");
	SetConsoleTextAttribute(hConsole, menuCol);
	cout << "USB Test - A lightweight Windows CLI utility for testing removable drives \n\n";
	SetConsoleTextAttribute(hConsole, defCol);

	cout << "Author: n-romanovich on GitHub \n";
	cout << "Official website: usbtest.page.gd \n";
	cout << "SourceForge: usb-test.sourceforge.io \n\n";

	cout << "Version: 1.1\n\n";

	cout << "Thanks for using USB Test! \n\n";

	system("pause");

	mainMenu();

}