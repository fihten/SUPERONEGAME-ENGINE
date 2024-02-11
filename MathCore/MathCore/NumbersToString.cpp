#include "pch.h"
#include "NumbersToString.h"
#include <sstream>

std::string numbersToString(float numbers[], int count)
{
	std::ostringstream ss;
	for (int i = 0; i < count; ++i)
	{
		if (i == 0)
			ss << '{';
		ss << numbers[i] << ((i == count - 1) ? '}' : ',');
	}
	return ss.rdbuf()->str();
}