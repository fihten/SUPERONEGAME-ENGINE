#include "pch.h"
#include "framework.h"

#include "ResourceManager.h"

ResourceManager* ResourceManager::ptr = nullptr;

ResourceManager::RegisterMessage ResourceManager::registerTechnique(string_id techniqueName, const ID3DX11EffectTechnique* technique)
{
	if (techniques.count(techniqueName) != 0)
		return RegisterMessage::TECHNIQUE_ALREADY_EXISTS;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	techniqueRes.ptr = technique;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPass(string_id techniqueName, string_id passName, ID3DX11EffectPass* pass)
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

ResourceManager::RegisterMessage ResourceManager::registerInputLayout(string_id techniqueName, string_id passName, ID3D11InputLayout* inputLayout)
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

ResourceManager::RegisterMessage ResourceManager::registerStreamsInfo(string_id techniqueName, string_id passName, const std::vector<InputLayoutResource::StreamInfo>& streamsInfo)
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

ResourceManager::RegisterMessage ResourceManager::registerFloat4x4(string_id techniqueName, string_id flt4x4Name, ID3DX11EffectMatrixVariable* flt4x4, unsigned int elementsCount)
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

ResourceManager::RegisterMessage ResourceManager::registerFloat4(string_id techniqueName, string_id flt4Name, ID3DX11EffectVariable* flt4, unsigned int elementsCount)
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

ResourceManager::RegisterMessage ResourceManager::registerFloat3(string_id techniqueName, string_id flt3Name, ID3DX11EffectVariable* flt3, unsigned int elementsCount)
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

ResourceManager::RegisterMessage ResourceManager::registerFloat2(string_id techniqueName, string_id flt2Name, ID3DX11EffectVariable* flt2, unsigned int elementsCount)
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

ResourceManager::RegisterMessage ResourceManager::registerFloat1(string_id techniqueName, string_id flt1Name, ID3DX11EffectVariable* flt1, unsigned int elementsCount)
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

ResourceManager::RegisterMessage ResourceManager::registerStruct(string_id techniqueName, string_id structName, const StructResource& structRes)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.structures.count(structName) != 0)
		return RegisterMessage::STRUCT_ALREADY_EXISTS;

	techniqueRes.structures[structName] = structRes;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerTexture(string_id techniqueName, string_id textureName, const Texture2dResource& texRes)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.textures.count(textureName) != 0)
		return RegisterMessage::TEXTURE_ALREADY_EXISTS;

	techniqueRes.textures[textureName] = texRes;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerTexturesArray(string_id techniqueName, string_id textureArrName, const Texture2dArrayResource& texArrRes)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.texturesArrays.count(textureArrName) != 0)
		return RegisterMessage::TEXTURES_ARRAY_ALREADY_EXISTS;

	techniqueRes.texturesArrays[textureArrName] = texArrRes;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerVariableLocation(string_id techniqueName, string_id varName, const VariableLocation& varLocation)
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

