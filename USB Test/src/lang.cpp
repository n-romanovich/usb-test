#include "include/lang.hpp"
#include <iostream>

//Language variable. If True - Russian; If False - English
bool isLangRu;		


/*
* @brief Identifies the system's language (Russian or English) by getting the system language id and comparing it to 1049 (Russian) 
*/
void GetSystemLanguage() {

	int lang = GetSystemDefaultLangID();		//Getting system's language id

	if (lang == 1049) { isLangRu = true;		//Comparing with 1049 (Russian)
	}
	else {
		isLangRu = false;		//If id isn't 1049,the program will use English
	}

	(isLangRu) ? setlocale(LC_ALL, "ru") : setlocale(LC_ALL, "en");

}