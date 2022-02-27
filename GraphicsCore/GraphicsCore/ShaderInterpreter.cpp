#include "ShaderInterpreter.h"
#include "StringUtils.h"

void ShaderBuilder::unknown()
{
	while (shaderText[currentIndex])
	{
		std::string word = readWord(
			shaderText,
			std::string(skipSymbols),
			std::string(stopSymbols),
			currentIndex
		);
	}
}