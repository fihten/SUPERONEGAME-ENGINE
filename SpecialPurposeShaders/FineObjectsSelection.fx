#include "Envelope.hlsl"

#define MAX_VERTICES_COUNT

Envelope selectorEnvelope;
float4x4 VP;

float3 vertices[MAX_VERTICES_COUNT];
uint indicies[3 * MAX_VERTICES_COUNT];

RWStructuredBuffer<uint> selectedTriangles;

[numthreads(256, 1, 1)]
void CS(int3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
}
