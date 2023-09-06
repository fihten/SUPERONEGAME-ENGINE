#include "constants.hlsl"

#define dParam 0.0001
#define size0 5

Texture2D<float4> imageA;
Texture2D<float4> imageB;

RWTexture2D<uint> mapAtoB;
RWTexture2D<uint> mapBtoA;

RWTexture2D<uint> error;

float4 sampleImageB(float2 posInB)
{
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

float calculateDiscrepancy(uint2 posInA, float3x3 transform, uint sizeX, uint sizeY)
{
	float discrepancy = 0.0f;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint mips = 0;
	imageA.GetDimensions(mip, width, height, mips);

	for (int i = -sizeX; i <= sizeX; i++)
	{
		if (posInA.x + i < 0)
			continue;
		if (posInA.x + i >= width)
			break;

		for (int j = -sizeY; j <= sizeY; j++)
		{
			if (posInA.y + j < 0)
				continue;
			if (posInA.y + j >= height)
				break;

			uint2 posInAij = posInA + int2(i, j);

			float4 colorInAij = imageA[posInAij];

			float2 posInB = posInA + mul(float3(i, j, 1), transform).xy;
			float4 colorInBij = sampleImageB(posInB);

			float4 diff = colorInBij - colorInAij;
			discrepancy += dot(diff, diff);
		}
	}

	return discrepancy;
}

float gradientLength(uint2 posInA, float3x3 transform, uint sizeX, uint sizeY)
{
	float4 gradient;

	float discrepancy = calculateDiscrepancy(posInA, transform, sizeX, sizeY);

	float3x3 transform0 = transform;
	transform0[0][0] += dParam;
	float discrepancy0 = calculateDiscrepancy(posInA, transform0, sizeX, sizeY);

	float3x3 transform1 = transform;
	transform1[0][1] += dParam;
	float discrepancy1 = calculateDiscrepancy(posInA, transform1, sizeX, sizeY);

	float3x3 transform2 = transform;
	transform2[1][0] += dParam;
	float discrepancy2 = calculateDiscrepancy(posInA, transform2, sizeX, sizeY);
	
	float3x3 transform3 = transform;
	transform3[1][1] += dParam;
	float discrepancy3 = calculateDiscrepancy(posInA, transform3, sizeX, sizeY);

	gradient.x = (discrepancy0 - discrepancy) / dParam;
	gradient.y = (discrepancy1 - discrepancy) / dParam;
	gradient.z = (discrepancy2 - discrepancy) / dParam;
	gradient.w = (discrepancy3 - discrepancy) / dParam;

	return dot(gradient, gradient);
}

float4 gradientOfGradientLength(uint2 posInA, float3x3 transform, uint sizeX, uint sizeY)
{
	float4 gradient;

	float gradientLength = gradientLength(posInA, transform, sizeX, sizeY);

	float3x3 transform0 = transform;
	transform0[0][0] += dParam;
	float gradientLength0 = gradientLength(posInA, transform0, sizeX, sizeY);

	float3x3 transform1 = transform;
	transform1[0][1] += dParam;
	float gradientLength1 = gradientLength(posInA, transform1, sizeX, sizeY);

	float3x3 transform2 = transform;
	transform2[1][0] += dParam;
	float gradientLength2 = gradientLength(posInA, transform2, sizeX, sizeY);

	float3x3 transform3 = transform;
	transform3[1][1] += dParam;
	float gradientLength3 = gradientLength(posInA, transform3, sizeX, sizeY);

	gradient.x = (gradientLength0 - gradientLength) / dParam;
	gradient.y = (gradientLength1 - gradientLength) / dParam;
	gradient.z = (gradientLength2 - gradientLength) / dParam;
	gradient.w = (gradientLength3 - gradientLength) / dParam;

	return gradient;
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

void fittingTransformByGradientDescent(
	uint2 posInA,
	uint2 posInB,
	uint sizeX,
	uint sizeY,
	inout float3x3 transform
)
{
	for (int i = 0; i < 5; i++)
	{
		float gradientLength = gradientLength(posInA, transform, sizeX, sizeY);
		float4 gradient = gradientOfGradientLength(posInA, transform, sizeX, sizeY);

		float c = -gradientLength / dot(gradient, gradient);

		transform[0][0] += c * gradient.x;
		transform[0][1] += c * gradient.y;

		transform[1][0] += c * gradient.z;
		transform[1][1] += c * gradient.w;
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

	fittingTransformByGradientDescent(
		posInA,
		posInB,
		sizeX,
		sizeY,
		transform
	);

	uint err = gradientLength(posInA, transform, sizeX, sizeY)*1000000;
	InterlockedMin(error[posInA], err);
}

[numthreads(16, 16, 4)]
void CS_map_A_onto_B(uint3 dispatchThreadID : SV_DispatchThreadID)
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

	fittingTransformByGradientDescent(
		posInA,
		posInB,
		sizeX,
		sizeY,
		transform
	);

	uint err = gradientLength(posInA, transform, sizeX, sizeY) * 1000000;
	if (err == error[posInA])
	{
		uint original_value;
		InterlockedExchange(mapAtoB[posInA], dispatchThreadID.y);
	}
}

[numthreads(32, 32, 1)]
void CS_map_B_onto_A(uint3 dispatchThreadID : SV_DispatchThreadID)
{

}

technique11 DefineTheSamePointsOnTwoImages
{
	pass CalculateError
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_calculate_error()));
	}
	pass MapAontoB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_map_A_onto_B()));
	}
	pass MapBontoA
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_map_B_onto_A()));
	}
};