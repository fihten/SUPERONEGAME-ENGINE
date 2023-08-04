#pragma once
#include <string>

typedef unsigned int string_id;

struct StringManager
{
	static void init();

	static string_id toStringId(const char* str);
	static string_id toStringId(const std::string& str);
private:
	StringManager();
	
	static StringManager* stringManager;
	char** hash = nullptr;
	const int hashSize = 1048576;
};