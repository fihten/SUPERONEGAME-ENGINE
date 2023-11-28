#include "ColorSpace.hlsl"

Texture2D<float4> tex;
RWStructuredBuffer<int> statistic;

int N;
int width;
int height;

int radius;

int2 r0;

[numthreads(16, 16, 4)]
void cs(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int2 r = dispatchThreadID.xy;
	r -= int2(radius, radius);
	if (length(r) > radius)
		return;

	r = r0 + r;
	if (r.x < 0)
		return;
	if (r.x > width - 1)
		return;
	if (r.y < 0)
		return;
	if (r.y > height - 1)
		return;

	int ch = dispatchThreadID.z;
	if (ch > 2)
		return;

	float3 v3 = adobeToRaw(tex[r].rgb);
	float v = v3[ch];

	float h = 1.0f / N;
	int index = floor(v / h);
	InterlockedAdd(statistic[index], 1);
}

technique11 CalculateStatisticOfTexture
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs()));
	}
};