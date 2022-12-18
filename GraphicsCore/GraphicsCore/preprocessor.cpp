#include "preprocessor.h"

bool fetchLine(const std::string& code, int start, int& end, std::string& line);
Directive fetchDirective(const std::string& line);
void processInclude(
	const std::string& line,
	const std::string& dir,
	std::map<std::string, std::string>& defines,
	std::string& code
);
void processIfndef(
	const std::string& code,
	int from, int& to,
	const std::string& line,
	const std::string& dir,
	std::map<std::string, std::string>& defines,
	std::string& res
);
void processDefine(
	const std::string& code,
	int from, int& to,
	const std::string& line,
	std::map<std::string, std::string>& defines,
	std::string& res
);
void insertLine(const std::string& line, std::string& code);

std::string preprocess(
	const std::string& code,
	const std::string& dir,
	std::map<std::string, std::string>& defines
)
{
	std::string res = "";
	std::string line = "";
	int start = 0;
	int end = 0;
	// lines fetching
	while (fetchLine(res, start, end, line))
	{
		switch (fetchDirective(line))
		{
		case INCLUDE:
			processInclude(line, dir, defines, res);
			break;
		case IFNDEF:
			processIfndef(code, start, end, line, dir, defines, res);
			break;
		case DEFINE:
			processDefine(code, start, end, line, defines, res);
			break;
		case NONE:
			insertLine(line, res);
			break;
		}
	}
}

bool fetchLine(const std::string& code, int start, int& end, std::string& line)
{
	end = code.find('\n', start);
	if (end == std::string::npos)
		return false;
	line = code.substr(start, end - start);
	return true;
}

Directive fetchDirective(const std::string& line)
{

}

void processInclude(
	const std::string& line,
	const std::string& dir,
	std::map<std::string, std::string>& defines,
	std::string& code
)
{

}

void processIfndef(
	const std::string& code,
	int from, int& to,
	const std::string& line,
	const std::string& dir,
	std::map<std::string, std::string>& defines,
	std::string& res
)
{

}

void processDefine(
	const std::string& code,
	int from, int& to,
	const std::string& line,
	std::map<std::string, std::string>& defines,
	std::string& res
)
{

}

void insertLine(const std::string& line, std::string& code)
{

}
