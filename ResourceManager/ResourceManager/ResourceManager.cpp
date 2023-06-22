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

ResourceManager::RegisterMessage ResourceManager::registerFloat4x4(const std::string& techniqueName, const std::string& flt4x4Name, ID3DX11EffectMatrixVariable* flt4x4, unsigned int elementsCount)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.float4x4s.count(flt4x4Name) != 0)
		return RegisterMessage::FLOAT4X4_ALREADY_EXISTS;

	techniqueRes.float4x4s[flt4x4Name].ptr = flt4x4;
	techniqueRes.float4x4s[flt4x4Name].elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat4(const std::string& techniqueName, const std::string& flt4Name, ID3DX11EffectVariable* flt4, unsigned int elementsCount)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.float4s.count(flt4Name) != 0)
		return RegisterMessage::FLOAT4_ALREADY_EXISTS;

	techniqueRes.float4s[flt4Name].ptr = flt4;
	techniqueRes.float4s[flt4Name].elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat3(const std::string& techniqueName, const std::string& flt3Name, ID3DX11EffectVariable* flt3, unsigned int elementsCount)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.float3s.count(flt3Name) != 0)
		return RegisterMessage::FLOAT3_ALREADY_EXISTS;

	techniqueRes.float3s[flt3Name].ptr = flt3;
	techniqueRes.float3s[flt3Name].elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat2(const std::string& techniqueName, const std::string& flt2Name, ID3DX11EffectVariable* flt2, unsigned int elementsCount)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.float2s.count(flt2Name) != 0)
		return RegisterMessage::FLOAT2_ALREADY_EXISTS;

	techniqueRes.float2s[flt2Name].ptr = flt2;
	techniqueRes.float2s[flt2Name].elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat1(const std::string& techniqueName, const std::string& flt1Name, ID3DX11EffectVariable* flt1, unsigned int elementsCount)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.float1s.count(flt1Name) != 0)
		return RegisterMessage::FLOAT1_ALREADY_EXISTS;

	techniqueRes.float1s[flt1Name].ptr = flt1;
	techniqueRes.float1s[flt1Name].elementsCount = elementsCount;

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

ResourceManager::RegisterMessage ResourceManager::registerTexture(const std::string& techniqueName, const std::string& textureName, const Texture2dResource& texRes)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.textures.count(textureName) != 0)
		return RegisterMessage::TEXTURE_ALREADY_EXISTS;

	techniqueRes.textures[textureName] = texRes;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerTexturesArray(const std::string& techniqueName, const std::string& textureArrName, const Texture2dArrayResource& texArrRes)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.texturesArrays.count(textureArrName) != 0)
		return RegisterMessage::TEXTURES_ARRAY_ALREADY_EXISTS;

	techniqueRes.texturesArrays[textureArrName] = texArrRes;

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
	if (techniqueRes.float3s.count(varName) != 0)
	{
		techniqueRes.float3s[varName].location = varLocation;
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

ResourceManager::RegisterMessage ResourceManager::registerImage(const std::string& name, ID3D11ShaderResourceView* image)
{
	if (imgs.count(name))
		return RegisterMessage::IMAGE_ALREADY_EXISTS;

	imgs[name] = image;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerImagesArray(const std::string& name, ID3D11ShaderResourceView* imagesArray)
{
	if (imgsArrs.count(name))
		return RegisterMessage::IMAGES_ARRAY_ALREADY_EXISTS;

	imgsArrs[name] = imagesArray;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPresenceOfGeometryShader(const std::string& techniqueName, const std::string& passName)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	pass.isThereAGeometryShaderHere = true;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPrimitiveType(const std::string& techniqueName, const std::string& passName, PassResource::PrimitiveType primType)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	if (!pass.isThereAGeometryShaderHere)
		return RegisterMessage::THERE_IS_NO_GEOMETRY_SHADER_IN_THE_PASS;

	pass.primType = primType;

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
	if (techniqueRes.float3s.count(variable) != 0)
		return techniqueRes.float3s.at(variable).location;

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

void ResourceManager::getFloat4s(const std::string& techniqueName, std::map<std::string, Float4Resource>& flt4s)
{
	flt4s.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	flt4s = techniqueRes.float4s;
}

void ResourceManager::getFloat3s(const std::string& techniqueName, std::map<std::string, Float3Resource>& flt3s)
{
	flt3s.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	flt3s = techniqueRes.float3s;
}

void ResourceManager::getFloat2s(const std::string& techniqueName, std::map<std::string, Float2Resource>& flt2s)
{
	flt2s.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	flt2s = techniqueRes.float2s;
}

void ResourceManager::getFloat1s(const std::string& techniqueName, std::map<std::string, Float1Resource>& flt1s)
{
	flt1s.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	flt1s = techniqueRes.float1s;
}

void ResourceManager::getStructures(const std::string& techniqueName, std::map<std::string, StructResource>& structs)
{
	structs.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	structs = techniqueRes.structures;
}

void ResourceManager::getTextures(const std::string& techniqueName, std::map<std::string, Texture2dResource>& textures)
{
	textures.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	textures = techniqueRes.textures;
}

void ResourceManager::getTexturesArrays(const std::string& techniqueName, std::map<std::string, Texture2dArrayResource>& texturesArrays)
{
	texturesArrays.clear();
	if (techniques.count(techniqueName) == 0)
		return;

	TechniqueResource& techniqueRes = techniques.at(techniqueName);
	texturesArrays = techniqueRes.texturesArrays;
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

ID3D11ShaderResourceView* ResourceManager::getImage(const std::string& name)
{
	if (!imgs.count(name))
		return nullptr;
	return imgs[name];
}

ID3D11ShaderResourceView* ResourceManager::getImagesArray(const std::string& name)
{
	if (!imgsArrs.count(name))
		return nullptr;
	return imgsArrs[name];
}

bool ResourceManager::isThereAGeometryShaderInThePass(const std::string& techniqueName, const std::string& passName)
{
	if (techniques.count(techniqueName) == 0)
		return false;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return false;

	auto& pass = techniqueRes.passes[passName];

	return pass.isThereAGeometryShaderHere;
}

PassResource::PrimitiveType ResourceManager::getPrimitiveType(const std::string& techniqueName, const std::string& passName)
{
	if (techniques.count(techniqueName) == 0)
		return PassResource::NONE;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return PassResource::NONE;

	auto& pass = techniqueRes.passes[passName];

	return pass.primType;
}

ResourceManager* ResourceManager::instance()
{
	if (ptr == nullptr)
		ptr = new ResourceManager();
	return ptr;
}