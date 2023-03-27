#include "pch.h"
#include "NumbersToString.h"
#include <sstream>

std::string numbersToString(float numbers[], int count)
{
	std::ostringstream ss;
	for (int i = 0; i < count; ++i)
		ss << ((i == 0) ? '{' : ' ') << numbers[i] << ((i == count - 1) ? '}' : ',');
	return ss.rdbuf()->str();
}