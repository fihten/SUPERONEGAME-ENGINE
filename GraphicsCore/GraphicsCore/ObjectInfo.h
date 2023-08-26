#pragma once
#include "Matrix4x4.h"

struct ObjectInfo
{
	flt4x4 world;
	uint32_t verticesOffset;
	uint32_t indicesOffset;
	uint32_t trianglesCount;
};