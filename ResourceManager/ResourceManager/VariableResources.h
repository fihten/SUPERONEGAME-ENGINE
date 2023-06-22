#pragma once
#include <string>
#include <D3dx11effect.h>

struct Float4x4Resource
{
	std::string location = "";
	unsigned int elementsCount = 0;
	ID3DX11EffectMatrixVariable* ptr = nullptr;
};

struct Float4Resource
{
	std::string location = "";
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

struct Float3Resource
{
	std::string location = "";
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

struct Float2Resource
{
	std::string location = "";
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

struct Float1Resource
{
	std::string location = "";
	unsigned int elementsCount = 0;
	ID3DX11EffectVariable* ptr = nullptr;
};

#define MAX_COUNT_OF_FIELDS 16
struct StructResource
{
	std::string name = "";

	struct Field
	{
		unsigned int offset = 0;
		unsigned int bytes = 0;
		std::string type = "";
		std::string name = "";
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