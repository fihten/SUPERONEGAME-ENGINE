#include "Segment.hlsl"
#include "SelectedObjects.hlsl"
#include "IntersectionUtils.hlsl"

StructuredBuffer<Segment> selectingSegments;
uint selectingSegmentsCount;

StructuredBuffer<float4> boundingSpheres;
uint boundingSpheresCount;

RWStructuredBuffer<SelectedObjects> selectedObjects;

[numthreads(8, 256, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint segmentIndex = dispatchThreadID.x;
	if (segmentIndex >= selectingSegmentsCount)
		return;

	uint objectIndex = dispatchThreadID.y;
	if (objectIndex >= boundingSpheresCount)
		return;

	Segment segment = selectingSegments[segmentIndex];
	float4 sphere = boundingSpheres[objectIndex];

	selectedObjects[segmentIndex][objectIndex] = 0;
	if (checkIntersection(segment, sphere))
		selectedObjects[segmentIndex][objectIndex] = 1;
}

technique11 RoughObjectsSelectionBySegments
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
};