#pragma once
#include <d3d11.h>
#include "StreamType.h"
#include "StringManager.h"

struct InputLayoutResource
{
	struct StreamInfo
	{
		string_id name;
		StreamType type;
		uint32_t size;
	};
	std::vector<StreamInfo> streamsInfo;
	ID3D11InputLayout* ptr = nullptr;
};