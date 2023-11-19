#include "Visitors.h"
#include <sstream>
#include <algorithm>

/*________________________________ShadersNamesVisitor________________________________*/

void ShadersNamesVisitor::startVisit(const ShaderUnits::TECHNIQUE11* pTECHNIQUE11)
{
	shadersNames.push_back(ShadersNames());
	shadersNames.back().technique = pTECHNIQUE11->getName();
}


void ShadersNamesVisitor::startVisit(const ShaderUnits::PASS* pPASS)
{
	shadersNames.back().passes.push_back(pPASS->getName());
}

void ShadersNamesVisitor::startVisit(const ShaderUnits::VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION)
{
	SHADER_VERSION = true;
}

void ShadersNamesVisitor::startVisit(const ShaderUnits::FUNCTION_CALL* pFUNCTION_CALL)
{
	if (SHADER_VERSION)
	{
		shadersNames.back().shaders.push_back(pFUNCTION_CALL->getName());
		SHADER_VERSION = false;
	}
}

/*________________________________GeometryShaderInfoVisitor________________________________*/

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::SHADER* pShader)
{
	withinTechnique = false;
	withinPass = false;
	withinFunctionDeclaration = false;
	withinArgumentsList = false;
	withinVariableDeclaration = false;
	withinSetGeometryShader = false;
	withinMaxVertexCount = false;
	geometryShaderIsPresented = false;
	primType = PassResource::NONE;
	fns.clear();
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::FUNCTION_DECL* pFuncDecl)
{
	withinFunctionDeclaration = true;
	fns.push_back(FunctionInfo());

	fns.back().functionName = pFuncDecl->getName();
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::ROUND_BRACKETS* pRoundBrackets)
{
	if (withinFunctionDeclaration && !withinMaxVertexCount)
		withinArgumentsList = true;
}

void GeometryShaderInfoVisitor::finishVisit(const ShaderUnits::ROUND_BRACKETS* pRoundBrackets)
{
	if (withinMaxVertexCount)
		return;
	withinArgumentsList = false;
	withinFunctionDeclaration = false;
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::VARIABLE_DECL* pVariableDeclaration)
{
	if (withinArgumentsList)
		withinVariableDeclaration = true;
}

void GeometryShaderInfoVisitor::finishVisit(const ShaderUnits::VARIABLE_DECL* pVariableDeclaration)
{
	withinVariableDeclaration = false;
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::POINT* pPoint)
{
	if (!withinVariableDeclaration)
		return;
	fns.back().primType = PassResource::POINT;
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::TECHNIQUE11* pTechnique11)
{
	if (pTechnique11->getName() == technique)
		withinTechnique = true;
}

void GeometryShaderInfoVisitor::finishVisit(const ShaderUnits::TECHNIQUE11* pTechnique11)
{
	withinTechnique = false;
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::PASS* pPass)
{
	if (!withinTechnique)
		return;
	if (pPass->getName() == pass)
		withinPass = true;
}

void GeometryShaderInfoVisitor::finishVisit(const ShaderUnits::PASS* pPass)
{
	withinPass = false;
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::SET_GEOMETRY_SHADER* pSetGeometryShader)
{
	if (!withinPass)
		return;
	withinSetGeometryShader = true;
}

void GeometryShaderInfoVisitor::finishVisit(const ShaderUnits::SET_GEOMETRY_SHADER* pSetGeometryShader)
{
	withinSetGeometryShader = false;
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::FUNCTION_CALL* pFunctionCall)
{
	if (!withinSetGeometryShader)
		return;

	auto it = std::find_if(fns.begin(), fns.end(), [&](const FunctionInfo& fi)->bool
	{
		return fi.functionName == pFunctionCall->getName();
	});

	if (it == fns.end())
		return;

	geometryShaderIsPresented = true;
	primType = it->primType;
}

void GeometryShaderInfoVisitor::startVisit(const ShaderUnits::MAXVERTEXCOUNT* pMaxVertexCount)
{
	withinMaxVertexCount = true;
}

void GeometryShaderInfoVisitor::finishVisit(const ShaderUnits::MAXVERTEXCOUNT* pMaxVertexCount)
{
	withinMaxVertexCount = false;
}

/*________________________________InputLayoutVisitor________________________________*/

void InputLayoutVisitor::startVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL)
{
	withinArgumentsList = false;
	if (shaderName == pFUNCTION_DECL->getName())
		withinShaderDeclaration = true;
}

void InputLayoutVisitor::startVisit(const ShaderUnits::ROUND_BRACKETS* pARGUMENTS_LIST)
{
	if (withinShaderDeclaration)
		withinArgumentsList = true;
}

