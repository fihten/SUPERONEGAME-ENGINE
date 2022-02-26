#include "StringUtils.h"

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

std::string readWord(
	const std::string& str,
	const std::string& skippedSymbols,
	const std::string& stopSymbols,
	int& currentIndex
)
{
	size_t from = skipSymbols(str.c_str(), skippedSymbols.c_str(), currentIndex);
	size_t to = nextStopSymbol(str.c_str(), stopSymbols.c_str(), from);
	
	currentIndex = to;
	std::string word = str.substr(from, to - from);
	if (word == "" && str[currentIndex])
		word = str[currentIndex++];

	return word;
}