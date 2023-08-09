#include "pch.h"
#include "framework.h"

#include "ResourceManager.h"

ResourceManager* ResourceManager::ptr = nullptr;

ResourceManager::RegisterMessage ResourceManager::registerTechnique(string_id techniqueName, const ID3DX11EffectTechnique* technique)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech != techniques.end())
		return RegisterMessage::TECHNIQUE_ALREADY_EXISTS;

	TechniqueResource& techniqueRes = techniques[techniqueName];
	techniqueRes.ptr = technique;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPass(string_id techniqueName, string_id passName, ID3DX11EffectPass* pass)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass != techniqueRes.passes.end())
		return RegisterMessage::PASS_ALREADY_EXISTS;

	auto& passResource = techniqueRes.passes[passName];
	passResource.ptr = pass;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerInputLayout(string_id techniqueName, string_id passName, ID3D11InputLayout* inputLayout)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = itPass->second;
	pass.inputLayout.ptr = inputLayout;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerStreamsInfo(string_id techniqueName, string_id passName, const std::vector<InputLayoutResource::StreamInfo>& streamsInfo)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = itPass->second;
	pass.inputLayout.streamsInfo = streamsInfo;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat4x4(string_id techniqueName, string_id flt4x4Name, ID3DX11EffectMatrixVariable* flt4x4, unsigned int elementsCount)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itFLT4X4 = techniqueRes.float4x4s.find(flt4x4Name);
	if (itFLT4X4 != techniqueRes.float4x4s.end())
		return RegisterMessage::FLOAT4X4_ALREADY_EXISTS;

	auto& flt4x4Res = techniqueRes.float4x4s[flt4x4Name];
	flt4x4Res.ptr = flt4x4;
	flt4x4Res.elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat4(string_id techniqueName, string_id flt4Name, ID3DX11EffectVariable* flt4, unsigned int elementsCount)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itFLT4 = techniqueRes.float4s.find(flt4Name);
	if (itFLT4 != techniqueRes.float4s.end())
		return RegisterMessage::FLOAT4_ALREADY_EXISTS;

	auto& flt4Res = techniqueRes.float4s[flt4Name];
	flt4Res.ptr = flt4;
	flt4Res.elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat3(string_id techniqueName, string_id flt3Name, ID3DX11EffectVariable* flt3, unsigned int elementsCount)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itFLT3 = techniqueRes.float3s.find(flt3Name);
	if (itFLT3 != techniqueRes.float3s.end())
		return RegisterMessage::FLOAT3_ALREADY_EXISTS;

	auto& flt3Res = techniqueRes.float3s[flt3Name];
	flt3Res.ptr = flt3;
	flt3Res.elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat2(string_id techniqueName, string_id flt2Name, ID3DX11EffectVariable* flt2, unsigned int elementsCount)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itFLT2 = techniqueRes.float2s.find(flt2Name);
	if (itFLT2 != techniqueRes.float2s.end())
		return RegisterMessage::FLOAT2_ALREADY_EXISTS;

	auto& flt2Res = techniqueRes.float2s[flt2Name];
	flt2Res.ptr = flt2;
	flt2Res.elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerFloat1(string_id techniqueName, string_id flt1Name, ID3DX11EffectVariable* flt1, unsigned int elementsCount)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itFLT1 = techniqueRes.float1s.find(flt1Name);
	if (itFLT1 != techniqueRes.float1s.end())
		return RegisterMessage::FLOAT1_ALREADY_EXISTS;

	auto& flt1Res = techniqueRes.float1s[flt1Name];
	flt1Res.ptr = flt1;
	flt1Res.elementsCount = elementsCount;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerStruct(string_id techniqueName, string_id structName, const StructResource& structRes)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itStruct = techniqueRes.structures.find(structName);
	if (itStruct != techniqueRes.structures.end())
		return RegisterMessage::STRUCT_ALREADY_EXISTS;

	techniqueRes.structures[structName] = structRes;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerTexture(string_id techniqueName, string_id textureName, const Texture2dResource& texRes)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itTex = techniqueRes.textures.find(textureName);
	if (itTex != techniqueRes.textures.end())
		return RegisterMessage::TEXTURE_ALREADY_EXISTS;

	techniqueRes.textures[textureName] = texRes;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerTexturesArray(string_id techniqueName, string_id textureArrName, const Texture2dArrayResource& texArrRes)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itTexArr = techniqueRes.texturesArrays.find(textureArrName);
	if (itTexArr != techniqueRes.texturesArrays.end())
		return RegisterMessage::TEXTURES_ARRAY_ALREADY_EXISTS;

	techniqueRes.texturesArrays[textureArrName] = texArrRes;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerVariableLocation(string_id techniqueName, string_id varName, const VariableLocation& varLocation)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itFLT4X4 = techniqueRes.float4x4s.find(varName);
	if (itFLT4X4 != techniqueRes.float4x4s.end())
	{
		itFLT4X4->second.location = varLocation;
		return RegisterMessage::OK;
	}
	const auto& itFLT3 = techniqueRes.float3s.find(varName);
	if (itFLT3!= techniqueRes.float3s.end())
	{
		itFLT3->second.location = varLocation;
		return RegisterMessage::OK;
	}

	return RegisterMessage::VARIABLE_DOESNT_EXIST;
}

