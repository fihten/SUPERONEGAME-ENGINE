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

void InputLayoutVisitor::startVisit(const ShaderUnits::SEMANTIC* pSEMANTIC)
{
	if (withinShaderDeclaration)
		semanticName = pSEMANTIC->getName();
}

void InputLayoutVisitor::startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
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
	desc.SemanticName = semanticName.substr(0, pos).c_str();
	desc.SemanticIndex = std::atoi(semanticName.substr(pos).c_str());
	
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
	ID3D11InputLayout** inputLayout = nullptr;
	device->CreateInputLayout(inputElements, inputElementsCount, shaderByteCode, byteCodeLength, inputLayout);

	return *inputLayout;
}

std::vector<ResourceManager::InputLayoutStreamInfo>&& InputLayoutVisitor::getStreamsInfo()
{
	std::vector<ResourceManager::InputLayoutStreamInfo> streamsInfo(inputElementsCount);
	for (int i = 0; i < inputElementsCount; ++i)
	{
		auto& info = streamsInfo[i];

		std::ostringstream ss;
		ss << inputElements[i].SemanticName << inputElements[i].SemanticIndex;
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

	return std::move(streamsInfo);
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
	if (withinCbuffer)
	{
		++elementsCount;
		withinVariableDeclaration = true;
	}
}

void ElementsOfCbufferVisitor::finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
{
	if (withinCbuffer)
		withinVariableDeclaration = false;
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::FLOAT4X4* pFLOAT4X4)
{
	if (withinCbuffer && withinVariableDeclaration)
		elements[elementsCount - 1].type = "float4x4";
}

void ElementsOfCbufferVisitor::startVisit(const ShaderUnits::VARIABLE* pVARIABLE)
{
	if (withinCbuffer && withinVariableDeclaration)
		elements[elementsCount - 1].name = pVARIABLE->getName();
}

void ElementsOfCbufferVisitor::getElements(ElementOfCbuffer*& pElements, int& count)
{
	pElements = elements;
	count = elementsCount;
}