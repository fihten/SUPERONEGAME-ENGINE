#include "EnvelopeUtils.hlsl"

#define MaxEnvelopesCount 512

Envelope envelopes[MaxEnvelopesCount];
uint envelopesCount;

Envelope selectorEnvelope;

RWStructuredBuffer<uint> selectedEnvelopes;

float4x4 VP;

[numthreads(256,1,1)]
void CS(int3 dispatchThreadID : SV_DispatchThreadID)
{
	int envelopeIndex = dispatchThreadID.x;
	if (envelopeIndex >= envelopesCount)
		return;

	selectedEnvelopes[envelopeIndex] = 0;
	Envelope envelope = envelopes[envelopeIndex];
	envelope.transform = mul(envelope.transform, VP);
	if(checkIntersection(selectorEnvelope, envelope))
		selectedEnvelopes[envelopeIndex] = 1;
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