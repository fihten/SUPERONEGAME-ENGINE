#include "constants.hlsl"
#include "Segment.hlsl"
#include "IntersectionUtils.hlsl"
#include "ObjectInfo.hlsl"

StructuredBuffer<Segment> selectingSegments;
StructuredBuffer<uint> selectedObjects;

uint selectingSegmentsCount;
uint objectsCount;

StructuredBuffer<float3> vertices;
StructuredBuffer<uint> indicies;
StructuredBuffer<ObjectInfo> objectsInfo;

RWStructuredBuffer<uint> distancesToClosestObjects;
RWStructuredBuffer<uint> closestObjects;

[numthreads(256, 1, 4)]
void CS_distance_to_object(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint segmentIndex = dispatchThreadID.z;
	if (segmentIndex >= selectingSegmentsCount)
		return;

	uint objectIndex = dispatchThreadID.y;
	if (objectIndex >= objectsCount)
		return;

	uint selectedObjectIndex = segmentIndex * MAX_BOUNDING_SPHERES_COUNT + objectIndex;
	if (selectedObjects[selectedObjectIndex] == 0)
		return;

	uint triangleIndex = dispatchThreadID.x;
	if (triangleIndex >= objectsInfo[objectIndex].trianglesCount)
		return;

	uint ii0 = objectsInfo[objectIndex].indicesOffset + 3 * triangleIndex + 0;
	uint ii1 = objectsInfo[objectIndex].indicesOffset + 3 * triangleIndex + 1;
	uint ii2 = objectsInfo[objectIndex].indicesOffset + 3 * triangleIndex + 2;

	uint vi0 = objectsInfo[objectIndex].verticesOffset + indicies[ii0];
	uint vi1 = objectsInfo[objectIndex].verticesOffset + indicies[ii1];
	uint vi2 = objectsInfo[objectIndex].verticesOffset + indicies[ii2];

	Triangle tri;
	tri.v0 = vertices[vi0];
	tri.v1 = vertices[vi1];
	tri.v2 = vertices[vi2];
	
	float4x4 w = objectsInfo[objectIndex].world;
	tri.v0 = mul(float4(tri.v0, 1), w).xyz;
	tri.v1 = mul(float4(tri.v1, 1), w).xyz;
	tri.v2 = mul(float4(tri.v2, 1), w).xyz;

	Segment seg = selectingSegments[segmentIndex];
	float t = 0;
	if (!checkIntersection(tri, seg, t))
		return;

	uint distance = length(seg.v1 - seg.v0) * t * 1000000;
	InterlockedMin(distancesToClosestObjects[segmentIndex], distance);
}

[numthreads(256, 1, 4)]
void CS_closest_object(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint segmentIndex = dispatchThreadID.z;
	if (segmentIndex >= selectingSegmentsCount)
		return;

	uint objectIndex = dispatchThreadID.y;
	if (objectIndex >= objectsCount)
		return;

	uint triangleIndex = dispatchThreadID.x;
	if (triangleIndex >= objectsInfo[objectIndex].trianglesCount)
		return;

	uint ii0 = objectsInfo[objectIndex].indicesOffset + 3 * triangleIndex + 0;
	uint ii1 = objectsInfo[objectIndex].indicesOffset + 3 * triangleIndex + 1;
	uint ii2 = objectsInfo[objectIndex].indicesOffset + 3 * triangleIndex + 2;

	uint vi0 = objectsInfo[objectIndex].verticesOffset + indicies[ii0];
	uint vi1 = objectsInfo[objectIndex].verticesOffset + indicies[ii1];
	uint vi2 = objectsInfo[objectIndex].verticesOffset + indicies[ii2];

	Triangle tri;
	tri.v0 = vertices[vi0];
	tri.v1 = vertices[vi1];
	tri.v2 = vertices[vi2];

	float4x4 w = objectsInfo[objectIndex].world;
	tri.v0 = mul(float4(tri.v0, 1), w).xyz;
	tri.v1 = mul(float4(tri.v1, 1), w).xyz;
	tri.v2 = mul(float4(tri.v2, 1), w).xyz;

	Segment seg = selectingSegments[segmentIndex];
	float t = 0;
	if (!checkIntersection(tri, seg, t))
		return;

	uint distance = length(seg.v1 - seg.v0) * t * 1000000;
	uint objectIndexOriginal = 0;
	if (distance == distancesToClosestObjects[segmentIndex])
		InterlockedExchange(closestObjects[segmentIndex], objectIndex, objectIndexOriginal);
}

technique11 FineObjectsSelectionBySegments
{
	pass CalculationOfDistancesToClosestObjects
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_distance_to_object()));
	}
	pass SearchOfClosestObjects
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_closest_object()));
	}
};