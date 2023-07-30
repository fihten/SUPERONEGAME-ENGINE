#include "IntersectionUtils.hlsl"

StructuredBuffer<float4> boundingSpheres;
uint spheresCount;

Frustum selectorFrustum;
RWStructuredBuffer<uint> selectedObjects;

float4x4 V;

[numthreads(256,1,1)]
void CS(int3 dispatchThreadID : SV_DispatchThreadID)
{
	int sphereIndex = dispatchThreadID.x;
	if (sphereIndex >= spheresCount)
		return;

	float4 sphere = boundingSpheres[sphereIndex];
	sphere.w = 1;
	sphere = mul(sphere, V);
	sphere.w = boundingSpheres[sphereIndex].w;

	selectedObjects[sphereIndex] = 0;
	if(checkIntersection(selectorFrustum, sphere))
		selectedObjects[sphereIndex] = 1;
}

technique11 RoughObjectsSelection
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS()));
	}
};