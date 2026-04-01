#include <iostream>
#include <windows.h>
#include "include/driveinfo.hpp"
#include "include/drivetest.hpp"
#include "include/messages.hpp"
#include "include/lang.hpp"
#include "include/json_utils.hpp"
#include "include/history.hpp"
#include "include/menu.hpp"
#include "include/version.hpp"

using namespace std;

std::vector<char> drives = getDrives();		//Contains drive letters in {'C', 'D', 'E', 'F'} format

int main()
{

	checkUpdates();

	getSystemLanguage();		//Get system language. If it's not 1049 (Russian), switch to English.
	
	mainMenu();

	return 0;
}