void InputLayoutVisitor::finishVisit(const ShaderUnits::ROUND_BRACKETS* pARGUMENTS_LIST)
{
	if (withinShaderDeclaration)
	{
		withinArgumentsList = false;
		withinShaderDeclaration = false;
	}
}

void InputLayoutVisitor::startVisit(const ShaderUnits::OUT_MODIFIER* pOUT)
{
	if (withinShaderDeclaration)
		inVariable = false;
}

void InputLayoutVisitor::startVisit(const ShaderUnits::FLOAT1* pFLOAT1)
{
	if (withinShaderDeclaration)
		format = "float";
}

void InputLayoutVisitor::startVisit(const ShaderUnits::FLOAT2* pFLOAT2)
{
	if (withinShaderDeclaration)
		format = "float2";
}

void InputLayoutVisitor::startVisit(const ShaderUnits::FLOAT3* pFLOAT3)
{
	if (withinShaderDeclaration)
		format = "float3";
}

void InputLayoutVisitor::startVisit(const ShaderUnits::FLOAT4* pFLOAT4)
{
	if (withinShaderDeclaration)
		format = "float4";
}

void InputLayoutVisitor::startVisit(const ShaderUnits::STRUCT* pStruct)
{
	++withinStruct;
}

void InputLayoutVisitor::finishVisit(const ShaderUnits::STRUCT* pStruct)
{
	--withinStruct;
}

void InputLayoutVisitor::startVisit(const ShaderUnits::SEMANTIC* pSEMANTIC)
{
	if (withinShaderDeclaration)
		semanticName = pSEMANTIC->getName();
}

void InputLayoutVisitor::startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	if (withinStruct == 0 && withinArgumentsList)
		inVariable = true;
	semanticName = "";
	format = "";
}

void InputLayoutVisitor::finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	if (!withinShaderDeclaration)
		return;
	if (!inVariable)
		return;
	if (semanticName == std::string(""))
		return;
	if (inputElementsCount == INPUT_ELEMENT_MAX_COUNT)
		return;

	auto& desc = inputElements[inputElementsCount];

	size_t pos = semanticName.find_last_not_of("0123456789");
	
	char* buffer = new char[pos + 2];
	semanticName.copy(buffer, pos + 1, 0);
	buffer[pos + 1] = 0;
	desc.SemanticName = buffer;

	desc.SemanticIndex = std::atoi(semanticName.substr(pos + 1).c_str());
	semanticName = "";

	desc.AlignedByteOffset = alignedByteOffset;

	if (format == std::string("float"))
	{
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		alignedByteOffset += 4;
	}
	else if (format == std::string("float2"))
	{
		desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		alignedByteOffset += 8;
	}
	else if (format == std::string("float3"))
	{
		desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		alignedByteOffset += 12;
	}
	else if (format == std::string("float4"))
	{
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		alignedByteOffset += 16;
	}

	desc.InputSlot = 0;
	desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc.InstanceDataStepRate = 0;

	++inputElementsCount;
}

void InputLayoutVisitor::startVisit(const ShaderUnits::SHADER* pSHADER)
{
	inputElementsCount = 0;
}

ID3D11InputLayout* InputLayoutVisitor::getInputLayout(ID3D11Device* device, const void* shaderByteCode, size_t byteCodeLength)
{
	ID3D11InputLayout* inputLayout = nullptr;
	device->CreateInputLayout(inputElements, inputElementsCount, shaderByteCode, byteCodeLength, &inputLayout);

	return inputLayout;
}

void InputLayoutVisitor::getStreamsInfo(std::vector<InputLayoutResource::StreamInfo>& streamsInfo)
{
	streamsInfo.resize(inputElementsCount);
	for (int i = 0; i < inputElementsCount; ++i)
	{
		auto& info = streamsInfo[i];

		std::ostringstream ss;
		ss << inputElements[i].SemanticName;
		if (inputElements[i].SemanticIndex > 0)
			ss << inputElements[i].SemanticIndex;
		info.name = StringManager::toStringId(ss.str());

		switch (inputElements[i].Format)
		{
		case DXGI_FORMAT_R32_FLOAT:
			info.type = FLT1;
			info.size = 4;
			break;
		case DXGI_FORMAT_R32G32_FLOAT:
			info.type = FLT2;
			info.size = 8;
			break;
		case DXGI_FORMAT_R32G32B32_FLOAT:
			info.type = FLT3;
			info.size = 12;
			break;
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			info.type = FLT4;
			info.size = 16;
			break;
		default:
			break;
		}
	}
}

