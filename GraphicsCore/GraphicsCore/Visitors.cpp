#include "Visitors.h"

void VertexShaderVisitor::startVisit(const TECHNIQUE11* pTECHNIQUE11)
{
	vss.push_back(VertexShader());
}

void VertexShaderVisitor::finishVisit(const TECHNIQUE11* pTECHNIQUE11)
{
	if (!vss.back().isValid())
		vss.pop_back();
}