ResourceManager::RegisterMessage ResourceManager::registerVertexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, ID3D11Buffer* vertexBuffer, bool structured)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = itPass->second;
	auto* vertexBuffers = &pass.vertexBuffers;
	if (structured)
		vertexBuffers = &pass.vertexBuffersStructured;

	const auto& itVB = vertexBuffers->find(meshId);
	if (itVB != vertexBuffers->end())
	{
		itVB->second->Release();
		itVB->second = vertexBuffer;
	}
	else
	{
		(*vertexBuffers)[meshId] = vertexBuffer;
	}

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerIndexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, ID3D11Buffer* indexBuffer, bool structured)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = itPass->second;
	auto* indexBuffers = &pass.indexBuffers;
	if(structured)
		indexBuffers = &pass.indexBuffersStructured;

	const auto& itIB = indexBuffers->find(meshId);
	if (itIB != indexBuffers->end())
	{
		itIB->second->Release();
		itIB->second = indexBuffer;
	}
	else
	{
		(*indexBuffers)[meshId] = indexBuffer;
	}

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerImage(string_id name, ID3D11ShaderResourceView* image)
{
	const auto& it = imgs.find(name);
	if (it != imgs.end())
		return RegisterMessage::IMAGE_ALREADY_EXISTS;

	imgs[name] = image;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerImagesArray(string_id name, ID3D11ShaderResourceView* imagesArray)
{
	const auto& it = imgsArrs.find(name);
	if (it != imgsArrs.end())
		return RegisterMessage::IMAGES_ARRAY_ALREADY_EXISTS;

	imgsArrs[name] = imagesArray;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPresenceOfGeometryShader(string_id techniqueName, string_id passName)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = itPass->second;
	pass.isThereAGeometryShaderHere = true;

	return RegisterMessage::OK;
}

ResourceManager::RegisterMessage ResourceManager::registerPrimitiveType(string_id techniqueName, string_id passName, PassResource::PrimitiveType primType)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return RegisterMessage::TECHNIQUE_DOESNT_EXIST;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return RegisterMessage::PASS_DOESNT_EXIST;

	auto& pass = itPass->second;
	if (!pass.isThereAGeometryShaderHere)
		return RegisterMessage::THERE_IS_NO_GEOMETRY_SHADER_IN_THE_PASS;

	pass.primType = primType;

	return RegisterMessage::OK;
}

const std::vector<InputLayoutResource::StreamInfo>* ResourceManager::getStreamsInfo(string_id techniqueName, string_id passName) const
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return nullptr;

	const TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return nullptr;

	const PassResource& passRes = itPass->second;
	return &passRes.inputLayout.streamsInfo;
}

ID3D11InputLayout* ResourceManager::getInputLayout(string_id techniqueName, string_id passName) const
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return nullptr;

	const TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return nullptr;

	return itPass->second.inputLayout.ptr;
}

ID3DX11EffectPass* ResourceManager::getPass(string_id techniqueName, string_id passName) const
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return nullptr;

	const TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return nullptr;

	return itPass->second.ptr;
}

const VariableLocation& ResourceManager::getVariableLocation(string_id techniqueName, string_id variable) const
{
	static VariableLocation fictionLocation;

	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return fictionLocation;

	const TechniqueResource& techniqueRes = itTech->second;

	const auto& itFLT4X4 = techniqueRes.float4x4s.find(variable);
	if (itFLT4X4 != techniqueRes.float4x4s.end())
		return itFLT4X4->second.location;

	const auto& itFLT4 = techniqueRes.float4s.find(variable);
	if (itFLT4 != techniqueRes.float4s.end())
		return itFLT4->second.location;

	const auto& itFLT3 = techniqueRes.float3s.find(variable);
	if (itFLT3 != techniqueRes.float3s.end())
		return itFLT3->second.location;

	const auto& itFLT2 = techniqueRes.float2s.find(variable);
	if (itFLT2 != techniqueRes.float2s.end())
		return itFLT2->second.location;

	const auto& itFLT1 = techniqueRes.float1s.find(variable);
	if (itFLT1 != techniqueRes.float1s.end())
		return itFLT1->second.location;

	return fictionLocation;
}

