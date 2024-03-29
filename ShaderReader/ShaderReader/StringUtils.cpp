#include "pch.h"
#include "StringUtils.h"
#include <sstream>

int skipSymbols(const char* str, const char* skippedSymbols, int currentIndex)
{
	if (skippedSymbols == 0)
		return currentIndex;

	int i = currentIndex;
	for (; str[i]; ++i)
	{
		bool skip = false;
		for (int j = 0; skippedSymbols[j]; ++j)
		{
			if (str[i] == skippedSymbols[j])
			{
				skip = true;
				break;
			}
		}

		if (skip)
			continue;

		return i;
	}

	return i;
}

int nextStopSymbol(const char* str, const char* stopSymbols, int currentIndex)
{
	int i = currentIndex;
	for (; str[i]; ++i)
	{
		for (int j = 0; stopSymbols[j]; ++j)
		{
			if (str[i] == stopSymbols[j])
				return i;
		}
	}

	return i;
}

bool readWord(
	const std::string& str,
	const std::string& skippedSymbols,
	const std::string& stopSymbols,
	int& currentIndex,
	std::string& word
)
{
	size_t from = skipSymbols(str.c_str(), skippedSymbols.c_str(), currentIndex);

	bool isComment = str[from] == '/' && str[from + 1] == '/';
	bool isAdditionAssign = str[from] == '+' && str[from + 1] == '=';
	bool isDividesAssign = str[from] == '/' && str[from + 1] == '=';
	bool isMultipliesAssign = str[from] == '*' && str[from + 1] == '=';
	bool isIncrement = str[from] == '+' && str[from + 1] == '+';
	size_t to = 0;
	if (isComment)
		to = nextStopSymbol(str.c_str(), "\n", from);
	else if (isDividesAssign || isMultipliesAssign || isAdditionAssign || isIncrement)
		to = from + 2;
	else
		to = nextStopSymbol(str.c_str(), stopSymbols.c_str(), from);

	currentIndex = to;
	word = str.substr(from, to - from);
	if (word == "" && str[currentIndex])
		word = str[currentIndex++];

	if (word == "")
		return false;

	return true;
}

bool isNumber(const char* str)
{
	std::istringstream iss(str);

	float f = 0.0f;
	iss >> f;
	if (iss.rdstate() & std::ios_base::failbit)
		return false;

	std::string s;
	iss >> s;
	if (s.length() != 0 && std::strcmp(s.c_str(), "f") != 0)
		return false;

	return true;
}

float getNumber(const char* str)
{
	std::istringstream iss(str);

	float f = 0.0f;
	iss >> f;

	return f;
}

bool isComment(const char* str)
{
	if (std::strlen(str) < 2)
		return false;
	if (str[0] != '/')
		return false;
	if (str[1] != '/')
		return false;
	return true;
}
