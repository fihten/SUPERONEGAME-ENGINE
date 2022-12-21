#include "preprocessor.h"
#include <fstream>

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
	int from, int& to,
	const std::string& dir,
	std::map<std::string, std::string>& defines
)
{
	std::string res = "";
	std::string line = "";
	int start = from;
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
	return res;
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
	if (line.find("#include") != std::string::npos)
		return Directive::INCLUDE;
	if (line.find("#ifndef") != std::string::npos)
		return Directive::IFNDEF;
	if (line.find("#define") != std::string::npos)
		return Directive::DEFINE;
	return Directive::NONE;
}

void processInclude(
	const std::string& line,
	const std::string& dir,
	std::map<std::string, std::string>& defines,
	std::string& code
)
{
	int dirBegin = line.find('"') + 1;
	int dirEnd = line.find('"', dirBegin);
	std::string directory = line.substr(dirBegin, dirEnd - dirBegin);
	directory = dir + "\\" + directory;

	std::ifstream file(directory);
	file.seekg(0, file.end);
	int n = (int)file.tellg();
	file.seekg(0, file.beg);

	char* szIncludedCode = new char[n + 1];
	szIncludedCode[n] = 0;
	file.read(szIncludedCode, n);

	std::string sIncludedCode(szIncludedCode);
	int to = 0;
	sIncludedCode = preprocess(sIncludedCode, 0, to, dir, defines);
	
	code += sIncludedCode;
	
	delete[] szIncludedCode;
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
	int ifndefStart = line.find("#ifndef", 0);
	int ifndefEnd = line.find(' ', ifndefStart + 1);

	int macrosStart = line.find_first_not_of(' ', ifndefEnd);
	int macrosEnd = line.find(' ', macrosStart);
	std::string macros = line.substr(macrosStart, macrosEnd - macrosStart);

	std::string inserted = preprocess(code, from, to, dir, defines);
	if (defines.find(macros) == defines.end())
	{
		res += inserted;
	}
	else
	{

	}
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
