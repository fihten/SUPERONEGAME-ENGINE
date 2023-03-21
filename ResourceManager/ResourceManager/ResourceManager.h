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

public:
	enum RegisterMessage
	{
		OK,
		TECHNIQUE_ALREADY_EXISTS,
		TECHNIQUE_DOESNT_EXIST,
		PASS_ALREADY_EXISTS,
		PASS_DOESNT_EXIST,
		FLOAT4X4_ALREADY_EXISTS,
		VARIABLE_DOESNT_EXIST,
		STRUCT_ALREADY_EXISTS
	};

	RegisterMessage registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique);
	RegisterMessage registerPass(const std::string& techniqueName, const std::string& passName, ID3DX11EffectPass* pass);
	RegisterMessage registerInputLayout(const std::string& techniqueName, const std::string& passName, ID3D11InputLayout* inputLayout);
	RegisterMessage registerStreamsInfo(const std::string& techniqueName, const std::string& passName, const std::vector<InputLayoutResource::StreamInfo>& streamsInfo);
	RegisterMessage registerFloat4x4(const std::string& techniqueName, const std::string& flt4x4Name, ID3DX11EffectMatrixVariable* flt4x4);
	RegisterMessage registerStruct(const std::string& techniqueName, const std::string& structName, const StructResource& structRes);
	RegisterMessage registerVariableLocation(const std::string& techniqueName, const std::string& varName, const std::string& varLocation);
	RegisterMessage registerVertexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId, ID3D11Buffer* vertexBuffer);
	RegisterMessage registerIndexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId, ID3D11Buffer* indexBuffer);

	const std::vector<InputLayoutResource::StreamInfo>* getStreamsInfo(const std::string& techniqueName, const std::string& passName) const;
	ID3D11InputLayout* getInputLayout(const std::string& techniqueName, const std::string& passName) const;
	ID3DX11EffectPass* getPass(const std::string& techniqueName, const std::string& passName) const;
	const std::string& getVariableLocation(const std::string& techniqueName, const std::string& variable) const;
	void getFloat4x4s(const std::string& techniqueName, std::map<std::string, Float4x4Resource>& flt4x4s);
	void getStructures(const std::string& techniqueName, std::map<std::string, StructResource>& structs);
	ID3D11Buffer* getVertexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId);
	ID3D11Buffer* getIndexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId);

public:
	static ResourceManager* instance();
};
