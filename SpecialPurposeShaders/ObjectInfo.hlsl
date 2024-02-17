#ifndef OBJECT_INFO
#define OBJECT_INFO

struct ObjectInfo
{
	float4x4 world;
	uint verticesOffset;
	uint indicesOffset;
	uint trianglesCount;
	uint verticesCount;
};

#endif