/*________________________________ElementsOfCbufferVisitor________________________________*/

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::SHADER* pSHADER)
{
	elementsCount = 0;
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::CBUFFER* pCBUFFER)
{
	withinCbuffer = true;
}

void ElementsOfCbufferVisitor::finishVisit(const ShaderUnits::CBUFFER* pCBUFFER)
{
	withinCbuffer = false;
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	std::string variableName = pVARIABLE_DECL->getName();
	if (variableName.find(" assigned") != std::string::npos)
		return;

	if (withinCbuffer && !withinStruct)
	{
		++elementsCount;
		withinVariableDeclaration = true;
		elements[elementsCount - 1].name = pVARIABLE_DECL->getName();
		elements[elementsCount - 1].elementsCount = 1;
	}
}

void ElementsOfCbufferVisitor::finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	if (withinCbuffer && !withinStruct)
		withinVariableDeclaration = false;
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::STRUCT* pSTRUCT)
{
	withinStruct = true;
	if (withinCbuffer && withinVariableDeclaration)
		elements[elementsCount - 1].type = std::string("struct ") + pSTRUCT->getName();
}

void ElementsOfCbufferVisitor::finishVisit(const ShaderUnits::STRUCT* pSTRUCT)
{
	withinStruct = false;
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::FLOAT4X4* pFLOAT4X4)
{
	if (withinCbuffer && withinVariableDeclaration && !withinStruct)
		elements[elementsCount - 1].type = "float4x4";
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::FLOAT4* pFLOAT4)
{
	if (withinCbuffer && withinVariableDeclaration && !withinStruct)
		elements[elementsCount - 1].type = "float4";
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::FLOAT3* pFLOAT3)
{
	if (withinCbuffer && withinVariableDeclaration && !withinStruct)
		elements[elementsCount - 1].type = "float3";
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::FLOAT2* pFLOAT2)
{
	if (withinCbuffer && withinVariableDeclaration && !withinStruct)
		elements[elementsCount - 1].type = "float2";
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::FLOAT1* pFLOAT1)
{
	if (withinCbuffer && withinVariableDeclaration && !withinStruct)
		elements[elementsCount - 1].type = "float";
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::SQUARE_BRACKETS* pSQUARE_BRACKETS)
{
	withinSquareBrackets = true;
}

void ElementsOfCbufferVisitor::finishVisit(const ShaderUnits::SQUARE_BRACKETS* pSQUARE_BRACKETS)
{
	withinSquareBrackets = false;
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::NUMBER* pNUMBER)
{
	if (withinCbuffer && withinVariableDeclaration && withinSquareBrackets && !withinStruct)
		elements[elementsCount - 1].elementsCount = std::atoi(pNUMBER->getName().c_str());
}

void ElementsOfCbufferVisitor::getElements(ElementOfCbuffer*& pElements, int& count)
{
	pElements = elements;
	count = elementsCount;
}

/*________________________________IsFileAShader________________________________*/

void IsFileAShader::startVisit(const ShaderUnits::SHADER* pSHADER)
{
	f = false;
}

void IsFileAShader::startVisit(const ShaderUnits::TECHNIQUE11* pTECHNIQUE11)
{
	f = true;
}

/*________________________________StructVisitor________________________________*/

void StructVisitor::startVisit(const ShaderUnits::SHADER* pSHADER)
{
	fieldOffset = 0;
	structAlreadyVisited = false;
	structInfo.fieldsCount = 0;
	structInfo.name = StringManager::toStringId(structName);
}

void StructVisitor::startVisit(const ShaderUnits::STRUCT* pSTRUCT)
{
	if (pSTRUCT->getName() == structName && !structAlreadyVisited)
		withinStruct = true;
}

void StructVisitor::finishVisit(const ShaderUnits::STRUCT* pSTRUCT)
{
	if (withinStruct)
	{
		structAlreadyVisited = true;
		structInfo.bytes = fieldOffset;
	}
	withinStruct = false;
}

void StructVisitor::startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	if (withinStruct)
	{
		withinVariableDeclaration = true;
		structInfo.fields[structInfo.fieldsCount].name =
			StringManager::toStringId(pVARIABLE_DECL->getName());
	}
}

void StructVisitor::finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	withinVariableDeclaration = false;
}

