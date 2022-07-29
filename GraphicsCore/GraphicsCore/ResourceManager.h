#pragma once

#include <map>
#include <string>
#include <d3dx11effect.h>

class ResourceManager
{
	struct TechniqueResource
	{
		const ID3DX11EffectTechnique* technique = nullptr;

		// passes
		struct PassResource
		{
			const ID3DX11EffectPass* pass = nullptr;
			const ID3D11InputLayout* inputLayout = nullptr;
		};
		std::map<std::string, PassResource> passes;

		// matrices
		std::map<std::string, const ID3D10EffectMatrixVariable*> matrices;
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

	RegisterMessage registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique);
	RegisterMessage registerPass(const std::string& techniqueName, const std::string& passName, const ID3DX11EffectPass* pass);
	RegisterMessage registerInputLayout(const std::string& techniqueName, const std::string& passName, const ID3D11InputLayout* inputLayout);
	RegisterMessage registerMatrix(const std::string& techniqueName, const std::string& matrixName, const ID3D10EffectMatrixVariable* matrix);
};