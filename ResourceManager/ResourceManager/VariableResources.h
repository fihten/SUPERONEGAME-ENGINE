#pragma once

#include <D3dx11effect.h>
#include "StringManager.h"
#include "VariableLocation.h"

struct Float4x4Resource
{
	VariableLocation location;
	unsigned int elementsCount = 0;
	ID3DX11EffectMatrixVariable* ptr = nullptr;
};

struct Float4Resource
{
	VariableLocation location;
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

struct Float3Resource
{
	VariableLocation location;
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

struct Float2Resource
{
	VariableLocation location;
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

struct Float1Resource
{
	VariableLocation location;
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

#define MAX_COUNT_OF_FIELDS 16
struct StructResource
{
	string_id name = string_id(-1);

	struct Field
	{
		unsigned int offset = 0;
		unsigned int bytes = 0;
		string_id type = string_id(-1);
		string_id name = string_id(-1);
	};
	Field fields[MAX_COUNT_OF_FIELDS];
	unsigned int fieldsCount;
	unsigned int bytes;

	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

struct Texture2dResource
{
	ID3DX11EffectShaderResourceVariable* tex = nullptr;
};

struct Texture2dArrayResource
{
	ID3DX11EffectShaderResourceVariable* texArr = nullptr;
};