#pragma once

#include <string>

int skipSymbols(const char* str, const char* skippedSymbols, int currentIndex);

int nextStopSymbol(const char* str, const char* stopSymbols, int currentIndex);

std::string readWord(
	const std::string& str,
	const std::string& skippedSymbols,
	const std::string& stopSymbols,
	int& currentIndex
);

bool isNumber(const char* str);
float getNumber(const char* str);

bool isComment(const char* str);