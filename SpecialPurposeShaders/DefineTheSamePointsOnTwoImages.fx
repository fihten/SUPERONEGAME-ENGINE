#include "constants.hlsl"

#define dParam 0.0001
#define size0 5

Texture2D<float4> imageA;
Texture2D<float4> imageB;

RWTexture2D<uint> mapAtoB;
RWTexture2D<uint> mapBtoA;

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

float calculateDiscrepancy0(uint2 posInA, float3x3 transform, uint sizeX, uint sizeY)
{
	float discrepancy = 0.0f;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint mips = 0;
	imageA.GetDimensions(mip, width, height, mips);

	for (int i = posInA.x - sizeX; i <= posInA.x + sizeX; i++)
	{
		if (i < 0)
			continue;
		if (i >= width)
			break;

		for (int j = posInA.y - sizeY; j <= posInA.y + sizeY; j++)
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

float calculateSquaredGradientLength(uint2 posInA, float3x3 transform, uint sizeX, uint sizeY)
{
	float4 gradient;

	float discrepancy = calculateDiscrepancy0(posInA, transform, sizeX, sizeY);

	float3x3 transform0 = transform;
	transform0[0][0] += dParam;
	float discrepancy0 = calculateDiscrepancy0(posInA, transform0, sizeX, sizeY);

	float3x3 transform1 = transform;
	transform1[0][1] += dParam;
	float discrepancy1 = calculateDiscrepancy0(posInA, transform1, sizeX, sizeY);

	float3x3 transform2 = transform;
	transform2[1][0] += dParam;
	float discrepancy2 = calculateDiscrepancy0(posInA, transform2, sizeX, sizeY);
	
	float3x3 transform3 = transform;
	transform3[1][1] += dParam;
	float discrepancy3 = calculateDiscrepancy0(posInA, transform3, sizeX, sizeY);
}

void calculateCoarseTransformByNetMethod(
	uint2 posInA, 
	uint2 posInB, 
	uint sizeX, 
	uint sizeY, 
	out float3x3 transform
)
{
	float minScale = 0.0f;
	float maxScale = 10.0f;
	uint m = 1000;
	float dS = (maxScale - minScale) / m;

	float minAngle = 0.0f;
	float maxAngle = 2 * PI;
	uint n = 100;
	float dA = (maxAngle - minAngle) / n;

	float discrepancy = 1000000;
	for (int si = 0; si < m; si++)
	{
		for (int ai = 0; ai < n; ai++)
		{
			float scale = minScale + si * dS;
			float angle = minAngle + ai * dA;

			float3x3 translate0 = float3x3(
				1, 0, 0,
				0, 1, 0,
				-posInA.x, -posInA.y, 1
				);
			float3x3 scaleRotate = scale * float3x3(
				cos(angle), sin(angle), 0,
				-sin(angle), cos(angle), 0,
				0, 0, 1
				);
			float3x3 translate1 = float3x3(
				1, 0, 0,
				0, 1, 0,
				posInB.x, posInB.y, 1
				);

			float3x3 tmpTransform = mul(translate0, mul(scaleRotate, translate1));
			float tmpDiscrepancy = calculateDiscrepancy0(posInA, tmpTransform, sizeX, sizeY);
			if (tmpDiscrepancy < discrepancy)
			{
				discrepancy = tmpDiscrepancy;
				transform = tmpTransform;
			}
		}
	}
}

[numthreads(16,16,4)]
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

	uint2 posInA(dispatchThreadID.x % width, dispatchThreadID.x / width);
	uint2 posInB(dispatchThreadID.y % width, dispatchThreadID.y / width);
	uint size = size0 + dispatchThreadID.z;

	float3x3 transformAxisX;
	calculateCoarseTransformByNetMethod(posInA, posInB, size, 0, transformAxisX);
	float3 axisX = float3(1, 0, 0);
	axisX = mul(axisX, transformAxisX);

	float3x3 transformAxisY;
	calculateCoarseTransformByNetMethod(posInA, posInB, 0, size, transformAxisY);
	float3 axisY = float3(0, 1, 0);
	axisY = mul(axisY, transformAxisY);

	float3x3 translate0 = float3x3(
		1, 0, 0,
		0, 1, 0,
		-posInA.x, -posInA.y, 1
		);
	float3x3 scaleRotate = float3x3(
		axisX,
		axisY,
		float3(0, 0, 1)
		);
	float3x3 translate1 = float3x3(
		1, 0, 0,
		0, 1, 0,
		posInB.x, posInB.y, 1
		);

	float3x3 transform = mul(translate0, mul(scaleRotate, translate1));
}