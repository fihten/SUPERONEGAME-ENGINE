#include "Visitors.h"
#include <sstream>

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

/*________________________________InputLayoutVisitor________________________________*/

void InputLayoutVisitor::startVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL)
{
	if (shaderName == pFUNCTION_DECL->getName())
		withinShaderDeclaration = true;
}

void InputLayoutVisitor::finishVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL)
{
	withinShaderDeclaration = false;
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
	if (withinStruct == 0)
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
		info.name = ss.str();

		switch (inputElements[i].Format)
		{
		case DXGI_FORMAT_R32_FLOAT:
			info.type = Mesh::FLT1;
			info.size = 4;
			break;
		case DXGI_FORMAT_R32G32_FLOAT:
			info.type = Mesh::FLT2;
			info.size = 8;
			break;
		case DXGI_FORMAT_R32G32B32_FLOAT:
			info.type = Mesh::FLT3;
			info.size = 12;
			break;
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			info.type = Mesh::FLT4;
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
	if (withinCbuffer && !withinStruct)
	{
		++elementsCount;
		withinVariableDeclaration = true;
		elements[elementsCount - 1].name = pVARIABLE_DECL->getName();
	}
}

void ElementsOfCbufferVisitor::finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	if (withinCbuffer)
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
	structInfo.name = structName;
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
		structInfo.fields[structInfo.fieldsCount].name = pVARIABLE_DECL->getName();
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
		structInfo.fields[structInfo.fieldsCount].type = "float";

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
		structInfo.fields[structInfo.fieldsCount].type = "float2";

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
		structInfo.fields[structInfo.fieldsCount].type = "float3";

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
		structInfo.fields[structInfo.fieldsCount].type = "float4";

		fieldOffset += 16;
		structInfo.fieldsCount += 1;
	}
}