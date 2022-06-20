#include "Visitors.h"

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
