#pragma once

#include "ShaderConverter.h"
#include <queue>

class HLSLConverter :public ShaderConverter
{
	std::queue<std::string> words;
public:
	HLSLConverter();

	void convertWord(const std::string& word);
};