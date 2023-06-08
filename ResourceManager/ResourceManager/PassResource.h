#pragma once
#include <D3dx11effect.h>
#include <vector>
#include "InputLayoutResource.h"

struct PassResource
{
	enum PrimitiveType
	{
		NONE = 0,
		POINT = 1,
		LINE = 2,
		TRIANGLE = 3,
		LINEADJ = 4,
		TRIANGLEADJ = 5
	};

	ID3DX11EffectPass* ptr = nullptr;

	InputLayoutResource inputLayout;
	std::map<uint32_t, ID3D11Buffer*> vertexBuffers;
	std::map<uint32_t, ID3D11Buffer*> indexBuffers;

	bool isThereAGeometryShaderHere = false;
	PrimitiveType primType = NONE;
};