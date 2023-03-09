#include "pch.h"
#include "HLSLConverter.h"

HLSLConverter::HLSLConverter()
{
	words = std::queue<std::string>();
}

void HLSLConverter::convertWord(const std::string& word)
{
	words.push(word);
}