#define dParam 0.0001

Texture2D<float4> imageA;
Texture2D<float4> imageB;

uint scanAreaSize;

RWTexture2D<uint2> mapAtoB;
RWTexture2D<uint2> mapBtoA;

RWTexture2D<uint> error;

float4 sampleImageB(uint2 posInA, float3x3 transform)
{
	float2 posInB = mul(float3(posInA, 1), transform).xy;
	
	uint2 posInB0 = floor(posInB);
	uint2 posInB1 = ceil(posInB);

	uint2 posInB00 = uint2(posInB0.x, posInB0.y);
	uint2 posInB01 = uint2(posInB0.x, posInB1.y);

	uint2 posInB10 = uint2(posInB1.x, posInB0.y);
	uint2 posInB11 = uint2(posInB1.x, posInB1.y);

	float4 colorInB00 = imageB[posInB00];
	float4 colorInB01 = imageB[posInB01];

	float4 colorInB10 = imageB[posInB10];
	float4 colorInB11 = imageB[posInB11];

	float2 l = smoothstep(posInB0, posInB1, posInB);
	
	float4 colorInB0 = lerp(colorInB00, colorInB01, l.y);
	float4 colorInB1 = lerp(colorInB10, colorInB11, l.y);

	float4 colorInB = lerp(colorInB0, colorInB1, l.x);

	return colorInB;
}

float calculateDiscrepancy0(uint2 posInA, float3x3 transform)
{
	float discrepancy = 0.0f;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint mips = 0;
	imageA.GetDimensions(mip, width, height, mips);

	for (int i = posInA.x - scanAreaSize; i <= posInA.x + scanAreaSize; i++)
	{
		if (i < 0)
			continue;
		if (i >= width)
			break;

		for (int j = posInA.y - scanAreaSize; j <= posInA.y + scanAreaSize; j++)
		{
			if (j < 0)
				continue;
			if (j >= height)
				break;

			uint2 posInAij = uint2(i, j);

			float4 colorInAij = imageA[posInAij];
			float4 colorInBij = sampleImageB(posInAij, transform);

			float4 diff = colorInBij - colorInAij;
			discrepancy += dot(diff, diff);
		}
	}

	return discrepancy;
}

[numthreads(32,32,1)]
void CS_calculate_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint mips = 0;
	imageA.GetDimensions(mip, width, height, mips);

	uint wXh = width * height;
	if (dispatchThreadID.x >= wXh)
		return;
	if (dispatchThreadID.y >= wXh)
		return;
	if (dispatchThreadID.x >= dispatchThreadID.y)
		return;

	uint2 posInA(dispatchThreadID.x % width, dispatchThreadID.x / width);
	uint2 posInB(dispatchThreadID.y % width, dispatchThreadID.y / width);

}