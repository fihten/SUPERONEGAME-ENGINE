#pragma once
#include <string>
#include <map>

enum Directive
{
	INCLUDE,
	IFNDEF,
	DEFINE,
	NONE
};

std::string preprocess(
	const std::string& code,
	std::map<std::string, std::string>& defines
);
