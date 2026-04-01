/*
WARNING: This module sends your local "USB Test" version to a remote server
         for version checking. Only the version string is transmitted.
         
         No personal data is collected!
*/


#include "include/version.hpp"
#include <windows.h>
#include <winhttp.h>
#include <string>
#include <iostream>
#include <regex>
#include <thread>
#include <functional>

#pragma comment(lib,"winhttp.lib")


const std::string CLIENT_VERSION = "1.1.1";     //Client's current version string


/*
* @brief Reads full HTTP response safely
* 
* @param HINTERNET hRequest: Handle to WinHTTP request
* 
* @return std::string containing response
*/
std::string readHttpResponse(HINTERNET hRequest) {
    std::string response;       //Stores full HTTP response
    DWORD dwSize = 0;           //Available data size
    char buffer[4096];          //Temporary buffer

    while (true) {
        if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) break;   //Query available data
        if (dwSize == 0) break;     //No more data

        DWORD bytesRead = 0;        //Bytes read this iteration
        if (!WinHttpReadData(hRequest, buffer, min(sizeof(buffer) - 1, dwSize), &bytesRead)) break;     //Read data
        if (bytesRead == 0) break;

        buffer[bytesRead] = '\0';       //Null-terminate
        response.append(buffer, bytesRead);     //Append to response
    }

    response.erase(response.find_last_not_of(" \r\n\t") + 1);       //Trim trailing whitespace
    return response;
}


/*
* @brief Validates version string format X.Y.Z
*
* @param const string version: Version string
*
* @return returns true/false
*/
bool IsValidVersion(const std::string& version) {
    return std::regex_match(version, std::regex(R"(^\d+\.\d+\.\d+$)"));     //Regex match X.Y.Z
}


/*
* @brief Compares server and client versions
*
* @param 
* const string server: Server version string
*
* const string client: Client version string
*
* @return true if server version is newer than client
*/
bool isNewerVersion(const std::string& server, const std::string& client) {
    int s1 = 0, s2 = 0, s3 = 0, c1 = 0, c2 = 0, c3 = 0;
    if (sscanf_s(server.c_str(), "%d.%d.%d", &s1, &s2, &s3) != 3) return false;     //Parse server
    if (sscanf_s(client.c_str(), "%d.%d.%d", &c1, &c2, &c3) != 3) return false;     //Parse client

    if (s1 != c1) return s1 > c1;   //Compare major
    if (s2 != c2) return s2 > c2;   //Compare minor
    return s3 > c3;                 //Compare patch
}

int checkUpdates() {
    const wchar_t* server = L"4746220.atspace.cc";      //Server hostname
    const wchar_t* urlPath = L"/version.php";       //Version check path

    HINTERNET hSession = WinHttpOpen(
        L"AppVersionCheck/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0
    ); //Open session
    if (!hSession) { return 1; }

    WinHttpSetTimeouts(hSession, 5000, 5000, 5000, 5000);    //Set connect/send/receive/resolve timeouts

    HINTERNET hConnect = WinHttpConnect(hSession, server, INTERNET_DEFAULT_HTTPS_PORT, 0);  //Connect HTTPS
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        return 1;
    }

    HINTERNET hRequest = WinHttpOpenRequest(
        hConnect,
        L"GET",                     //HTTP method
        urlPath,                    //URL path
        NULL,                       //HTTP version
        WINHTTP_NO_REFERER,          //Referer
        WINHTTP_DEFAULT_ACCEPT_TYPES,//Accept types
        WINHTTP_FLAG_SECURE          //Use HTTPS
    );

    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    //Set security flags to ignore invalid or self-signed certificates
    DWORD flags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
        SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
        SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;
    if (!WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &flags, sizeof(flags))) {
        std::cerr << "Failed to set security flags\n";
    }

    //Send request
    if (!WinHttpSendRequest(hRequest, NULL, 0, NULL, 0, 0, 0)) {
        WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
        return 1;
    }

    if (!WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
        return 1;
    }


    DWORD statusCode = 0;
    DWORD size = sizeof(statusCode);

    if (!WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, NULL, &statusCode, &size, NULL)) {
        WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
        return 1;
    }

    if (statusCode != 200) {
        WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
        return 1; 
    }

    std::string serverVersion = readHttpResponse(hRequest);     //Read server response

    if (!IsValidVersion(serverVersion)) {
        std::cerr << "Invalid server version: " << serverVersion << "\n";
        WinHttpCloseHandle(hRequest); WinHttpCloseHandle(hConnect); WinHttpCloseHandle(hSession);
        return 1;
    }

    if (serverVersion == CLIENT_VERSION) { 
    }
    else {
        std::cout << "New version is available! \n\nDownload it on: \nSourceForge: usb-test.sourceforge.io/ \nGitHub: github.com/n-romanovich/usb-test \n";
        system("pause");
    }

    //Clean up
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);

    return 0;
}