#include "ColorSpace.hlsl"

Texture2DArray<float4> photos;

RWTexture2DArray<uint> photosIntegralsXho;
RWTexture2DArray<uint> photosIntegralsYho;
RWTexture2DArray<uint> photosIntegralsZho;

Texture2DArray<uint> photosIntegralsXhi;
Texture2DArray<uint> photosIntegralsYhi;
Texture2DArray<uint> photosIntegralsZhi;

RWTexture2DArray<uint> photosIntegralsX;
RWTexture2DArray<uint> photosIntegralsY;
RWTexture2DArray<uint> photosIntegralsZ;

int width;
int height;
int texturesCount;

int cellDimensionX;
int cellDimensionY;

[numthreads(16, 16, 4)]
void cs_clear_h(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= width - cellDimensionX + 1)
		return;

	int y = dispatchThreadID.y;
	if (y >= height)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 locationOut = uint3(x, y, texturesCount);
	photosIntegralsXho[locationOut].r = 0;
	photosIntegralsYho[locationOut].r = 0;
	photosIntegralsZho[locationOut].r = 0;
}

[numthreads(16, 16, 4)]
void cs_clear(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= width - cellDimensionX + 1)
		return;

	int y = dispatchThreadID.y;
	if (y >= height - cellDimensionY + 1)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 locationOut = uint3(x, y, texturesCount);
	photosIntegralsX[locationOut].r = 0;
	photosIntegralsY[locationOut].r = 0;
	photosIntegralsZ[locationOut].r = 0;
}

[numthreads(16, 16, 4)]
void cs_h(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= width)
		return;

	int y = dispatchThreadID.y;
	if (y >= height)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 locationIn = uint3(x, y, indexOfPhoto);
	uint3 color = round(255 * srgbToRaw(photos[locationIn].rgb));

	int minX = max(0, x - cellDimensionX + 1);
	int maxX = min(width - cellDimensionX, x);
	for (x = minX; x <= maxX; x++)
	{
		uint3 locationOut = uint3(x, y, indexOfPhoto);

		uint originalValue;
		InterlockedAdd(
			photosIntegralsXho[locationOut].r,
			color.x, originalValue);
		InterlockedAdd(
			photosIntegralsYho[locationOut].r,
			color.y, originalValue);
		InterlockedAdd(
			photosIntegralsZho[locationOut].r,
			color.z, originalValue);
	}
}

[numthreads(16, 16, 4)]
void cs(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= width - cellDimensionX + 1)
		return;

	int y = dispatchThreadID.y;
	if (y >= height)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 locationIn = uint3(x, y, indexOfPhoto);
	uint colorX = photosIntegralsXhi[locationIn].r;
	uint colorY = photosIntegralsYhi[locationIn].r;
	uint colorZ = photosIntegralsZhi[locationIn].r;

	int minY = max(0, y - cellDimensionY + 1);
	int maxY = min(height - cellDimensionY, y);
	for (y = minY; y <= maxY; y++)
	{
		uint3 locationOut = uint3(x, y, indexOfPhoto);

		uint originalValue;
		InterlockedAdd(
			photosIntegralsX[locationOut].r,
			colorX, originalValue);
		InterlockedAdd(
			photosIntegralsY[locationOut].r,
			colorY, originalValue);
		InterlockedAdd(
			photosIntegralsZ[locationOut].r,
			colorZ, originalValue);
	}
}

technique11 ClearGridIntegrals
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_clear_h()));
	}
	pass P1
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_clear()));
	}
};

technique11 CalculateGridIntegrals
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_h()));
	}
	pass P1
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs()));
	}
};