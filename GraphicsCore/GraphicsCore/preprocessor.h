#pragma once
#include <string>
#include <map>

std::string preprocess(
	const std::string& code,
	std::map<std::string, std::string>& defines
);
