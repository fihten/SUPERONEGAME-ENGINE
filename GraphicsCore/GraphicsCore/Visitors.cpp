#include "Visitors.h"

/*________________________________ShadersNamesVisitor________________________________*/

void ShadersNamesVisitor::startVisit(const TECHNIQUE11* pTECHNIQUE11)
{
	shadersNames.push_back(ShadersNames());
	shadersNames.back().technique = pTECHNIQUE11->getName();
}


void ShadersNamesVisitor::startVisit(const PASS* pPASS)
{
	shadersNames.back().passes.push_back(pPASS->getName());
}

void ShadersNamesVisitor::startVisit(const VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION)
{
	SHADER_VERSION = true;
}

void ShadersNamesVisitor::startVisit(const FUNCTION_CALL* pFUNCTION_CALL)
{
	if (SHADER_VERSION)
	{
		shadersNames.back().shaders.push_back(pFUNCTION_CALL->getName());
		SHADER_VERSION = false;
	}
}

/*________________________________InputLayoutVisitor________________________________*/

void InputLayoutVisitor::startVisit(const FUNCTION_DECL* pFUNCTION_DECL)
{
	if (shaderName == pFUNCTION_DECL->getName())
		withinShaderDeclaration = true;
}

void InputLayoutVisitor::finishVisit(const FUNCTION_DECL* pFUNCTION_DECL)
{
	withinShaderDeclaration = false;
}

void InputLayoutVisitor::startVisit(const OUT_MODIFIER* pOUT)
{
	if (withinShaderDeclaration)
		inVariable = false;
}

void InputLayoutVisitor::startVisit(const FLOAT1* pFLOAT1)
{
	if (withinShaderDeclaration)
		format = "float";
}

void InputLayoutVisitor::startVisit(const FLOAT2* pFLOAT2)
{
	if (withinShaderDeclaration)
		format = "float2";
}

void InputLayoutVisitor::startVisit(const FLOAT3* pFLOAT3)
{
	if (withinShaderDeclaration)
		format = "float3";
}

void InputLayoutVisitor::startVisit(const FLOAT4* pFLOAT4)
{
	if (withinShaderDeclaration)
		format = "float4";
}