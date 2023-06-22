#pragma once
#include "VariableResources.h"
#include "PassResource.h"

struct TechniqueResource
{
	const ID3DX11EffectTechnique* ptr = nullptr;

	// passes
	std::map<std::string, PassResource> passes;
	// float4x4
	std::map<std::string, Float4x4Resource> float4x4s;
	// float4
	std::map<std::string, Float4Resource> float4s;
	// float3
	std::map<std::string, Float3Resource> float3s;
	// float2
	std::map<std::string, Float2Resource> float2s;
	// float1
	std::map<std::string, Float1Resource> float1s;
	// structures
	std::map<std::string, StructResource> structures;
	// textures
	std::map<std::string, Texture2dResource> textures;
	// textures arrays
	std::map<std::string, Texture2dArrayResource> texturesArrays;
};