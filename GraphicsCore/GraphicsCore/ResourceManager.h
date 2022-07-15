#pragma once

#include <map>
#include <string>
#include <d3dx11effect.h>

class ResourceManager
{
	struct TechniqueResource
	{
		const ID3DX11EffectTechnique* technique = nullptr;

		struct PassResource
		{
			const ID3DX11EffectPass* pass = nullptr;
			const ID3D11InputLayout* inputLayout = nullptr;
		};
		std::map<std::string, PassResource> passes;
	};
	std::map<std::string, TechniqueResource> techniques;

public:
	enum RegisterMessage
	{
		OK,
		TECHNIQUE_ALREADY_EXISTS,
		TECHNIQUE_DOESNT_EXIST,
		PASS_ALREADY_EXISTS
	};

	RegisterMessage registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique);
	RegisterMessage registerPass(const std::string& techniqueName, const std::string& passName, const ID3DX11EffectPass* pass);
};