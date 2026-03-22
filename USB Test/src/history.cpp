#include <iostream>
#include <string>
#include <fstream>
#include "include/history.hpp"

using namespace std;
using json = nlohmann::json;		//nlohmann's JSON lib

extern json jDrives;
extern bool isLangRu;

extern HANDLE hConsole;
extern int menuCol, defCol, errCol;

/*
* @brief Displays test history
*/
void viewHistory() {
	system("cls");
	readData();

	for (int i = 0; i < jDrives.size(); i++)
	{
		displayTestData(i);		//Displays each test record
	}
	system("pause");
	menuHistory();
}


/*
* @brief Test Import/Export menu
*/
void menuImportExport() {
	system("title USB TEST - Import/Export");

	system("cls");
	int selectedOption = -1;

	if (isLangRu) {
		SetConsoleTextAttribute(hConsole, menuCol);
		cout << "ИМПОРТ И ЭКСПОРТ";
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\n\n1. \tИмпортировать тест из файла";
		cout << "\n2. \tЭкспортировать тест в файл";
		cout << "\n3. \tНазад";
		cout << "\n\nВведите номер пункта меню: \n";
	}
	else {
		SetConsoleTextAttribute(hConsole, menuCol);
		cout << "IMPORT AND EXPORT";
		SetConsoleTextAttribute(hConsole, defCol);

		cout << "\n\n1. \tImport test from file";
		cout << "\n2. \tExport test to file";
		cout << "\n3. \tBack";
		cout << "\n\nEnter the menu option number: \n";
	}
	cin >> selectedOption;

	if (selectedOption > 3 || selectedOption < 1 || cin.fail()) {
		msgInputError(false);
		system("cls");
		menuImportExport();
	}

	//Menu logic
	switch (selectedOption) {
		case 1:
			importData();

			system("cls");
			menuHistory();
			break;
		case 2:
			exportData();

			system("cls");
			menuHistory();
			break;
		case 3:
			menuHistory();
			break;
	}
}


/*
* @brief Compares two tests
*
* @param
* int optionOneType: type of first option (pick result from history/import result)
* 
* int optionTwoType: type of second option (pick result from history/import result)
* 
* string optionOneValue: result number in test history or the file path to import
* 
* string optionTwoValue: result number in test history or the file path to import
*/
void compareTests(int optionOneType, int optionTwoType, string optionOneValue, string optionTwoValue) {

	readData();

	json jComparisonOne = json::array();		//Test one
	json jComparisonTwo = json::array();		//Test two


	if (optionOneType == 1) {		//If test from history
		int valueOne = stoi(optionOneValue);
		jComparisonOne.push_back(jDrives[valueOne - 1]);
	} else {		//Else import test

		ifstream file(optionOneValue);
		if (!file.is_open()) {
			msgFileOpenError();
			mainMenu();

		}
		file >> jComparisonOne;

		file.close();

	}

	if (optionTwoType == 1) {		//If test from history
		int valueTwo = stoi(optionTwoValue);
		jComparisonTwo.push_back(jDrives[valueTwo - 1]);
	} else {		//Else import test

		ifstream file(optionTwoValue);
		if (!file.is_open()) {
			msgFileOpenError();
			mainMenu();

		}
		file >> jComparisonTwo;

		file.close();
	}

	compareData(jComparisonOne, jComparisonTwo);

	system("pause");
}