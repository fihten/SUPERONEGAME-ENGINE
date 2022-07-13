#pragma once

#include <map>
#include <string>
#include <d3dx11effect.h>

class ResourceManager
{
	struct TechniqueResource
	{
		const ID3DX11EffectTechnique* technique = nullptr;
	};
	std::map<std::string, TechniqueResource> techniques;

public:
	enum RegisterMessage
	{
		OK,
		TECHNIQUE_ALREADY_EXIST
	};

	RegisterMessage registerTechnique(const std::string& techniqueName, const ID3DX11EffectTechnique* technique);
};