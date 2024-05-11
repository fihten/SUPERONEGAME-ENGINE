#include "MapToTexArray.hlsl"

Texture2DArray<uint> photosIntegralsAx;
Texture2DArray<uint> photosIntegralsAy;
Texture2DArray<uint> photosIntegralsAz;

Texture2DArray<uint> photosIntegralsBx;
Texture2DArray<uint> photosIntegralsBy;
Texture2DArray<uint> photosIntegralsBz;

RWTexture2DArray<uint> AA;
RWTexture2DArray<uint> AB;
RWTexture2DArray<uint> BB;

RWTexture2DArray<uint> AAfraction;
RWTexture2DArray<uint> ABfraction;
RWTexture2DArray<uint> BBfraction;

RWTexture2DArray<uint> maxA;
RWTexture2DArray<uint> maxB;

RWTexture2DArray<uint> pointsCountAA;
RWTexture2DArray<uint> pointsCountAB;
RWTexture2DArray<uint> pointsCountBB;

int widthAreal;
int heightAreal;

int widthB;
int heightB;

int widthAA;
int heightAA;

int widthAB;
int heightAB;

int widthABreal;
int heightABreal;

int widthBB;
int heightBB;

int texturesCount;

int2 cellDimension;
int2 offsetRange;
int2 offset0;

int radius;
int indexOfA;

[numthreads(16, 16, 4)]
void cs_clear_AA_maxA(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= widthAA)
		return;

	int y = dispatchThreadID.y;
	if (y >= heightAA)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 location = uint3(x, y, indexOfPhoto);
	AA[location].r = 0;
	AAfraction[location].r = 0;
	maxA[location].r = 0;
	pointsCountAA[location].r = 0;
}

[numthreads(16, 16, 4)]
void cs_clear_BB_maxB(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= widthBB)
		return;

	int y = dispatchThreadID.y;
	if (y >= heightBB)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 location = uint3(x, y, indexOfPhoto);
	BB[location].r = 0;
	BBfraction[location].r = 0;
	maxB[location].r = 0;
	pointsCountBB[location].r = 0;
}

[numthreads(16, 16, 4)]
void cs_clear_AB(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= widthAB)
		return;

	int y = dispatchThreadID.y;
	if (y >= heightAB)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 location = uint3(x, y, indexOfPhoto);

	uint2 dims0 = uint2(widthAB, heightAB);
	uint2 dims1 = uint2(widthABreal, heightABreal);
	location = mapToTexArray(location, dims0, dims1);

	AB[location].r = 0;
	ABfraction[location].r = 0;
	pointsCountAB[location].r = 0;
}

[numthreads(16, 16, 4)]
void cs_AA_maxA(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= widthAreal)
		return;

	int y = dispatchThreadID.y;
	if (y >= heightAreal)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 locationIn = uint3(x, y, indexOfPhoto);
	uint3 colorA = float3(
		photosIntegralsAx[locationIn].r,
		photosIntegralsAy[locationIn].r,
		photosIntegralsAz[locationIn].r
		);

	uint uiAA = dot(colorA, colorA);
	float fAA = (float)(uiAA) / 65025.0f;

	uiAA = floor(fAA);
	uint uiAAfraction = 1000000 * (fAA - uiAA);

	uint maxA_ = max(colorA.x, max(colorA.y, colorA.z));

	int diameter = 2 * radius + 1;
	int2 m = int2(x, y);
	int2 n = m % cellDimension;
	m = m / (diameter * cellDimension);

	uint3 locationOut;
	locationOut.xy = m * cellDimension + n;
	locationOut.z = indexOfPhoto;

	uint originalValue = 0;
	InterlockedAdd(AA[locationOut].r, uiAA, originalValue);
	InterlockedAdd(AAfraction[locationOut].r, uiAAfraction, originalValue);
	InterlockedMax(maxA[locationOut].r, maxA_, originalValue);
	InterlockedAdd(pointsCountAA[locationOut].r, 3, originalValue);
}

