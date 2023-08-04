// StringManager.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "StringManager.h"
#include <cmath>

StringManager* StringManager::stringManager = nullptr;

StringManager::StringManager()
{
	hash = new char* [hashSize];
}

void StringManager::init()
{
	if (stringManager != nullptr)
		return;
	stringManager = new StringManager();
}

string_id StringManager::toStringId(const char* str)
{
	string_id strID = 0;

	int n = std::strlen(str);
	int u = sizeof(string_id);
	int n0 = n / u;
	string_id* str0 = (string_id*)str;
	for (int ci = 0; ci < n0; ci += u)
	{
		string_id strID0 = str0[ci];
		strID = (strID & (~strID0)) | ((~strID) & strID0);
	}

	string_id strID0 = 0;
	for (int ci = 0; ci < n % u; ci++)
	{
		char ch = *(str + n - u + ci);
		strID0 |= ((string_id)(ch) << ci);
	}
	strID = (strID & (~strID0)) | ((~strID) & strID0);

	strID %= stringManager->hashSize;

	if (stringManager->hash[strID] != nullptr)
	{
		if (std::strcmp(stringManager->hash[strID], str) == 0)
			return strID;
		else
			return string_id(-1);
	}

	stringManager->hash[strID] = new char[n + 1];
	std::memcpy(stringManager->hash[strID], str, n);
	stringManager->hash[strID][n] = 0;

	return strID;
}

string_id StringManager::toStringId(const std::string& str)
{
	return toStringId(str.c_str());
}