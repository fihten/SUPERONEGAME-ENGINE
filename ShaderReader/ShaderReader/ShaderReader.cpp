#include "pch.h"
#include "framework.h"

#include <map>
#include <fstream>

#include "ShaderReader.h"
#include "preprocessor.h"
#include "StringUtils.h"

void ShaderReader::openShader(LPCTSTR shader_path)
{
	// interpret shader
	std::ifstream shaderFile(shader_path);

	shaderFile.seekg(0, shaderFile.end);
	int length = shaderFile.tellg();
	shaderFile.seekg(0, shaderFile.beg);

	char* shaderText = new char[length + 1];
	shaderFile.read(shaderText, length);
	shaderText[length] = 0;

	int to = 0;

	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SHADERS", shadersFolder, sz);

	std::map<std::string, std::string> defines;

	text = preprocess(shaderText, 0, to, shadersFolder, defines);
}

void ShaderReader::parseShader(ShaderConverter& converter)
{
	int currentIndex = 0;
	std::string word = "";
	while (readWord(
		text,
		std::string(skipSymbols),
		std::string(stopSymbols),
		currentIndex,
		word
	))
	{
		converter.convertWord(word);
	}
}