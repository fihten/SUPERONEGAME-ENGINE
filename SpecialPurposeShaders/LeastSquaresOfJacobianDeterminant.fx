Texture2DArray<uint> AA;
Texture2DArray<uint> AB;
Texture2DArray<uint> BB;

Texture2DArray<uint> AAfraction;
Texture2DArray<uint> ABfraction;
Texture2DArray<uint> BBfraction;

Texture2DArray<uint> maxA;
Texture2DArray<uint> maxB;

StructuredBuffer<uint> mapAtoB;

int widthAA;
int heightAA;

int widthAB;
int heightAB;

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

RWTexture2DArray<float> error;
RWTexture2DArray<uint4> AtoB;

[numthreads(16, 16, 4)]
void cs_clear(uint3 dispatchThreadID : SV_DispatchThreadID)
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
	error[location] = 0xFFFFFFFF;
	AtoB[location] = uint4(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
}

[numthreads(16, 16, 4)]
void cs_error(uint3 dispatchThreadID : SV_DispatchThreadID)
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

	uint3 locationInAB = uint3(x, y, indexOfPhoto);
	
	float AB_ = AB[locationInAB].r;
	float ABfraction_ = ABfraction[locationInAB].r;
	AB_ = AB_ + ABfraction_ / 1000000.0f;

	uint3 locationInAA = uint3(x, y, indexOfPhoto);
	locationInAA.xy += offset0;
	int2 n = locationInAA.xy % offsetRange;
	n -= offset0;
	locationInAA.xy /= offsetRange;
	locationInAA.xy *= cellDimension;
	locationInAA.xy += n; 

	float AA_ = AA[locationInAA].r;
	float AAfraction_ = AAfraction[locationInAA].r;
	AA_ = AA_ + AAfraction_ / 1000000.0f;

	uint3 locationInBB = uint3(x, y, indexOfPhoto);
	locationInBB.xy += offset0;
	locationInBB.xy /= offsetRange;

	float BB_ = BB[locationInBB].r;
	float BBfraction_ = BBfraction[locationInBB].r;
	BB_ = BB_ + BBfraction_ / 1000000.0f;

	float maxA_ = maxA[locationInAA].r;
	maxA_ /= 255.0f;

	float maxB_ = maxB[locationInBB].r;
	maxB_ /= 255.0f;

	AA_ /= maxA_ * maxA_;
	BB_ /= maxB_ * maxB_;
	AB_ /= maxA_ * maxB_;

	float J = AB_ / AA_;
	float err = J * J * AA_ + BB_ - 2 * J * AB_;

	int2 posInA = locationInAA.xy + 0.5 * cellDimension;

	float2x2 m;
	m[0][0] = scale0 * cos(angle0); m[0][1] = scale0 * sin(angle0);
	m[1][0] = -scale1 * sin(angle1); m[1][1] = scale1 * cos(angle1);

	int2 posInB = posInA;
	posInB = max(mul(posInB, m), int2(0, 0));

	uint4 mapping;
	mapping.x = (posInA.x << 16) | (posInA.y & 0xffff);
	mapping.y = indexOfPhoto;
	mapping.z = (posInB.x << 16) | (posInB.y & 0xffff);
	mapping.w = mapAtoB[indexOfPhoto];

	uint3 locationOut = locationInAA;
	error[locationOut] = err;
	AtoB[locationOut] = mapping;
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
};