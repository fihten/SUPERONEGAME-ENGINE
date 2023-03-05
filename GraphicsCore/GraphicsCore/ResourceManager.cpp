#include "ResourceManager.h"

ResourceManager resourceManager;

ResourceManager::RegisterMessage ResourceManager::registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique)
{
	if (techniques.count(techniqueName) != 0)
		return RegisterMessage::TECHNIQUE_ALREADY_EXISTS;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	techniqueRes.technique = technique;

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
	passResource.pass = pass;

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
	pass.inputLayout = inputLayout;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerStreamsInfo(const std::string& techniqueName, const std::string& passName, const std::vector<InputLayoutStreamInfo>& streamsInfo)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	pass.streamsInfo = streamsInfo;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerMatrix(const std::string& techniqueName, const std::string& matrixName, ID3DX11EffectMatrixVariable* matrix)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.matrices.count(matrixName) != 0)
		return RegisterMessage::MATRIX_ALREADY_EXISTS;

	techniqueRes.matrices[matrixName] = matrix;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerStruct(const std::string& techniqueName, const std::string& structName, const StructInfo& structInfo)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.structures.count(structName) != 0)
		return RegisterMessage::STRUCT_ALREADY_EXISTS;

	techniqueRes.structures[structName] = structInfo;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerVariableLocations(const std::string& techniqueName, const std::map<std::string, std::string>& locationOfVariable)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	techniqueRes.locationOfVariable = locationOfVariable;

	return RegisterMessage::OK;
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

const std::vector<ResourceManager::InputLayoutStreamInfo>* ResourceManager::getStreamsInfo(const std::string& techniqueName, const std::string& passName) const
{
	if (techniques.count(techniqueName) == 0)
		return nullptr;

	const TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.passes.count(passName) == 0)
		return nullptr;

	const TechniqueResource::PassResource& passRes = techniqueRes.passes.at(passName);
	return &passRes.streamsInfo;
}

ID3D11InputLayout* ResourceManager::getInputLayout(const std::string& techniqueName, const std::string& passName) const
{
	if (techniques.count(techniqueName) == 0)
		return nullptr;

	const TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.passes.count(passName) == 0)
		return nullptr;

	return techniqueRes.passes.at(passName).inputLayout;
}

ID3DX11EffectPass* ResourceManager::getPass(const std::string& techniqueName, const std::string& passName) const
{
	if (techniques.count(techniqueName) == 0)
		return nullptr;

	const TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.passes.count(passName) == 0)
		return nullptr;

	return techniqueRes.passes.at(passName).pass;
}

const std::string& ResourceManager::getVariableLocation(const std::string& techniqueName, const std::string& variable) const
{
	if (techniques.count(techniqueName) == 0)
		return "";

	const TechniqueResource& techniqueRes = techniques.at(techniqueName);
	if (techniqueRes.locationOfVariable.count(variable) == 0)
		return "";

	return techniqueRes.locationOfVariable.at(variable);
}

void ResourceManager::getMatrices(const std::string& techniqueName, std::map<std::string, ID3DX11EffectMatrixVariable*>& matrices)
{
	matrices.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	matrices = techniqueRes.matrices;
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
