#pragma once

#include "Mesh.h"
#include "ShaderVisitor.h"
#include "ResourceManager.h"
#include <vector>
#include <D3D11.h>

class ShadersNamesVisitor :public ShaderVisitor
{
public:
	struct ShadersNames
	{
		std::string technique = "";
		std::vector<std::string> passes;
		std::vector<std::string> shaders;
	};

private:
	std::vector<ShadersNames> shadersNames;
	bool SHADER_VERSION = false;

public:
	void startVisit(const ShaderUnits::TECHNIQUE11* pTECHNIQUE11);
	void startVisit(const ShaderUnits::PASS* pPASS);
	void startVisit(const ShaderUnits::VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION);
	void startVisit(const ShaderUnits::FUNCTION_CALL* pFUNCTION_CALL);

public:
	std::vector<ShadersNames>&& getShadersNames() { return std::move(shadersNames); };
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
	
	void startVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL);
	void finishVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL);

	void startVisit(const ShaderUnits::OUT_MODIFIER* pOUT);

	void startVisit(const ShaderUnits::FLOAT1* pFLOAT1);
	void startVisit(const ShaderUnits::FLOAT2* pFLOAT2);
	void startVisit(const ShaderUnits::FLOAT3* pFLOAT3);
	void startVisit(const ShaderUnits::FLOAT4* pFLOAT4);

	void startVisit(const ShaderUnits::SEMANTIC* pSEMANTIC);

	void startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);
	void finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);

	void startVisit(const ShaderUnits::SHADER* pSHADER);

public:
	ID3D11InputLayout* getInputLayout(ID3D11Device* device, const void* shaderByteCode, size_t byteCodeLength);
	std::vector<ResourceManager::InputLayoutStreamInfo>&& getStreamsInfo();
};

#define ELEMENTS_OF_CBUFFER_MAX_COUNT 32

struct ID3DX11EffectVariable;

struct ElementOfCbuffer
{
	std::string type = "";
	std::string name = "";
	ID3DX11EffectVariable* v = nullptr;
};

class ElementsOfCbufferVisitor : public ShaderVisitor
{
	bool withinCbuffer = false;
	bool withinVariableDeclaration = false;
public:
	void startVisit(const ShaderUnits::SHADER* pSHADER);

	void startVisit(const ShaderUnits::CBUFFER* pCBUFFER);
	void finishVisit(const ShaderUnits::CBUFFER* pCBUFFER);

	void startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);
	void finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);

	void startVisit(const ShaderUnits::FLOAT4X4* pFLOAT4X4);

	void getElements(ElementOfCbuffer*& pElements, int& count);

private:
	ElementOfCbuffer elements[ELEMENTS_OF_CBUFFER_MAX_COUNT];
	int elementsCount = 0;
};