#pragma once

#include "ShaderComponent.h"
#include "ShaderVisitor.h"

class HLSLConverter;
class HLSLShader
{
	ShaderUnits::SHADER* shader = nullptr;

	friend HLSLConverter;
public:
	HLSLShader()
	{
		shader = nullptr;
	}
	~HLSLShader()
	{
		delete shader;
	}

	void query(ShaderVisitor* visitor)
	{
		shader->query(visitor);
	}
};