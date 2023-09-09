#include "constants.hlsl"

#define size0 5

Texture2DArray<float4> imageA;
Texture2DArray<float4> imageB;

RWTexture2D<uint> mapAtoB;
RWTexture2D<uint> mapBtoA;

RWTexture2D<uint> error;

float4 sampleImage(Texture2DArray<float4> image, float2 fPos)
{
	uint2 uiPos = floor(fPos);

	float4 a00 = image[uint3(uiPos, 0)];
	float4 a10 = image[uint3(uiPos, 1)];
	float4 a20 = image[uint3(uiPos, 2)];
	float4 a30 = image[uint3(uiPos, 3)];
	float4 a40 = image[uint3(uiPos, 4)];
	float4 a50 = image[uint3(uiPos, 5)];

	float4 a01 = image[uint3(uiPos, 6)];
	float4 a11 = image[uint3(uiPos, 7)];
	float4 a21 = image[uint3(uiPos, 8)];
	float4 a31 = image[uint3(uiPos, 9)];
	float4 a41 = image[uint3(uiPos, 10)];
	float4 a51 = image[uint3(uiPos, 11)];

	float4 a02 = image[uint3(uiPos, 12)];
	float4 a12 = image[uint3(uiPos, 13)];
	float4 a22 = image[uint3(uiPos, 14)];
	float4 a32 = image[uint3(uiPos, 15)];
	float4 a42 = image[uint3(uiPos, 16)];
	float4 a52 = image[uint3(uiPos, 17)];

	float4 a03 = image[uint3(uiPos, 18)];
	float4 a13 = image[uint3(uiPos, 19)];
	float4 a23 = image[uint3(uiPos, 20)];
	float4 a33 = image[uint3(uiPos, 21)];
	float4 a43 = image[uint3(uiPos, 22)];
	float4 a53 = image[uint3(uiPos, 23)];

	float4 a04 = image[uint3(uiPos, 24)];
	float4 a14 = image[uint3(uiPos, 25)];
	float4 a24 = image[uint3(uiPos, 26)];
	float4 a34 = image[uint3(uiPos, 27)];
	float4 a44 = image[uint3(uiPos, 28)];
	float4 a54 = image[uint3(uiPos, 29)];

	float4 a05 = image[uint3(uiPos, 30)];
	float4 a15 = image[uint3(uiPos, 31)];
	float4 a25 = image[uint3(uiPos, 32)];
	float4 a35 = image[uint3(uiPos, 33)];
	float4 a45 = image[uint3(uiPos, 34)];
	float4 a55 = image[uint3(uiPos, 35)];

	float2 xy = fPos - uiPos;
	
	float x = xy.x;
	float x2 = x * x;
	float x3 = x2 * x;
	float x4 = x3 * x;
	float x5 = x4 * x;

	float y = xy.y;
	float y2 = y * y;
	float y3 = y2 * y;
	float y4 = y3 * x;
	float y5 = y4 * x;

	float4 color = 0;

	color += a00 + a10 * x + a20 * x2 + a30 * x3 + a40 * x4 + a50 * x5;
	 
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