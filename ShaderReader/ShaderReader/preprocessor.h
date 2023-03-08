#pragma once

#include <string>
#include <map>

std::string preprocess(
	const std::string& code,
	int from, int& to,
	const std::string& dir,
	std::map<std::string, std::string>& defines
);