[numthreads(16, 16, 4)]
void cs_BB_maxB(uint3 dispatchThreadID : SV_DispatchThreadID)
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

	uint3 locationIn = uint3(x, y, indexOfPhoto);
	uint3 colorB = float3(
		photosIntegralsBx[locationIn].r,
		photosIntegralsBy[locationIn].r,
		photosIntegralsBz[locationIn].r
		);
	
	uint uiBB = dot(colorB, colorB);
	float fBB = (float)(uiBB) / 65025.0f;

	uiBB = floor(fBB);
	uint uiBBfraction = 1000000 * (fBB - uiBB);
	uint maxB_ = max(colorB.x, max(colorB.y, colorB.z));

	int diameter = 2 * radius + 1;
	uint3 locationOut = uint3(x / diameter, y / diameter, indexOfPhoto);

	uint originalValue = 0;
	InterlockedAdd(BB[locationOut].r, uiBB, originalValue);
	InterlockedAdd(BBfraction[locationOut].r, uiBBfraction, originalValue);
	InterlockedMax(maxB[locationOut].r, maxB_, originalValue);
	InterlockedAdd(pointsCountBB[locationOut].r, 3, originalValue);
}

[numthreads(16, 16, 4)]
void cs_AB(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	int x = dispatchThreadID.x;
	int y = dispatchThreadID.y;

	int2 m = int2(x, y);
	m += offset0;
	int2 n = m % offsetRange;
	n -= offset0;
	uint3 locationInA;
	locationInA.z = indexOfA;
	locationInA.xy = m / offsetRange;
	locationInA.xy *= cellDimension;
	locationInA.xy += n;

	if (locationInA.x < 0)
		return;
	if (locationInA.x >= widthAreal)
		return;
	if (locationInA.y < 0)
		return;
	if (locationInA.y >= heightAreal)
		return;

	uint3 colorA = float3(
		photosIntegralsAx[locationInA].r,
		photosIntegralsAy[locationInA].r,
		photosIntegralsAz[locationInA].r
		);

	uint3 locationInB;
	locationInB.z = indexOfPhoto;
	locationInB.xy = m / offsetRange;

	uint3 colorB = float3(
		photosIntegralsBx[locationInB].r,
		photosIntegralsBy[locationInB].r,
		photosIntegralsBz[locationInB].r
		);

	uint uiAB = dot(colorA, colorB);
	float fAB = (float)(uiAB) / 65025.0f;

	uiAB = floor(fAB);
	uint uiABfraction = 1000000 * (fAB - uiAB);

	int diameter = 2 * radius + 1;
	uint3 locationOutAB;
	locationOutAB.z = indexOfPhoto;
	locationOutAB.xy = m / (diameter * offsetRange);
	locationOutAB.xy *= offsetRange;
	locationOutAB.xy += n;

	if (locationOutAB.x < 0)
		return;
	if (locationOutAB.y < 0)
		return;

	uint2 dims0 = uint2(widthAB, heightAB);
	uint2 dims1 = uint2(widthABreal, heightABreal);
	locationOutAB = mapToTexArray(locationOutAB, dims0, dims1);

	uint originalValue = 0;
	InterlockedAdd(AB[locationOutAB].r, uiAB, originalValue);
	InterlockedAdd(ABfraction[locationOutAB].r, uiABfraction, originalValue);
	InterlockedAdd(pointsCountAB[locationOutAB].r, 3, originalValue);
}

technique11 MakeOperationsOnGridIntegrals
{
	pass clear_AA_maxA
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_clear_AA_maxA()));
	}
	pass clear_BB_maxB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_clear_BB_maxB()));
	}
	pass clear_AB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_clear_AB()));
	}
	pass AA_maxA
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_AA_maxA()));
	}
	pass BB_maxB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_BB_maxB()));
	}
	pass AB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_AB()));
	}
};