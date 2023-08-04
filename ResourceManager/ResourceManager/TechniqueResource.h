#pragma once
#include "VariableResources.h"
#include "PassResource.h"
#include "StringManager.h"

struct TechniqueResource
{
	const ID3DX11EffectTechnique* ptr = nullptr;

	// passes
	std::map<string_id, PassResource> passes;
	// float4x4
	std::map<string_id, Float4x4Resource> float4x4s;
	// float4
	std::map<string_id, Float4Resource> float4s;
	// float3
	std::map<string_id, Float3Resource> float3s;
	// float2
	std::map<string_id, Float2Resource> float2s;
	// float1
	std::map<string_id, Float1Resource> float1s;
	// structures
	std::map<string_id, StructResource> structures;
	// textures
	std::map<string_id, Texture2dResource> textures;
	// textures arrays
	std::map<string_id, Texture2dArrayResource> texturesArrays;
};