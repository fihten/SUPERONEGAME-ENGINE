#pragma once

#include "ShaderVisitor.h"
#include <vector>
#include <D3D11.h>

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

#define INPUT_ELEMENT_MAX_COUNT 32
class InputLayoutVisitor : public ShaderVisitor
{
	D3D11_INPUT_ELEMENT_DESC inputElements[INPUT_ELEMENT_MAX_COUNT];
	int inputElementsCount = 0;
	
	std::string shaderName = "";

	bool withinShaderDeclaration = false;
	bool inVariable = true;

	std::string semanticName = "";
	std::string format = "";
	UINT alignedByteOffset = 0;

public:
	void setShaderName(const std::string& shaderName) { this->shaderName = shaderName; };
	
	void startVisit(const FUNCTION_DECL* pFUNCTION_DECL);
	void finishVisit(const FUNCTION_DECL* pFUNCTION_DECL);

	void startVisit(const OUT_MODIFIER* pOUT);

	void startVisit(const FLOAT1* pFLOAT1);
	void startVisit(const FLOAT2* pFLOAT2);
	void startVisit(const FLOAT3* pFLOAT3);
	void startVisit(const FLOAT4* pFLOAT4);

	void startVisit(const SEMANTIC* pSEMANTIC);
};