#include "preprocessor.h"
#include <fstream>
#include <algorithm>

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
			processIfndef(code, end + 1, end, line, dir, defines, res);
			break;
		case DEFINE:
			processDefine(code, end + 1, end, line, defines, res);
			break;
		case ELSE:
		case ENDIF:
			end = start;
			return res;
		case NONE:
			insertLine(line, res);
			break;
		}
		start = end + 1;
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
	if (line.find("#else") != std::string::npos)
		return Directive::ELSE;
	if (line.find("#endif") != std::string::npos)
		return Directive::ENDIF;
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

	std::map<std::string, std::string> defines0;
	std::string ifndefBranch = preprocess(code, from, to, dir, defines0);
	
	std::string line0 = "";
	std::string elseBranch = "";
	std::map<std::string, std::string> defines1;
	if (fetchLine(code, to + 1, to, line0))
	{
		if (fetchDirective(line0) == Directive::ELSE)
		{
			elseBranch = preprocess(code, to + 1, to, dir, defines1);
			std::string tmpLine = "";
			fetchLine(code, to + 1, to, tmpLine);
		}
	}

	if (defines.find(macros) == defines.end())
	{
		defines.insert(defines0.begin(), defines0.end());
		res += ifndefBranch;
	}
	else
	{
		defines.insert(defines1.begin(), defines1.end());
		res += elseBranch;
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
	int defineStart = line.find("#define");
	int defineEnd = line.find(' ', defineStart);

	int macrosStart = line.find_first_not_of(' ', defineEnd);
	int macrosEnd = line.find_first_of(" (", macrosStart);
	if (line[macrosEnd] == '(')
		macrosEnd = line.find_first_of(')', macrosEnd) + 1;

	std::string macros = line.substr(macrosStart, macrosEnd - macrosStart);
	auto newEnd = std::remove(macros.begin(), macros.end(), ' ');
	int newLen = newEnd - macros.begin();
	macros.resize(newLen, ' ');

	std::string macrosValue = "";
	std::string tmpLine = line;
	int firstCh = macrosEnd;
	int lastCh = 0;
	bool lastIsBackSlash = false;
	do
	{
		lastCh = tmpLine.find_last_not_of(' ');
		lastIsBackSlash = tmpLine[lastCh] == '\\';
		
		lastCh = lastIsBackSlash ? lastCh : lastCh + 1;
		macrosValue += tmpLine.substr(firstCh, lastCh - firstCh);
		macrosValue += "\n";
		
		firstCh = 0;
		if (lastIsBackSlash)
		{
			fetchLine(code, from, to, tmpLine);
			from = to + 1;
		}
	} while (lastIsBackSlash);

	defines[macros] = macrosValue;
}

void insertLine(const std::string& line, std::string& code)
{

}