ResourceManager::RegisterMessage ResourceManager::registerVertexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, ID3D11Buffer* vertexBuffer, bool structured)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	auto* vertexBuffers = &pass.vertexBuffers;
	if (structured)
		vertexBuffers = &pass.vertexBuffersStructured;
	if ((*vertexBuffers).count(meshId))
		(*vertexBuffers)[meshId]->Release();
	(*vertexBuffers)[meshId] = vertexBuffer;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerIndexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, ID3D11Buffer* indexBuffer, bool structured)
{
	if (techniques.count(techniqueName) == 0)
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	if (techniqueRes.passes.count(passName) == 0)
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = techniqueRes.passes[passName];
	auto* indexBuffers = &pass.indexBuffers;
	if(structured)
		indexBuffers = &pass.indexBuffersStructured;
	if ((*indexBuffers).count(meshId))
		(*indexBuffers)[meshId]->Release();
	(*indexBuffers)[meshId] = indexBuffer;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerImage(string_id name, ID3D11ShaderResourceView* image)
{
	if (imgs.count(name))
		return RegisterMessage::IMAGE_ALREADY_EXISTS;

	imgs[name] = image;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerImagesArray(string_id name, ID3D11ShaderResourceView* imagesArray)
{
	if (imgsArrs.count(name))
		return RegisterMessage::IMAGES_ARRAY_ALREADY_EXISTS;

	imgsArrs[name] = imagesArray;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPresenceOfGeometryShader(string_id techniqueName, string_id passName)
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

ResourceManager::RegisterMessage ResourceManager::registerPrimitiveType(string_id techniqueName, string_id passName, PassResource::PrimitiveType primType)
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

const std::vector<InputLayoutResource::StreamInfo>* ResourceManager::getStreamsInfo(string_id techniqueName, string_id passName) 
{
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return nullptr;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	if (passName != cashed_pass_name_id || techniqueCasheWasUpdated)
	{
		if (cashed_technique_resource->passes.count(passName) == 0)
			return nullptr;
		cashed_pass_resource = &cashed_technique_resource->passes.at(passName);
		cashed_pass_name_id = passName;
		techniqueCasheWasUpdated = false;
	}
	return &cashed_pass_resource->inputLayout.streamsInfo;
}

ID3D11InputLayout* ResourceManager::getInputLayout(string_id techniqueName, string_id passName)
{
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return nullptr;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	if (passName != cashed_pass_name_id || techniqueCasheWasUpdated)
	{
		if (cashed_technique_resource->passes.count(passName) == 0)
			return nullptr;
		cashed_pass_resource = &cashed_technique_resource->passes.at(passName);
		cashed_pass_name_id = passName;
		techniqueCasheWasUpdated = false;
	}

	return cashed_pass_resource->inputLayout.ptr;
}

ID3DX11EffectPass* ResourceManager::getPass(string_id techniqueName, string_id passName)
{
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return nullptr;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	if (passName != cashed_pass_name_id || techniqueCasheWasUpdated)
	{
		if (cashed_technique_resource->passes.count(passName) == 0)
			return nullptr;
		cashed_pass_resource = &cashed_technique_resource->passes.at(passName);
		cashed_pass_name_id = passName;
		techniqueCasheWasUpdated = false;
	}

	return cashed_pass_resource->ptr;
}

const VariableLocation& ResourceManager::getVariableLocation(string_id techniqueName, string_id variable)
{
	static VariableLocation fictionLocation;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return fictionLocation;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}
		
	if (cashed_technique_resource->float4x4s.count(variable) != 0)
		return cashed_technique_resource->float4x4s.at(variable).location;
	if (cashed_technique_resource->float4s.count(variable) != 0)
		return cashed_technique_resource->float4s.at(variable).location;
	if (cashed_technique_resource->float3s.count(variable) != 0)
		return cashed_technique_resource->float3s.at(variable).location;
	if (cashed_technique_resource->float2s.count(variable) != 0)
		return cashed_technique_resource->float2s.at(variable).location;
	if (cashed_technique_resource->float1s.count(variable) != 0)
		return cashed_technique_resource->float1s.at(variable).location;

	return fictionLocation;
}

std::map<string_id, Float4x4Resource>& ResourceManager::getFloat4x4s(string_id techniqueName)
{
	std::map<string_id, Float4x4Resource> empty;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return empty;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	return cashed_technique_resource->float4x4s;
}

std::map<string_id, Float4Resource>& ResourceManager::getFloat4s(string_id techniqueName)
{
	static std::map<string_id, Float4Resource> empty;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return empty;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	return cashed_technique_resource->float4s;
}

std::map<string_id, Float3Resource>& ResourceManager::getFloat3s(string_id techniqueName)
{
	static std::map<string_id, Float3Resource> empty;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return empty;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	return cashed_technique_resource->float3s;
}

std::map<string_id, Float2Resource>& ResourceManager::getFloat2s(string_id techniqueName)
{
	static std::map<string_id, Float2Resource> empty;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return empty;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	return cashed_technique_resource->float2s;
}

std::map<string_id, Float1Resource>& ResourceManager::getFloat1s(string_id techniqueName)
{
	static std::map<string_id, Float1Resource> empty;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return empty;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	return cashed_technique_resource->float1s;
}

std::map<string_id, StructResource>& ResourceManager::getStructures(string_id techniqueName)
{
	static std::map<string_id, StructResource> empty;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return empty;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	return cashed_technique_resource->structures;
}

std::map<string_id, Texture2dResource>& ResourceManager::getTextures(string_id techniqueName)
{
	static std::map<string_id, Texture2dResource> empty;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return empty;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	return cashed_technique_resource->textures;
}

std::map<string_id, Texture2dArrayResource>& ResourceManager::getTexturesArrays(string_id techniqueName)
{
	static std::map<string_id, Texture2dArrayResource> empty;
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return empty;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	return cashed_technique_resource->texturesArrays;
}

ID3D11Buffer* ResourceManager::getVertexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, bool structured)
{
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return nullptr;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	if (passName != cashed_pass_name_id || techniqueCasheWasUpdated)
	{
		if (cashed_technique_resource->passes.count(passName) == 0)
			return nullptr;
		cashed_pass_resource = &cashed_technique_resource->passes.at(passName);
		cashed_pass_name_id = passName;
		techniqueCasheWasUpdated = false;
	}

	auto* vertexBuffers = &cashed_pass_resource->vertexBuffers;
	if (structured)
		vertexBuffers = &cashed_pass_resource->vertexBuffersStructured;
	if ((*vertexBuffers).count(meshId) == 0)
		return nullptr;

	return (*vertexBuffers)[meshId];
}

ID3D11Buffer* ResourceManager::getIndexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, bool structured)
{
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return nullptr;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	if (passName != cashed_pass_name_id || techniqueCasheWasUpdated)
	{
		if (cashed_technique_resource->passes.count(passName) == 0)
			return nullptr;
		cashed_pass_resource = &cashed_technique_resource->passes.at(passName);
		cashed_pass_name_id = passName;
		techniqueCasheWasUpdated = false;
	}

	auto* indexBuffers = &cashed_pass_resource->indexBuffers;
	if(structured)
		indexBuffers = &cashed_pass_resource->indexBuffersStructured;
	if ((*indexBuffers).count(meshId) == 0)
		return nullptr;

	return (*indexBuffers)[meshId];
}

