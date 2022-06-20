#pragma once

#include "ShaderVisitor.h"
#include <vector>

struct ShadersNames
{
	std::string technique = "";
	std::vector<std::string> passes;
	std::vector<std::string> shaders;
};

class ShadersNamesVisitor :public ShaderVisitor
{
	std::vector<ShadersNames> shadersNames;
	bool SHADER_VERSION = false;
public:
	void startVisit(const TECHNIQUE11* pTECHNIQUE11);
	void startVisit(const PASS* pPASS);
	void startVisit(const VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION);
	void startVisit(const FUNCTION_CALL* pFUNCTION_CALL);

};