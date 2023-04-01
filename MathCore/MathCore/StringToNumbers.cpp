#include "pch.h"
#include "StringToNumbers.h"

void stringToNumbers(const std::string& str, float numbers[], int maxCount)
{
	std::memset((void*)numbers, 0, maxCount * sizeof(float));

	int pos = str.find_first_of("{");
	if (pos == str.npos)
		return;

	pos += 1;
	
	int index = 0;
	pos = str.find_first_not_of(" ", pos);
	while (pos != str.npos && str[pos] != '}' && index < maxCount)
	{
		int startOfNumber = str.find_first_not_of(" ", pos);
		if (startOfNumber == str.npos)
			return;

		int endOfNumber = str.find_first_of(" ,}", startOfNumber);
		if (endOfNumber == str.npos)
			return;

		std::string numberStr = str.substr(startOfNumber, endOfNumber - startOfNumber);
		numbers[index++] = std::atof(numberStr.c_str());

		pos = endOfNumber;
		if(str[pos] == ' ')
			pos = str.find_first_of(",}", pos);
		if (str[pos] == ',')
			++pos;

		pos = str.find_first_not_of(" ", pos);
	}
}