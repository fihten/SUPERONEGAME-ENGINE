#include "IntersectionUtils.hlsl"
#include "ObjectInfo.hlsl"

Frustum selectorFrustum;
StructuredBuffer<Segment> selectorFrustumDiagonals;

float4x4 view;
float threshold;

StructuredBuffer<float3> vertices;
StructuredBuffer<uint> indicies;
StructuredBuffer<ObjectInfo> objectsInfo;

StructuredBuffer<uint> roughlySelectedObjects;
uint objectsCount;

RWStructuredBuffer<uint> selectedTriangles;

[numthreads(1, 256, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint objectIndex = dispatchThreadID.x;
	if (objectIndex >= objectsCount)
		return;
	if (roughlySelectedObjects[objectIndex] == 0)
		return;

	uint triangleIndex = dispatchThreadID.y;
	if (triangleIndex >= objectsInfo[objectIndex].trianglesCount)
		return;

	uint ii0 = 3 * triangleIndex + 0 + objectsInfo[objectIndex].indicesOffset;
	uint ii1 = 3 * triangleIndex + 1 + objectsInfo[objectIndex].indicesOffset;
	uint ii2 = 3 * triangleIndex + 2 + objectsInfo[objectIndex].indicesOffset;

	uint vi0 = indicies[ii0] + objectsInfo[objectIndex].verticesOffset;
	uint vi1 = indicies[ii1] + objectsInfo[objectIndex].verticesOffset;
	uint vi2 = indicies[ii2] + objectsInfo[objectIndex].verticesOffset;

	float4 v0 = float4(vertices[vi0], 1);
	float4 v1 = float4(vertices[vi1], 1);
	float4 v2 = float4(vertices[vi2], 1);

	float4x4 WV = mul(objectsInfo[objectIndex].world, view);

	v0 = mul(v0, WV);
	v1 = mul(v1, WV);
	v2 = mul(v2, WV);

	Triangle tri;
	tri.v0 = v0.xyz;
	tri.v1 = v1.xyz;
	tri.v2 = v2.xyz;

	if (checkIntersection(selectorFrustum, selectorFrustumDiagonals, tri, threshold))
		InterlockedAdd(selectedTriangles[objectIndex], 1);
}

technique11 FineObjectsSelection
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
};