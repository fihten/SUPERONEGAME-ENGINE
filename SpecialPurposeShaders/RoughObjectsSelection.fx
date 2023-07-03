#include "EnvelopeUtils.hlsl"

#define MaxEnvelopesCount 512

Envelope envelopes[MaxEnvelopesCount];
Envelope selectorEnvelope;

RWStructuredBuffer<uint> selectedEnvelopes;

[numthreads(256,1,1)]
void CS(int3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	selectedEnvelopes[i] = 0;
	if(checkIntersection(selectorEnvelope, envelopes[i]))
		selectedEnvelopes[i] = 1;
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