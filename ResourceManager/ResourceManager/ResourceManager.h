#pragma once

#include <map>
#include <string>
#include "TechniqueResource.h"

class ResourceManager
{
	std::map<std::string, TechniqueResource> techniques;

public:
	enum RegisterMessage
	{
		OK,
		TECHNIQUE_ALREADY_EXISTS,
		TECHNIQUE_DOESNT_EXIST,
		PASS_ALREADY_EXISTS,
		PASS_DOESNT_EXIST,
		MATRIX_ALREADY_EXISTS,
		STRUCT_ALREADY_EXISTS
	};

	RegisterMessage registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique);
	RegisterMessage registerPass(const std::string& techniqueName, const std::string& passName, ID3DX11EffectPass* pass);
	RegisterMessage registerInputLayout(const std::string& techniqueName, const std::string& passName, ID3D11InputLayout* inputLayout);
	RegisterMessage registerStreamsInfo(const std::string& techniqueName, const std::string& passName, const std::vector<InputLayoutStreamInfo>& streamsInfo);
	RegisterMessage registerMatrix(const std::string& techniqueName, const std::string& matrixName, ID3DX11EffectMatrixVariable* matrix);
	RegisterMessage registerStruct(const std::string& techniqueName, const std::string& structName, const StructInfo& structInfo);
	RegisterMessage registerVariableLocations(const std::string& techniqueName, const std::map<std::string, std::string>& locationOfVariable);
	RegisterMessage registerVertexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId, ID3D11Buffer* vertexBuffer);
	RegisterMessage registerIndexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId, ID3D11Buffer* indexBuffer);

	const std::vector<InputLayoutStreamInfo>* getStreamsInfo(const std::string& techniqueName, const std::string& passName) const;
	ID3D11InputLayout* getInputLayout(const std::string& techniqueName, const std::string& passName) const;
	ID3DX11EffectPass* getPass(const std::string& techniqueName, const std::string& passName) const;
	const std::string& getVariableLocation(const std::string& techniqueName, const std::string& variable) const;
	void getMatrices(const std::string& techniqueName, std::map<std::string, ID3DX11EffectMatrixVariable*>& matrices);
	void getStructures(const std::string& techniqueName, std::map<std::string, StructInfo>& structs);
	ID3D11Buffer* getVertexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId);
	ID3D11Buffer* getIndexBuffer(const std::string& techniqueName, const std::string& passName, uint32_t meshId);
};

extern ResourceManager resourceManager;