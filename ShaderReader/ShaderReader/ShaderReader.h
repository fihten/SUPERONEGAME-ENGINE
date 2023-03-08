#pragma once

#include "ShaderConverter.h"
#include <windows.h>

class ShaderReader
{
	std::string text = "";

	const char skipSymbols[5] =
	{
		32, // space
		9,  // tab
		10, // line feed
		13, // carriage return
		0
	};

	const char stopSymbols[19] =
	{
			32, // space
			9,  // tab
			10, // line feed
			13, // carriage return
			'(',
			')',
			'{',
			'}',
			',',
			';',
			':',
			'+',
			'-',
			'*',
			'/',
			'=',
			'[',
			']',
			0
	};

public:
	void openShader(LPCTSTR shader_path);
	void parseShader(ShaderConverter& converter);
};