std::map<string_id, Float4x4Resource>& ResourceManager::getFloat4x4s(string_id techniqueName)
{
	std::map<string_id, Float4x4Resource> empty;

	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return empty;

	TechniqueResource& techniqueRes = itTech->second;
	return techniqueRes.float4x4s;
}

std::map<string_id, Float4Resource>& ResourceManager::getFloat4s(string_id techniqueName)
{
	static std::map<string_id, Float4Resource> empty;
	
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return empty;

	TechniqueResource& techniqueRes = itTech->second;
	return techniqueRes.float4s;
}

std::map<string_id, Float3Resource>& ResourceManager::getFloat3s(string_id techniqueName)
{
	static std::map<string_id, Float3Resource> empty;
	
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return empty;

	TechniqueResource& techniqueRes = itTech->second;
	return techniqueRes.float3s;
}

std::map<string_id, Float2Resource>& ResourceManager::getFloat2s(string_id techniqueName)
{
	static std::map<string_id, Float2Resource> empty;
	
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return empty;

	TechniqueResource& techniqueRes = itTech->second;
	return techniqueRes.float2s;
}

std::map<string_id, Float1Resource>& ResourceManager::getFloat1s(string_id techniqueName)
{
	static std::map<string_id, Float1Resource> empty;
	
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return empty;

	TechniqueResource& techniqueRes = itTech->second;
	return techniqueRes.float1s;
}

std::map<string_id, StructResource>& ResourceManager::getStructures(string_id techniqueName)
{
	static std::map<string_id, StructResource> empty;
	
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return empty;

	TechniqueResource& techniqueRes = itTech->second;
	return techniqueRes.structures;
}

std::map<string_id, Texture2dResource>& ResourceManager::getTextures(string_id techniqueName)
{
	static std::map<string_id, Texture2dResource> empty;

	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return empty;

	TechniqueResource& techniqueRes = itTech->second;
	return techniqueRes.textures;
}

std::map<string_id, Texture2dArrayResource>& ResourceManager::getTexturesArrays(string_id techniqueName)
{
	static std::map<string_id, Texture2dArrayResource> empty;

	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return empty;

	TechniqueResource& techniqueRes = itTech->second;
	return techniqueRes.texturesArrays;
}

ID3D11Buffer* ResourceManager::getVertexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, bool structured)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return nullptr;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return nullptr;

	auto& pass = itPass->second;
	auto* vertexBuffers = &pass.vertexBuffers;
	if (structured)
		vertexBuffers = &pass.vertexBuffersStructured;

	const auto& itVB = vertexBuffers->find(meshId);
	if (itVB == vertexBuffers->end())
		return nullptr;

	return itVB->second;
}

ID3D11Buffer* ResourceManager::getIndexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, bool structured)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return nullptr;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return nullptr;

	auto& pass = itPass->second;
	auto* indexBuffers = &pass.indexBuffers;
	if(structured)
		indexBuffers = &pass.indexBuffersStructured;

	const auto& itIB = indexBuffers->find(meshId);
	if (itIB == indexBuffers->end())
		return nullptr;

	return itIB->second;
}

ID3D11ShaderResourceView* ResourceManager::getImage(string_id name)
{
	const auto& it = imgs.find(name);
	if (it == imgs.end())
		return nullptr;
	return it->second;
}

ID3D11ShaderResourceView* ResourceManager::getImagesArray(string_id name)
{
	const auto& it = imgsArrs.find(name);
	if (it == imgsArrs.end())
		return nullptr;
	return it->second;
}

bool ResourceManager::isThereAGeometryShaderInThePass(string_id techniqueName, string_id passName)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return false;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return false;

	auto& pass = itPass->second;

	return pass.isThereAGeometryShaderHere;
}

PassResource::PrimitiveType ResourceManager::getPrimitiveType(string_id techniqueName, string_id passName)
{
	const auto& itTech = techniques.find(techniqueName);
	if (itTech == techniques.end())
		return PassResource::NONE;

	TechniqueResource& techniqueRes = itTech->second;

	const auto& itPass = techniqueRes.passes.find(passName);
	if (itPass == techniqueRes.passes.end())
		return PassResource::NONE;

	auto& pass = itPass->second;

	return pass.primType;
}

ResourceManager* ResourceManager::instance()
{
	if (ptr == nullptr)
		ptr = new ResourceManager();
	return ptr;
}