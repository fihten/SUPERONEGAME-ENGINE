#ifndef ENVELOPE_UTILS
#define ENVELOPE_UTILS

#include "Envelope.hlsl"

#define FLT_MAX 1000000
#define FLT_MIN -1000000

void evaluate(in Envelope envelope, out float3 min, out float3 max)
{
	float4 vertices[8] =
	{
		float4(envelope.min.x, envelope.min.y, envelope.min.z, 1),
		float4(envelope.min.x, envelope.min.y, envelope.max.z, 1),
		float4(envelope.min.x, envelope.max.y, envelope.min.z, 1),
		float4(envelope.min.x, envelope.max.y, envelope.max.z, 1),
		float4(envelope.max.x, envelope.min.y, envelope.min.z, 1),
		float4(envelope.max.x, envelope.min.y, envelope.max.z, 1),
		float4(envelope.max.x, envelope.max.y, envelope.min.z, 1),
		float4(envelope.max.x, envelope.max.y, envelope.max.z, 1)
	};
	
	min = float3(FLT_MAX, FLT_MAX, FLT_MAX);
	max = float3(FLT_MIN, FLT_MIN, FLT_MIN);
	
	[unroll]
	for (int i = 0; i < 8; i++)
	{
		float4 vH = mul(vertices[i], envelope.transform);
		float3 vNDC = vH.xyz / vH.w;

		if (vNDC.x < min.x)
			min.x = vNDC.x;
		if (vNDC.y < min.y)
			min.y = vNDC.y;
		if (vNDC.z < min.z)
			min.z = vNDC.z;

		if (vNDC.x > max.x)
			max.x = vNDC.x;
		if (vNDC.y > max.y)
			max.y = vNDC.y;
		if (vNDC.z > max.z)
			max.z = vNDC.z;
	}
}

bool checkIntersection(Envelope envelope0, Envelope envelope1)
{
	float3 min0;
	float3 max0;
	evaluate(envelope0, min0, max0);

	float3 min1;
	float3 max1;
	evaluate(envelope1, min1, max1);

	float3 min = max(min0, min1);
	float3 max = min(max0, max1);

	return min.x <= max.x && min.y <= max.y && min.z <= max.z;
}

#endif