#include "include/lang.hpp"
#include <iostream>
#include <windows.h>

//Language variable. If True - Russian; If False - English
bool isLangRu;

const int RUSSIAN_LANG_ID = 1049;


/*
* @brief Identifies the system's language (Russian or English) by getting the system language id and comparing it to 1049 (Russian) 
*/
void getSystemLanguage() {

	int lang = GetSystemDefaultLangID();		//Getting system's language id

	if (lang == RUSSIAN_LANG_ID) {		//Comparing with 1049 (Russian)
		isLangRu = true;
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
	}
	else {
		isLangRu = false;		//If id isn't 1049,the program will use English
	}

	(isLangRu) ? setlocale(LC_ALL, "ru") : setlocale(LC_ALL, "en");

}