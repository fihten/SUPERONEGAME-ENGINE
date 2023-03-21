#include "pch.h"
#include "framework.h"

#include "ResourceManager.h"

ResourceManager* ResourceManager::ptr = nullptr;

ResourceManager::RegisterMessage ResourceManager::registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique)
{
	if (techniques.count(techniqueName) != 0)
		return RegisterMessage::TECHNIQUE_ALREADY_EXISTS;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	techniqueRes.ptr = technique;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPass(const std::string& techniqueName, const std::string& passName, ID3DX11EffectPass* pass)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) != 0)
		return RegisterMessage::PASS_ALREADY_EXISTS;

	auto& passResource = techniqueRes.passes[passName];
	passResource.ptr = pass;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerInputLayout(const std::string& techniqueName, const std::string& passName, ID3D11InputLayout* inputLayout)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	pass.inputLayout.ptr = inputLayout;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerStreamsInfo(const std::string& techniqueName, const std::string& passName, const std::vector<InputLayoutResource::StreamInfo>& streamsInfo)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	pass.inputLayout.streamsInfo = streamsInfo;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat4x4(const std::string& techniqueName, const std::string& flt4x4Name, ID3DX11EffectMatrixVariable* flt4x4)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.float4x4s.count(flt4x4Name) != 0)
		return RegisterMessage::FLOAT4X4_ALREADY_EXISTS;

	techniqueRes.float4x4s[flt4x4Name].ptr = flt4x4;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerStruct(const std::string& techniqueName, const std::string& structName, const StructResource& structRes)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.structures.count(structName) != 0)
		return RegisterMessage::STRUCT_ALREADY_EXISTS;

	techniqueRes.structures[structName] = structRes;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerVariableLocation(const std::string& techniqueName, const std::string& varName, const std::string& varLocation)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.float4x4s.count(varName) != 0)
	{
		techniqueRes.float4x4s[varName].location = varLocation;
		return RegisterMessage::OK;
	}

	return RegisterMessage::VARIABLE_DOESNT_EXIST;
}

ResourceManager::RegisterMessage ResourceManager::registerVertexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId, ID3D11Buffer* vertexBuffer)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	if (pass.vertexBuffers.count(meshId))
		pass.vertexBuffers[meshId]->Release();
	pass.vertexBuffers[meshId] = vertexBuffer;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerIndexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId, ID3D11Buffer* indexBuffer)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	if (pass.indexBuffers.count(meshId))
		pass.indexBuffers[meshId]->Release();
	pass.indexBuffers[meshId] = indexBuffer;

	return RegisterMessage::OK;
}

const std::vector<InputLayoutResource::StreamInfo>* ResourceManager::getStreamsInfo(const std::string& techniqueName, const std::string& passName) const
{
	if (techniques.count(techniqueName) == 0)
		return nullptr;

	const TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.passes.count(passName) == 0)
		return nullptr;

	const PassResource& passRes = techniqueRes.passes.at(passName);
	return &passRes.inputLayout.streamsInfo;
}

ID3D11InputLayout* ResourceManager::getInputLayout(const std::string& techniqueName, const std::string& passName) const
{
	if (techniques.count(techniqueName) == 0)
		return nullptr;

	const TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.passes.count(passName) == 0)
		return nullptr;

	return techniqueRes.passes.at(passName).inputLayout.ptr;
}

ID3DX11EffectPass* ResourceManager::getPass(const std::string& techniqueName, const std::string& passName) const
{
	if (techniques.count(techniqueName) == 0)
		return nullptr;

	const TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.passes.count(passName) == 0)
		return nullptr;

	return techniqueRes.passes.at(passName).ptr;
}

const std::string& ResourceManager::getVariableLocation(const std::string& techniqueName, const std::string& variable) const
{
	if (techniques.count(techniqueName) == 0)
		return "";

	const TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.float4x4s.count(variable) != 0)
		return techniqueRes.float4x4s.at(variable).location;

	return "";
}

void ResourceManager::getFloat4x4s(const std::string& techniqueName, std::map<std::string, Float4x4Resource>& flt4x4s)
{
	flt4x4s.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	flt4x4s = techniqueRes.float4x4s;
}

void ResourceManager::getStructures(const std::string& techniqueName, std::map<std::string, StructResource>& structs)
{
	structs.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	structs = techniqueRes.structures;
}

ID3D11Buffer* ResourceManager::getVertexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId)
{
	if (techniques.count(techniqueName) == 0)
		return nullptr;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return nullptr;

	auto& pass = techniqueRes.passes[passName];
	if (pass.vertexBuffers.count(meshId) == 0)
		return nullptr;

	return pass.vertexBuffers[meshId];
}

ID3D11Buffer* ResourceManager::getIndexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId)
{
	if (techniques.count(techniqueName) == 0)
		return nullptr;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return nullptr;

	auto& pass = techniqueRes.passes[passName];
	if (pass.indexBuffers.count(meshId) == 0)
		return nullptr;

	return pass.indexBuffers[meshId];
}

ResourceManager* ResourceManager::instance()
{
	if (ptr == nullptr)
		ptr = new ResourceManager();
	return ptr;
}