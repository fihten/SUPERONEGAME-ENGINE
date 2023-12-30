#pragma once

#include <map>
#include "TechniqueResource.h"
#include "StringManager.h"

class ResourceManager
{
	ResourceManager() {}
	static ResourceManager* ptr;

private:
	std::map<string_id, TechniqueResource> techniques;
	std::map<string_id, ID3D11ShaderResourceView*> imgs;
	std::map<string_id, ID3D11ShaderResourceView*> imgsArrs;
	std::map<string_id, ID3D11BlendState*> blendStates;
	std::map<string_id, ID3D11DepthStencilState*> depthStencilStates;

private:
	string_id cashed_technique_name_id = string_id(-1);
	string_id cashed_pass_name_id = string_id(-1);
	bool techniqueCasheWasUpdated = false;

	TechniqueResource* cashed_technique_resource = nullptr;
	PassResource* cashed_pass_resource = nullptr;

public:
	enum RegisterMessage
	{
		OK,
		TECHNIQUE_ALREADY_EXISTS,
		TECHNIQUE_DOESNT_EXIST,
		PASS_ALREADY_EXISTS,
		PASS_DOESNT_EXIST,
		FLOAT4X4_ALREADY_EXISTS,
		FLOAT4_ALREADY_EXISTS,
		FLOAT3_ALREADY_EXISTS,
		FLOAT2_ALREADY_EXISTS,
		FLOAT1_ALREADY_EXISTS,
		INT2_ALREADY_EXISTS,
		INT1_ALREADY_EXISTS,
		VARIABLE_DOESNT_EXIST,
		STRUCT_ALREADY_EXISTS,
		TEXTURE_ALREADY_EXISTS,
		TEXTURES_ARRAY_ALREADY_EXISTS,
		IMAGE_ALREADY_EXISTS,
		IMAGES_ARRAY_ALREADY_EXISTS,
		THERE_IS_NO_GEOMETRY_SHADER_IN_THE_PASS,
		BLEND_STATE_ALREADY_EXISTS,
		DEPTH_STENCIL_STATE_ALREADY_EXISTS
	};

	RegisterMessage registerTechnique(string_id techniqueName, const ID3DX11EffectTechnique* technique);
	RegisterMessage registerPass(string_id techniqueName, string_id passName, ID3DX11EffectPass* pass);
	RegisterMessage registerInputLayout(string_id techniqueName, string_id passName, ID3D11InputLayout* inputLayout);
	RegisterMessage registerStreamsInfo(string_id techniqueName, string_id passName, const std::vector<InputLayoutResource::StreamInfo>& streamsInfo);
	RegisterMessage registerFloat4x4(string_id techniqueName, string_id flt4x4Name, ID3DX11EffectMatrixVariable* flt4x4, unsigned int elementsCount);
	RegisterMessage registerFloat4(string_id techniqueName, string_id flt4Name, ID3DX11EffectVariable* flt4, unsigned int elementsCount);
	RegisterMessage registerFloat3(string_id techniqueName, string_id flt3Name, ID3DX11EffectVariable* flt3, unsigned int elementsCount);
	RegisterMessage registerFloat2(string_id techniqueName, string_id flt2Name, ID3DX11EffectVariable* flt2, unsigned int elementsCount);
	RegisterMessage registerFloat1(string_id techniqueName, string_id flt1Name, ID3DX11EffectVariable* flt1, unsigned int elementsCount);
	RegisterMessage registerInt2(string_id techniqueName, string_id int2Name, ID3DX11EffectVariable* int2, unsigned int elementsCount);
	RegisterMessage registerInt1(string_id techniqueName, string_id int1Name, ID3DX11EffectVariable* int1, unsigned int elementsCount);
	RegisterMessage registerStruct(string_id techniqueName, string_id structName, const StructResource& structRes);
	RegisterMessage registerTexture(string_id techniqueName, string_id textureName, const Texture2dResource& texRes);
	RegisterMessage registerTexturesArray(string_id techniqueName, string_id textureArrName, const Texture2dArrayResource& texArrRes);
	RegisterMessage registerVariableLocation(string_id techniqueName, string_id varName, const VariableLocation& varLocation);
	RegisterMessage registerVertexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, ID3D11Buffer* vertexBuffer, bool structured);
	RegisterMessage registerIndexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, ID3D11Buffer* indexBuffer, bool structured);
	RegisterMessage registerImage(string_id name, ID3D11ShaderResourceView* image);
	RegisterMessage registerImagesArray(string_id name, ID3D11ShaderResourceView* imagesArray);
	RegisterMessage registerPresenceOfGeometryShader(string_id techniqueName, string_id passName);
	RegisterMessage registerPrimitiveType(string_id techniqueName, string_id passName, PassResource::PrimitiveType primType);
	RegisterMessage registerVertexBufferSRV(string_id techniqueName, string_id passName, uint32_t meshId, ID3D11ShaderResourceView* mVerticesSRV);
	RegisterMessage registerIndexBufferSRV(string_id techniqueName, string_id passName, uint32_t meshId, ID3D11ShaderResourceView* mIndicesSRV);
	RegisterMessage registerBlendState(string_id blendStateName, ID3D11BlendState* blendState);
	RegisterMessage registerDepthStencilState(string_id depthStencilStateName, ID3D11DepthStencilState* depthStencilState);

	const std::vector<InputLayoutResource::StreamInfo>* getStreamsInfo(string_id techniqueName, string_id passName);
	ID3D11InputLayout* getInputLayout(string_id techniqueName, string_id passName);
	ID3DX11EffectPass* getPass(string_id techniqueName, string_id passName);
	const VariableLocation& getVariableLocation(string_id techniqueName, string_id variable);
	std::map<string_id, Float4x4Resource>& getFloat4x4s(string_id techniqueName);
	std::map<string_id, Float4Resource>& getFloat4s(string_id techniqueName);
	std::map<string_id, Float3Resource>& getFloat3s(string_id techniqueName);
	std::map<string_id, Float2Resource>& getFloat2s(string_id techniqueName);
	std::map<string_id, Float1Resource>& getFloat1s(string_id techniqueName);
	std::map<string_id, Int2Resource>& getInt2s(string_id techniqueName);
	std::map<string_id, Int1Resource>& getInt1s(string_id techniqueName);
	std::map<string_id, StructResource>& getStructures(string_id techniqueName);
	std::map<string_id, Texture2dResource>& getTextures(string_id techniqueName);
	std::map<string_id, Texture2dArrayResource>& getTexturesArrays(string_id techniqueName);
	ID3D11Buffer* getVertexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, bool structured);
	ID3D11Buffer* getIndexBuffer(string_id techniqueName, string_id passName, uint32_t meshId, bool structured);
	ID3D11ShaderResourceView* getImage(string_id name);
	ID3D11ShaderResourceView* getImagesArray(string_id name);
	bool isThereAGeometryShaderInThePass(string_id techniqueName, string_id passName);
	PassResource::PrimitiveType getPrimitiveType(string_id techniqueName, string_id passName);
	ID3D11ShaderResourceView* getVertexBufferSRV(string_id techniqueName, string_id passName, uint32_t meshId);
	ID3D11ShaderResourceView* getIndexBufferSRV(string_id techniqueName, string_id passName, uint32_t meshId);
	ID3D11BlendState* getBlendState(string_id blendStateName);
	ID3D11DepthStencilState* getDepthStencilState(string_id depthStencilStateName);

public:
	static ResourceManager* instance();
};
