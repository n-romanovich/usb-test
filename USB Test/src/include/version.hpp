#pragma once
#include <string>
#include <windows.h>
#include <winhttp.h>
#include <functional>

std::string readHttpResponse(HINTERNET hRequest);
bool IsValidVersion(const std::string& version);
bool isNewerVersion(const std::string& server, const std::string& client);
int checkUpdates();