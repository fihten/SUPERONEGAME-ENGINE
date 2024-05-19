#include "MapToTexArray.hlsl"
#include "constants.hlsl"

Texture2DArray<uint> AA;
Texture2DArray<uint> AB;
Texture2DArray<uint> BB;

Texture2DArray<uint> AAfraction;
Texture2DArray<uint> ABfraction;
Texture2DArray<uint> BBfraction;

Texture2DArray<uint> maxA;
Texture2DArray<uint> maxB;

int widthAA;
int heightAA;

int widthAB;
int heightAB;

int widthABreal;
int heightABreal;

int widthBB;
int heightBB;

int texturesCount;

float angle0;
float scale0;

float angle1;
float scale1;

int radius;
int2 cellDimension;
int2 offsetRange;
int2 offset0;

int indexOfA;

RWTexture2DArray<uint> error;
Texture2DArray<uint> errorIn;
RWTexture2DArray<uint> AtoBx;
RWTexture2DArray<uint> AtoBy;
RWTexture2DArray<uint> AtoBz;
RWTexture2DArray<uint> AtoBw;

[numthreads(16, 16, 4)]
void cs_clear(uint3 dispatchThreadID : SV_DispatchThreadID)
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
	error[location].r = 0xFFFFFFFF;
	AtoBx[location].r = 0xFFFFFFFF;
	AtoBy[location].r = 0xFFFFFFFF;
	AtoBz[location].r = 0xFFFFFFFF;
	AtoBw[location].r = 0xFFFFFFFF;
}

[numthreads(16, 16, 4)]
void cs_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;
	if (indexOfPhoto == indexOfA)
		return;

	int x = dispatchThreadID.x;
	int y = dispatchThreadID.y;

	uint3 locationInAA = uint3(x, y, indexOfA);
	locationInAA.xy += offset0;
	int2 n = locationInAA.xy % offsetRange;
	locationInAA.xy /= offsetRange;
	locationInAA.xy *= cellDimension;
	locationInAA.xy += n - offset0;

	if (locationInAA.x >= widthAA)
		return;
	if (locationInAA.y >= heightAA)
		return;

	uint3 locationInAB = uint3(x, y, indexOfPhoto);
	
	uint2 dims0 = uint2(widthAB, heightAB);
	uint2 dims1 = uint2(widthABreal, heightABreal);
	locationInAB = mapToTexArray(locationInAB, dims0, dims1);

	float AB_ = AB[locationInAB].r;
	float ABfraction_ = ABfraction[locationInAB].r;
	AB_ = AB_ + ABfraction_ / (acc * acc);

	float AA_ = AA[locationInAA].r;
	float AAfraction_ = AAfraction[locationInAA].r;
	AA_ = AA_ + AAfraction_ / (acc * acc);

	uint3 locationInBB = uint3(x, y, indexOfPhoto);
	locationInBB.xy += offset0;
	locationInBB.xy /= offsetRange;

	float BB_ = BB[locationInBB].r;
	float BBfraction_ = BBfraction[locationInBB].r;
	BB_ = BB_ + BBfraction_ / (acc * acc);

	float maxA_ = maxA[locationInAA].r;
	maxA_ /= 255.0f;

	float maxB_ = maxB[locationInBB].r;
	maxB_ /= 255.0f;

	AA_ /= maxA_ * maxA_;
	BB_ /= maxB_ * maxB_;
	AB_ /= maxA_ * maxB_;

	float J = AB_ / AA_;
	float err = J * J * AA_ + BB_ - 2 * J * AB_;
	int diameter = 2 * radius + 1;
	int pointsCount = 3 * diameter * diameter;
	err /= pointsCount;

	uint3 locationOut = locationInBB;

	uint originalValue = 0;
	InterlockedMin(error[locationOut].r, (uint)(1000000 * err), originalValue);
}

[numthreads(16, 16, 4)]
void cs_mapping(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;
	if (indexOfPhoto == indexOfA)
		return;

	int x = dispatchThreadID.x;
	int y = dispatchThreadID.y;

	uint3 locationInAA = uint3(x, y, indexOfA);
	locationInAA.xy += offset0;
	int2 n = locationInAA.xy % offsetRange;
	locationInAA.xy /= offsetRange;
	locationInAA.xy *= cellDimension;
	locationInAA.xy += n - offset0;

	if (locationInAA.x >= widthAA)
		return;
	if (locationInAA.y >= heightAA)
		return;

	uint3 locationInAB = uint3(x, y, indexOfPhoto);

	uint2 dims0 = uint2(widthAB, heightAB);
	uint2 dims1 = uint2(widthABreal, heightABreal);
	locationInAB = mapToTexArray(locationInAB, dims0, dims1);

	float AB_ = AB[locationInAB].r;
	float ABfraction_ = ABfraction[locationInAB].r;
	AB_ = AB_ + ABfraction_ / (acc * acc);

	float AA_ = AA[locationInAA].r;
	float AAfraction_ = AAfraction[locationInAA].r;
	AA_ = AA_ + AAfraction_ / (acc * acc);

	uint3 locationInBB = uint3(x, y, indexOfPhoto);
	locationInBB.xy += offset0;
	locationInBB.xy /= offsetRange;

	float BB_ = BB[locationInBB].r;
	float BBfraction_ = BBfraction[locationInBB].r;
	BB_ = BB_ + BBfraction_ / (acc * acc);

	float maxA_ = maxA[locationInAA].r;
	maxA_ /= 255.0f;

	float maxB_ = maxB[locationInBB].r;
	maxB_ /= 255.0f;

	AA_ /= maxA_ * maxA_;
	BB_ /= maxB_ * maxB_;
	AB_ /= maxA_ * maxB_;

	float J = AB_ / AA_;
	float err = J * J * AA_ + BB_ - 2 * J * AB_;
	int diameter = 2 * radius + 1;
	int pointsCount = 3 * diameter * diameter;
	err /= pointsCount;
	if (errorIn[locationInBB].r != (uint)(1000000 * err))
		return;

	int2 posInA = locationInBB.xy * cellDimension * diameter + n - offset0 + (radius * cellDimension);

	float2x2 m;
	m[0][0] = scale0 * cos(angle0); m[0][1] = scale0 * sin(angle0);
	m[1][0] = -scale1 * sin(angle1); m[1][1] = scale1 * cos(angle1);

	float2 posInB = locationInBB.xy * diameter * cellDimension + (radius * cellDimension);
	posInB = max(mul(posInB, m), float2(0, 0));

	uint4 mapping;
	mapping.x = (posInA.x << 16) | (posInA.y & 0xffff);
	mapping.y = indexOfA;
	mapping.z = ((uint)(posInB.x) << 16) | ((uint)(posInB.y) & 0xffff);
	mapping.w = indexOfPhoto;

	uint3 locationOut = locationInBB;
	uint originalValue = 0;
	InterlockedExchange(AtoBx[locationOut].r, mapping.x, originalValue);
	InterlockedExchange(AtoBy[locationOut].r, mapping.y, originalValue);
	InterlockedExchange(AtoBz[locationOut].r, mapping.z, originalValue);
	InterlockedExchange(AtoBw[locationOut].r, mapping.w, originalValue);
}

technique11 Clear
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_clear()));
	}
};

technique11 ApplyLeastSquareMethod
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_error()));
	}
	pass P1
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_mapping()));
	}
};