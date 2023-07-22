#ifndef ENVELOPE
#define ENVELOPE

struct Envelope
{
	float3 min;
	float pad0;
	float3 max;
	float pad1;
	float4x4 transform;
};

#endif