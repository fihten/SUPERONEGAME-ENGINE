#include "Envelope.hlsl"
#include "IntersectionUtils.hlsl"

Envelope selectorEnvelope;
float4x4 WVP;
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

	v0 = mul(v0, WVP);
	v1 = mul(v1, WVP);
	v2 = mul(v2, WVP);

	Triangle tri;
	tri.v0 = v0.xyz / v0.w;
	tri.v1 = v1.xyz / v1.w;
	tri.v2 = v2.xyz / v2.w;

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