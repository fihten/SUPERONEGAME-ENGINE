#pragma once

#include "ShaderConverter.h"
#include <windef.h>

class ShaderReader
{
public:
	void openShader(LPCTSTR shader_path);
	void parseShader(ShaderConverter& converter);
};