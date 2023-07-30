#include "IntersectionUtils.hlsl"

Frustum selectorFrustum;
StructuredBuffer<Segment> selectorFrustumDiagonals;

float4x4 WV;
float threshold;

StructuredBuffer<float3> vertices;
StructuredBuffer<uint> indicies;
uint trianglesCount;

RWStructuredBuffer<uint> selectedTriangles;

[numthreads(256, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint triangleIndex = dispatchThreadID.x;
	if (triangleIndex >= trianglesCount)
		return;

	uint vi0 = indicies[3 * triangleIndex + 0];
	uint vi1 = indicies[3 * triangleIndex + 1];
	uint vi2 = indicies[3 * triangleIndex + 2];

	float4 v0 = float4(vertices[vi0], 1);
	float4 v1 = float4(vertices[vi1], 1);
	float4 v2 = float4(vertices[vi2], 1);

	v0 = mul(v0, WV);
	v1 = mul(v1, WV);
	v2 = mul(v2, WV);

	Triangle tri;
	tri.v0 = v0.xyz;
	tri.v1 = v1.xyz;
	tri.v2 = v2.xyz;

	selectedTriangles[triangleIndex] = 0;
	if (checkIntersection(selectorEnvelope, tri, threshold))
		selectedTriangles[triangleIndex] = 1;
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