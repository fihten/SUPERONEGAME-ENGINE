#pragma once

#include "ShaderVisitor.h"
#include <vector>

struct VertexShader
{
	std::string technique = "";
	std::string pass = "";
	std::string vertexShader = "";

	bool isValid() { return technique != std::string("") && pass != std::string("") && vertexShader != std::string(""); };
};

class VertexShaderVisitor :public ShaderVisitor
{
	std::vector<VertexShader> vss;
public:
	void startVisit(const TECHNIQUE11* pTECHNIQUE11);
	void finishVisit(const TECHNIQUE11* pTECHNIQUE11);
};