#include "ColorSpace.hlsl"

Texture2DArray<float4> photos;
RWTexture2DArray<uint> photosIntegralsX;
RWTexture2DArray<uint> photosIntegralsY;
RWTexture2DArray<uint> photosIntegralsZ;

int width;
int height;
int texturesCount;

int widthB;
int heightB;

float angle0;
float scale0;

float angle1;
float scale1;

int cellDimensionX;
int cellDimensionY;

[numthreads(16, 16, 4)]
void cs_clear(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= widthB)
		return;

	int y = dispatchThreadID.y;
	if (y >= heightB)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 locationOut = uint3(x, y, indexOfPhoto);
	photosIntegralsX[locationOut].r = 0;
	photosIntegralsY[locationOut].r = 0;
	photosIntegralsZ[locationOut].r = 0;
}

[numthreads(16, 16, 4)]
void cs(uint3 dispatchThreadID : SV_DispatchThreadID)
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

	float2x2 m;
	m[0][0] = scale0 * cos(angle0); m[0][1] = scale0 * sin(angle0);
	m[1][0] = -scale1 * sin(angle1); m[1][1] = scale1 * cos(angle1);

	float det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
	float2x2 mInv = float2x2(
		m[1][1], -m[0][1],
		-m[1][0], m[0][0]
		);
	mInv /= det;

	float2 r = float2(x, y);
	float2 r_ = mul(r, mInv);

	int cellIndexX = floor(r_.x / cellDimensionX);
	int cellIndexY = floor(r_.y / cellDimensionY);

	if (cellIndexX < 0 || cellIndexX >= widthB)
		return;
	if (cellIndexY < 0 || cellIndexY >= heightB)
		return;

	uint3 locationIn = uint3(x, y, indexOfPhoto);
	uint3 color = round(255 * srgbToRaw(photos[locationIn].rgb));

	uint originalValue;
	uint3 locationOut = uint3(cellIndexX, cellIndexY, indexOfPhoto);
	InterlockedAdd(
		photosIntegralsX[locationOut].r,
		color.x, originalValue);
	InterlockedAdd(
		photosIntegralsY[locationOut].r,
		color.y, originalValue);
	InterlockedAdd(
		photosIntegralsZ[locationOut].r,
		color.z, originalValue);
}

technique11 ClearGridIntegrals
{
	pass P0
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
		SetComputeShader(CompileShader(cs_5_0, cs()));
	}
};