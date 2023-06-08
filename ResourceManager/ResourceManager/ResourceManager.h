#pragma once

#include <map>
#include <string>
#include "TechniqueResource.h"

class ResourceManager
{
	ResourceManager() {}
	static ResourceManager* ptr;

private:
	std::map<std::string, TechniqueResource> techniques;
	std::map<std::string, ID3D11ShaderResourceView*> imgs;
	std::map<std::string, ID3D11ShaderResourceView*> imgsArrs;

public:
	enum RegisterMessage
	{
		OK,
		TECHNIQUE_ALREADY_EXISTS,
		TECHNIQUE_DOESNT_EXIST,
		PASS_ALREADY_EXISTS,
		PASS_DOESNT_EXIST,
		FLOAT4X4_ALREADY_EXISTS,
		FLOAT3_ALREADY_EXISTS,
		VARIABLE_DOESNT_EXIST,
		STRUCT_ALREADY_EXISTS,
		TEXTURE_ALREADY_EXISTS,
		TEXTURES_ARRAY_ALREADY_EXISTS,
		IMAGE_ALREADY_EXISTS,
		IMAGES_ARRAY_ALREADY_EXISTS,
		THERE_IS_NO_GEOMETRY_SHADER_IN_THE_PASS
	};

	RegisterMessage registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique);
	RegisterMessage registerPass(const std::string& techniqueName, const std::string& passName, ID3DX11EffectPass* pass);
	RegisterMessage registerInputLayout(const std::string& techniqueName, const std::string& passName, ID3D11InputLayout* inputLayout);
	RegisterMessage registerStreamsInfo(const std::string& techniqueName, const std::string& passName, const std::vector<InputLayoutResource::StreamInfo>& streamsInfo);
	RegisterMessage registerFloat4x4(const std::string& techniqueName, const std::string& flt4x4Name, ID3DX11EffectMatrixVariable* flt4x4, unsigned int elementsCount);
	RegisterMessage registerFloat3(const std::string& techniqueName, const std::string& flt3Name, ID3DX11EffectVariable* flt3, unsigned int elementsCount);
	RegisterMessage registerStruct(const std::string& techniqueName, const std::string& structName, const StructResource& structRes);
	RegisterMessage registerTexture(const std::string& techniqueName, const std::string& textureName, const Texture2dResource& texRes);
	RegisterMessage registerTexturesArray(const std::string& techniqueName, const std::string& textureArrName, const Texture2dArrayResource& texArrRes);
	RegisterMessage registerVariableLocation(const std::string& techniqueName, const std::string& varName, const std::string& varLocation);
	RegisterMessage registerVertexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId, ID3D11Buffer* vertexBuffer);
	RegisterMessage registerIndexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId, ID3D11Buffer* indexBuffer);
	RegisterMessage registerImage(const std::string& name, ID3D11ShaderResourceView* image);
	RegisterMessage registerImagesArray(const std::string& name, ID3D11ShaderResourceView* imagesArray);
	RegisterMessage registerPresenceOfGeometryShader(const std::string& techniqueName, const std::string& passName);
	RegisterMessage registerPrimitiveType(const std::string& techniqueName, const std::string& passName, PassResource::PrimitiveType primType);

	const std::vector<InputLayoutResource::StreamInfo>* getStreamsInfo(const std::string& techniqueName, const std::string& passName) const;
	ID3D11InputLayout* getInputLayout(const std::string& techniqueName, const std::string& passName) const;
	ID3DX11EffectPass* getPass(const std::string& techniqueName, const std::string& passName) const;
	const std::string& getVariableLocation(const std::string& techniqueName, const std::string& variable) const;
	void getFloat4x4s(const std::string& techniqueName, std::map<std::string, Float4x4Resource>& flt4x4s);
	void getFloat3s(const std::string& techniqueName, std::map<std::string, Float3Resource>& flt3s);
	void getStructures(const std::string& techniqueName, std::map<std::string, StructResource>& structs);
	void getTextures(const std::string& techniqueName, std::map<std::string, Texture2dResource>& textures);
	void getTexturesArrays(const std::string& techniqueName, std::map<std::string, Texture2dArrayResource>& texturesArrays);
	ID3D11Buffer* getVertexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId);
	ID3D11Buffer* getIndexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId);
	ID3D11ShaderResourceView* getImage(const std::string& name);
	ID3D11ShaderResourceView* getImagesArray(const std::string& name);
	bool isThereAGeometryShaderInThePass(const std::string& techniqueName, const std::string& passName);
	PassResource::PrimitiveType getPrimitiveType(const std::string& techniqueName, const std::string& passName);

public:
	static ResourceManager* instance();
};
