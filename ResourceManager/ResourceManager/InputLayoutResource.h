#pragma once
#include <string>
#include <d3d11.h>
#include "Mesh.h"

struct InputLayoutResource
{
	struct StreamInfo
	{
		std::string name;
		Mesh::StreamType type;
		uint32_t size;
	};
	std::vector<StreamInfo> streamsInfo;
	ID3D11InputLayout* ptr = nullptr;
};