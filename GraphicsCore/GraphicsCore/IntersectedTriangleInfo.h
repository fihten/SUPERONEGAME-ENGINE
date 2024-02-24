#pragma once 
#include <stdint.h>

struct IntersectedTriangleInfo
{
	uint32_t objectIndex;
	uint32_t triangleIndex;
	uint32_t packedBarycentricCoords;
};