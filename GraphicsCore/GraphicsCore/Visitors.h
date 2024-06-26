#pragma once

#include "Mesh.h"
#include "ShaderVisitor.h"
#include "ResourceManager.h"
#include <vector>
#include <string>
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

class GeometryShaderInfoVisitor : public ShaderVisitor
{
	bool withinTechnique = false;
	bool withinPass = false;
	bool withinFunctionDeclaration = false;
	bool withinArgumentsList = false;
	bool withinVariableDeclaration = false;
	bool withinSetGeometryShader = false;
	bool withinMaxVertexCount = false;

	struct FunctionInfo
	{
		std::string functionName = "";
		PassResource::PrimitiveType primType;
	};
	std::vector<FunctionInfo> fns;

public:
	void startVisit(const ShaderUnits::SHADER* pShader);
	void startVisit(const ShaderUnits::FUNCTION_DECL* pFuncDecl);
	
	void startVisit(const ShaderUnits::ROUND_BRACKETS* pRoundBrackets);
	void finishVisit(const ShaderUnits::ROUND_BRACKETS* pRoundBrackets);
	
	void startVisit(const ShaderUnits::VARIABLE_DECL* pVariableDeclaration);
	void finishVisit(const ShaderUnits::VARIABLE_DECL* pVariableDeclaration);
	
	void startVisit(const ShaderUnits::POINT* pPoint);
	
	void startVisit(const ShaderUnits::TECHNIQUE11* pTechnique11);
	void finishVisit(const ShaderUnits::TECHNIQUE11* pTechnique11);

	void startVisit(const ShaderUnits::PASS* pPass);
	void finishVisit(const ShaderUnits::PASS* pPass);
	
	void startVisit(const ShaderUnits::SET_GEOMETRY_SHADER* pSetGeometryShader);
	void finishVisit(const ShaderUnits::SET_GEOMETRY_SHADER* pSetGeometryShader);
	
	void startVisit(const ShaderUnits::FUNCTION_CALL* pFunctionCall);

	void startVisit(const ShaderUnits::MAXVERTEXCOUNT* pMaxVertexCount);
	void finishVisit(const ShaderUnits::MAXVERTEXCOUNT* pMaxVertexCount);

public:
	std::string technique = "";
	std::string pass = "";

	bool geometryShaderIsPresented = false;
	PassResource::PrimitiveType primType = PassResource::NONE;
};

#define INPUT_ELEMENT_MAX_COUNT 32
class InputLayoutVisitor : public ShaderVisitor
{
	D3D11_INPUT_ELEMENT_DESC inputElements[INPUT_ELEMENT_MAX_COUNT];
	int inputElementsCount = 0;
	
	std::string shaderName = "";

	bool withinShaderDeclaration = false;
	bool withinArgumentsList = false;
	int withinStruct = 0;
	bool inVariable = false;

	std::string semanticName = "";
	std::string format = "";
	UINT alignedByteOffset = 0;

public:
	void setShaderName(const std::string& shaderName) { this->shaderName = shaderName; };
	
	void startVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL);

	void startVisit(const ShaderUnits::ROUND_BRACKETS* pARGUMENTS_LIST);
	void finishVisit(const ShaderUnits::ROUND_BRACKETS* pARGUMENTS_LIST);

	void startVisit(const ShaderUnits::OUT_MODIFIER* pOUT);

	void startVisit(const ShaderUnits::FLOAT1* pFLOAT1);
	void startVisit(const ShaderUnits::FLOAT2* pFLOAT2);
	void startVisit(const ShaderUnits::FLOAT3* pFLOAT3);
	void startVisit(const ShaderUnits::FLOAT4* pFLOAT4);

	void startVisit(const ShaderUnits::STRUCT* pStruct);
	void finishVisit(const ShaderUnits::STRUCT* pStruct);

	void startVisit(const ShaderUnits::SEMANTIC* pSEMANTIC);

	void startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);
	void finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);

	void startVisit(const ShaderUnits::SHADER* pSHADER);

public:
	ID3D11InputLayout* getInputLayout(ID3D11Device* device, const void* shaderByteCode, size_t byteCodeLength);
	void getStreamsInfo(std::vector<InputLayoutResource::StreamInfo>& streamsInfo);
};

#define ELEMENTS_OF_CBUFFER_MAX_COUNT 32

