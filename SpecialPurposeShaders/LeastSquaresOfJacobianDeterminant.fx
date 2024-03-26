Texture2DArray<uint> AA;
Texture2DArray<uint> AB;
Texture2DArray<uint> BB;

Texture2DArray<uint> AAfraction;
Texture2DArray<uint> ABfraction;
Texture2DArray<uint> BBfraction;

Texture2DArray<uint> maxA;
Texture2DArray<uint> maxB;

StructuredBuffer<uint> mapAtoB;

int width;
int height;
int texturesCount;

float angle0;
float scale0;

float angle1;
float scale1;

int offsetX;
int offsetY;

int cellRadius;
int unitX;
int unitY;

RWTexture2DArray<float> error;
RWTexture2DArray<uint4> AtoB;

[numthreads(16, 16, 4)]
void cs_clear(uint3 dispatchThreadID : SV_DispatchThreadID)
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

	uint3 location = uint3(x, y, indexOfPhoto);
	error[location] = 0xFFFFFFFF;
	AtoB[location] = uint4(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF);
}

[numthreads(16, 16, 4)]
void cs_error(uint3 dispatchThreadID : SV_DispatchThreadID)
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
	
	float AA_ = AA[locationIn].r;
	float AAfraction_ = AAfraction[locationIn].r;
	AA_ = AA_ + AAfraction_ / 1000000.0f;

	float BB_ = BB[locationIn].r;
	float BBfraction_ = BBfraction[locationIn].r;
	BB_ = BB_ + BBfraction_ / 1000000.0f;

	float AB_ = AB[locationIn].r;
	float ABfraction_ = ABfraction[locationIn].r;
	AB_ = AB_ + ABfraction_ / 1000000.0f;

	float maxA_ = maxA[locationIn].r;
	maxA_ /= 255.0f;

	float maxB_ = maxB[locationIn].r;
	maxB_ /= 255.0f;

	AA_ /= maxA_ * maxA_;
	BB_ /= maxB_ * maxB_;
	AB_ /= maxA_ * maxB_;

	float J = AB_ / AA_;
	float err = J * J * AA_ + BB_ - 2 * J * AB_;

	int cellDiameterX = (2 * cellRadius + 1) * unitX;
	int cellDiameterY = (2 * cellRadius + 1) * unitY;

	int x_ = ((float)x + 0.5f) * cellDiameterX;
	int y_ = ((float)y + 0.5f) * cellDiameterY;

	int2 posInA = int2(x_, y_);

	float2x2 m;
	m[0][0] = scale0 * cos(angle0); m[0][1] = scale0 * sin(angle0);
	m[1][0] = -scale1 * sin(angle1); m[1][1] = scale1 * cos(angle1);

	int2 posInB = int2(x_ - offsetX, y_ - offsetY);
	posInB = max(mul(posInB, m), int2(0, 0));

	uint4 mapping;
	mapping.x = (posInA.x << 16) | (posInA.y & 0xffff);
	mapping.y = indexOfPhoto;
	mapping.z = (posInB.x << 16) | (posInB.y & 0xffff);
	mapping.w = mapAtoB[indexOfPhoto];

	uint3 locationOut = locationIn;
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