#pragma once

#include <map>
#include <string>
#include <d3dx11effect.h>
#include "Mesh.h"

class ResourceManager
{
	struct InputLayoutStreamInfo;
	struct TechniqueResource
	{
		const ID3DX11EffectTechnique* technique = nullptr;

		// passes
		struct PassResource
		{
			ID3DX11EffectPass* pass = nullptr;
			ID3D11InputLayout* inputLayout = nullptr;

			std::vector<InputLayoutStreamInfo> streamsInfo;
		};
		std::map<std::string, PassResource> passes;

		// matrices
		std::map<std::string, ID3DX11EffectMatrixVariable*> matrices;

		// location of variable
		std::map<std::string, std::string> locationOfVariable;
	};
	std::map<std::string, TechniqueResource> techniques;

public:
	enum RegisterMessage
	{
		OK,
		TECHNIQUE_ALREADY_EXISTS,
		TECHNIQUE_DOESNT_EXIST,
		PASS_ALREADY_EXISTS,
		PASS_DOESNT_EXIST,
		MATRIX_ALREADY_EXISTS
	};

	struct InputLayoutStreamInfo
	{
		std::string name;
		Mesh::StreamType type;
		uint32_t size;
	};

	RegisterMessage registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique);
	RegisterMessage registerPass(const std::string& techniqueName, const std::string& passName, ID3DX11EffectPass* pass);
	RegisterMessage registerInputLayout(const std::string& techniqueName, const std::string& passName, ID3D11InputLayout* inputLayout);
	RegisterMessage registerStreamsInfo(const std::string& techniqueName, const std::string& passName, const std::vector<InputLayoutStreamInfo>&& streamsInfo);
	RegisterMessage registerMatrix(const std::string& techniqueName, const std::string& matrixName, ID3DX11EffectMatrixVariable* matrix);
	RegisterMessage registerVariableLocations(const std::string& techniqueName, const std::map<std::string, std::string>& locationOfVariable);

	const std::vector<InputLayoutStreamInfo>* getStreamsInfo(const std::string& techniqueName, const std::string& passName) const;
	ID3D11InputLayout* getInputLayout(const std::string& techniqueName, const std::string& passName) const;
	ID3DX11EffectPass* getPass(const std::string& techniqueName, const std::string& passName) const;
	const std::string& getVariableLocation(const std::string& techniqueName, const std::string& variable) const;
	void getMatrices(const std::string& techniqueName, std::map<std::string, ID3DX11EffectMatrixVariable*>& matrices);
};

extern ResourceManager resourceManager;