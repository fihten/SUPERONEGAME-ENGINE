#pragma once
#include <D3dx11effect.h>
#include <vector>
#include "InputLayoutResource.h"

struct PassResource
{
	ID3DX11EffectPass* ptr = nullptr;

	InputLayoutResource inputLayout;
	std::map<uint32_t, ID3D11Buffer*> vertexBuffers;
	std::map<uint32_t, ID3D11Buffer*> indexBuffers;
};