void StructVisitor::startVisit(const ShaderUnits::FLOAT1* pFLOAT1)
{
	if (withinStruct && withinVariableDeclaration)
	{
		structInfo.fields[structInfo.fieldsCount].bytes = 4;
		structInfo.fields[structInfo.fieldsCount].offset = fieldOffset;
		structInfo.fields[structInfo.fieldsCount].type =
			StringManager::toStringId("float");

		fieldOffset += 4;
		structInfo.fieldsCount += 1;
	}
}

void StructVisitor::startVisit(const ShaderUnits::FLOAT2* pFLOAT2)
{
	if (withinStruct && withinVariableDeclaration)
	{
		structInfo.fields[structInfo.fieldsCount].bytes = 8;
		structInfo.fields[structInfo.fieldsCount].offset = fieldOffset;
		structInfo.fields[structInfo.fieldsCount].type =
			StringManager::toStringId("float2");

		fieldOffset += 8;
		structInfo.fieldsCount += 1;
	}
}

void StructVisitor::startVisit(const ShaderUnits::FLOAT3* pFLOAT3)
{
	if (withinStruct && withinVariableDeclaration)
	{
		structInfo.fields[structInfo.fieldsCount].bytes = 12;
		structInfo.fields[structInfo.fieldsCount].offset = fieldOffset;
		structInfo.fields[structInfo.fieldsCount].type =
			StringManager::toStringId("float3");

		fieldOffset += 12;
		structInfo.fieldsCount += 1;
	}
}

void StructVisitor::startVisit(const ShaderUnits::FLOAT4* pFLOAT4)
{
	if (withinStruct && withinVariableDeclaration)
	{
		structInfo.fields[structInfo.fieldsCount].bytes = 16;
		structInfo.fields[structInfo.fieldsCount].offset = fieldOffset;
		structInfo.fields[structInfo.fieldsCount].type =
			StringManager::toStringId("float4");

		fieldOffset += 16;
		structInfo.fieldsCount += 1;
	}
}

/*________________________________GlobalVariablesVisitor________________________________*/

void GlobalVariablesVisitor::startVisit(const ShaderUnits::CBUFFER* pCBUFFER)
{
	withinCbuffer = true;
}

void GlobalVariablesVisitor::finishVisit(const ShaderUnits::CBUFFER* pCBUFFER)
{
	withinCbuffer = false;
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL)
{
	withinFunctionDeclaration = true;
}

void GlobalVariablesVisitor::finishVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL)
{
	withinFunctionDeclaration = false;
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::STRUCT* pSTRUCT)
{
	++withinStruct;
}

void GlobalVariablesVisitor::finishVisit(const ShaderUnits::STRUCT* pSTRUCT)
{
	--withinStruct;
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	if (withinCbuffer)
		return;
	if (withinFunctionDeclaration)
		return;
	if (withinStruct)
		return;
	withinVariableDeclaration = true;

	globalVariables[globalVariablesCount].name = pVARIABLE_DECL->getName();
}

void GlobalVariablesVisitor::finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	if (withinCbuffer)
		return;
	if (withinFunctionDeclaration)
		return;
	if (withinStruct)
		return;
	withinVariableDeclaration = false;
	++globalVariablesCount;
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::SHADER* pSHADER)
{
	globalVariablesCount = 0;
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::TEXTURE2D* pTEXTURE2D)
{
	if (withinCbuffer)
		return;
	if (withinFunctionDeclaration)
		return;
	if (withinStruct)
		return;
	if (!withinVariableDeclaration)
		return;

	globalVariables[globalVariablesCount].type = "Texture2D";
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::TEXTURE2DARRAY* pTEXTURE2DARRAY)
{
	if (withinCbuffer)
		return;
	if (withinFunctionDeclaration)
		return;
	if (withinStruct)
		return;
	if (!withinVariableDeclaration)
		return;

	globalVariables[globalVariablesCount].type = "Texture2DArray";
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::INT1* pINT1)
{
	if (withinCbuffer)
		return;
	if (withinFunctionDeclaration)
		return;
	if (withinStruct)
		return;
	if (!withinVariableDeclaration)
		return;

	globalVariables[globalVariablesCount].type = "int";
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::INT2* pINT2)
{
	if (withinCbuffer)
		return;
	if (withinFunctionDeclaration)
		return;
	if (withinStruct)
		return;
	if (!withinVariableDeclaration)
		return;

	globalVariables[globalVariablesCount].type = "int2";
}

void GlobalVariablesVisitor::startVisit(const ShaderUnits::FLOAT1* pFloat1)
{
	if (withinCbuffer)
		return;
	if (withinFunctionDeclaration)
		return;
	if (withinStruct)
		return;
	if (!withinVariableDeclaration)
		return;

	globalVariables[globalVariablesCount].type = "float";
}