ID3D11ShaderResourceView* ResourceManager::getImage(string_id name)
{
	if (!imgs.count(name))
		return nullptr;
	return imgs[name];
}

ID3D11ShaderResourceView* ResourceManager::getImagesArray(string_id name)
{
	if (!imgsArrs.count(name))
		return nullptr;
	return imgsArrs[name];
}

bool ResourceManager::isThereAGeometryShaderInThePass(string_id techniqueName, string_id passName)
{
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return false;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	if (passName != cashed_pass_name_id || techniqueCasheWasUpdated)
	{
		if (cashed_technique_resource->passes.count(passName) == 0)
			return false;
		cashed_pass_resource = &cashed_technique_resource->passes.at(passName);
		cashed_pass_name_id = passName;
		techniqueCasheWasUpdated = false;
	}

	return cashed_pass_resource->isThereAGeometryShaderHere;
}

PassResource::PrimitiveType ResourceManager::getPrimitiveType(string_id techniqueName, string_id passName)
{
	if (techniqueName != cashed_technique_name_id)
	{
		if (techniques.count(techniqueName) == 0)
			return PassResource::NONE;

		cashed_technique_resource = &techniques.at(techniqueName);
		cashed_technique_name_id = techniqueName;
		techniqueCasheWasUpdated = true;
	}

	if (passName != cashed_pass_name_id || techniqueCasheWasUpdated)
	{
		if (cashed_technique_resource->passes.count(passName) == 0)
			return PassResource::NONE;
		cashed_pass_resource = &cashed_technique_resource->passes.at(passName);
		cashed_pass_name_id = passName;
		techniqueCasheWasUpdated = false;
	}

	return cashed_pass_resource->primType;
}

ResourceManager* ResourceManager::instance()
{
	if (ptr == nullptr)
		ptr = new ResourceManager();
	return ptr;
}