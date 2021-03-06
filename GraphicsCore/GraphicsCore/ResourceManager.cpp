#include "ResourceManager.h"

ResourceManager::RegisterMessage ResourceManager::registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique)
{
	if (techniques.count(techniqueName) != 0)
		return RegisterMessage::TECHNIQUE_ALREADY_EXISTS;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	techniqueRes.technique = technique;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPass(const std::string& techniqueName, const std::string& passName, const ID3DX11EffectPass* pass)
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

ResourceManager::RegisterMessage ResourceManager::registerInputLayout(const std::string& techniqueName, const std::string& passName, const ID3D11InputLayout* inputLayout)
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

ResourceManager::RegisterMessage ResourceManager::registerMatrix(const std::string& techniqueName, const std::string& matrixName, const ID3D10EffectMatrixVariable* matrix)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.matrices.count(matrixName) != 0)
		return RegisterMessage::MATRIX_ALREADY_EXISTS;

	techniqueRes.matrices[matrixName] = matrix;

	return RegisterMessage::OK;
}