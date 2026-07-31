#include "include/feedback.hpp"
#include <iostream>
#include <windows.h>
#include <winhttp.h>

const char* MSG_SHOWN_REG_VAL = "FeedbackMsgShown1";

extern bool isLangRu;

using namespace std;

/*
* @brief Collects user feedback and sends to the server
* 
* @param int dialogAnswer: user's choice (IDYES/IDNO)
*/
void collectFeedback(int dialogAnswer) {
	
	system("cls");

	if (dialogAnswer == IDYES) {
		string feedback = msgFeedback();
		sendFeedback(feedback);

	}
	else if (dialogAnswer == IDNO) {
		string feedback = "DIDN'T ANSWER";
		sendFeedback(feedback);
	}

	editRegistry();
}


/*
* @brief Displays a Windows-style message box
* 
* @return Returns user's choice
*/
int displayFeedbackDialog() {
	if (!checkMsgAlreadyShown()) {
		int dialogResult;
		if (isLangRu) {
			dialogResult = MessageBox(
				NULL,
				L"USB Test - бесплатное open-source ПО, созданное одним разработчиком.\n\nЕсли программа вам помогла - помогите взамен, оценив работу (1-5 баллов).\n\nНажмите Да, чтобы ПРЯМО В КОНСОЛИ поделиться вашим мнением после результатов теста (без регистрации, 2 сек).\n\n*(Это сообщение не появится снова)",
				L"Вопрос",
				MB_YESNO | MB_ICONQUESTION
			);
		}
		else {
			dialogResult = MessageBox(
				NULL,
				L"USB Test is free, open-source, and indie-developed.\n\nIf it helped, I’d love to get your personal feedback (1-5 rating).\n\nClick Yes to share your thoughts DIRECTLY IN THE CONSOLE after the results (no signup, 2 seconds).\n\n*(This message won't show again)",
				L"Question",
				MB_YESNO | MB_ICONQUESTION
			);
		}

		return dialogResult;
	}

	return -1;
}


/*
* @brief Checks the registry to see whether the message was displayed
* 
* @return Returns true/false
*/
bool checkMsgAlreadyShown() {
	HKEY hKey;
	DWORD value = 0;
	DWORD size = sizeof(DWORD);

	//If HKCU\Software\USBTest key exists
	if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\USBTest", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
		
		LSTATUS status = RegQueryValueExA(hKey, MSG_SHOWN_REG_VAL, NULL, NULL, (LPBYTE)&value, &size);
		RegCloseKey(hKey);

		//If HKCU\Software\USBTest\MSG_SHOWN_REG_VAL value exists AND equals 1
		if (status == ERROR_SUCCESS && value == 1) {
			return true;
		}

		return false;
	}
	else { return false; }
}


/*
* @brief Creates USBTest\MSG_SHOWN_REG_VAL value at HKEY_CURRENT_USER\Software
*/
void editRegistry() {
	HKEY hKey;
	//If HKCU\Software\USBTest key was successfully created
	if (RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\USBTest", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {

		//Create HKCU\Software\USBTest\MSG_SHOWN_REG_VAL value = 1
		DWORD value = 1;
		RegSetValueExA(hKey, MSG_SHOWN_REG_VAL, 0, REG_DWORD, (const BYTE*)&value, sizeof(value));

		RegCloseKey(hKey);
	}
}


/*
* @brief Send user feedback to the server
* 
* @param string feedback: user's review
*/
void sendFeedback(string feedback) {
	string postData = "key=usbtestfb&feedback=" + feedback;

	HINTERNET hSession = WinHttpOpen(L"USBTEST", 0, 0, 0, 0);
	HINTERNET hConnect = WinHttpConnect(hSession, L"4746220.atspace.cc", INTERNET_DEFAULT_HTTP_PORT, 0);
	HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/feedback.php", 0, 0, 0, 0);

	LPCWSTR headers = L"Content-Type: application/x-www-form-urlencoded\r\n";

	WinHttpSendRequest(hRequest, headers, -1L, (void*)postData.c_str(), postData.length(), postData.length(), 0);
	WinHttpReceiveResponse(hRequest, NULL);

	WinHttpCloseHandle(hRequest);
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
}