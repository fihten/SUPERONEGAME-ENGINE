#include "preprocessor.h"

bool fetchLine(const std::string& code, int start, int& end, std::string& line);
Directive fetchDirective(const std::string& line);
void processInclude(
	const std::string& line,
	const std::string& dir,
	std::map<std::string, std::string>& defines,
	std::string& code
);

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

			break;
		case DEFINE:

			break;
		case NONE:

			break;
		}
	}
}

bool fetchLine(const std::string& code, int start, int& end, std::string& line)
{

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