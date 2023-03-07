#pragma once
#include <string>

class ShaderConverter
{
public:
	virtual void convertWord(const std::string& word) = 0;
};