struct ID3DX11EffectVariable;

struct ElementOfCbuffer
{
	std::string type = "";
	std::string name = "";
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* v = nullptr;
};

class ElementsOfCbufferVisitor : public ShaderVisitor
{
	bool withinCbuffer = false;
	bool withinVariableDeclaration = false;
	bool withinStruct = false;
	bool withinSquareBrackets = false;
public:
	void startVisit(const ShaderUnits::SHADER* pSHADER);

	void startVisit(const ShaderUnits::CBUFFER* pCBUFFER);
	void finishVisit(const ShaderUnits::CBUFFER* pCBUFFER);

	void startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);
	void finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);

	void startVisit(const ShaderUnits::STRUCT* pSTRUCT);
	void finishVisit(const ShaderUnits::STRUCT* pSTRUCT);

	void startVisit(const ShaderUnits::FLOAT4X4* pFLOAT4X4);
	void startVisit(const ShaderUnits::FLOAT4* pFLOAT4);
	void startVisit(const ShaderUnits::FLOAT3* pFLOAT3);
	void startVisit(const ShaderUnits::FLOAT2* pFLOAT2);
	void startVisit(const ShaderUnits::FLOAT1* pFLOAT1);

	void startVisit(const ShaderUnits::SQUARE_BRACKETS* pSQUARE_BRACKETS);
	void finishVisit(const ShaderUnits::SQUARE_BRACKETS* pSQUARE_BRACKETS);

	void startVisit(const ShaderUnits::NUMBER* pNUMBER);

	void getElements(ElementOfCbuffer*& pElements, int& count);

private:
	ElementOfCbuffer elements[ELEMENTS_OF_CBUFFER_MAX_COUNT];
	int elementsCount = 0;
};

class IsFileAShader :public ShaderVisitor
{
	void startVisit(const ShaderUnits::SHADER* pSHADER);
	void startVisit(const ShaderUnits::TECHNIQUE11* pTECHNIQUE11);

	bool f = false;
public:
	bool fileIsAShader() { return f; }
};

class StructVisitor :public ShaderVisitor
{
	bool structAlreadyVisited = false;
	bool withinStruct = false;
	bool withinVariableDeclaration = false;
	unsigned int fieldOffset = 0;
public:
	std::string structName = "";
	StructResource structInfo;

public:
	void startVisit(const ShaderUnits::SHADER* pSHADER);

	void startVisit(const ShaderUnits::STRUCT* pSTRUCT);
	void finishVisit(const ShaderUnits::STRUCT* pSTRUCT);

	void startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);
	void finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);

	void startVisit(const ShaderUnits::FLOAT1* pFLOAT1);
	void startVisit(const ShaderUnits::FLOAT2* pFLOAT2);
	void startVisit(const ShaderUnits::FLOAT3* pFLOAT3);
	void startVisit(const ShaderUnits::FLOAT4* pFLOAT4);
};

#define GLOBAL_VARIABLES_MAX_COUNT 64

struct GLOBAL_VARIABLE
{
	std::string name = "";
	std::string type = "";
};

class GlobalVariablesVisitor :public ShaderVisitor
{
	bool withinCbuffer = false;
	bool withinFunctionDeclaration = false;
	int withinStruct = 0;

	bool withinVariableDeclaration = false;

public:
	void startVisit(const ShaderUnits::CBUFFER* pCBUFFER);
	void finishVisit(const ShaderUnits::CBUFFER* pCBUFFER);

	void startVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL);
	void finishVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL);

	void startVisit(const ShaderUnits::STRUCT* pSTRUCT);
	void finishVisit(const ShaderUnits::STRUCT* pSTRUCT);

	void startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);
	void finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL);

	void startVisit(const ShaderUnits::SHADER* pSHADER);

	void startVisit(const ShaderUnits::TEXTURE2D* pTEXTURE2D);
	void startVisit(const ShaderUnits::TEXTURE2DARRAY* pTEXTURE2DARRAY);

	void startVisit(const ShaderUnits::INT1* pINT1);
	void startVisit(const ShaderUnits::INT2* pINT2);

	void startVisit(const ShaderUnits::FLOAT1* pFLOAT1);

public:
	GLOBAL_VARIABLE globalVariables[GLOBAL_VARIABLES_MAX_COUNT];
	int globalVariablesCount = 0;
};