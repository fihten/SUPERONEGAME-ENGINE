#ifndef ENVELOPE
#define ENVELOPE

struct Envelope
{
	float3 min;
	float3 max;
	float4x4 transform;
};

#endif