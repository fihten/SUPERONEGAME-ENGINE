Texture2DArray<uint4> photosIntegralsA;
Texture2DArray<uint4> photosIntegralsB;

RWTexture2DArray<uint> AA;
RWTexture2DArray<uint> AB;
RWTexture2DArray<uint> BB;

RWTexture2DArray<uint> AAfraction;
RWTexture2DArray<uint> ABfraction;
RWTexture2DArray<uint> BBfraction;

RWTexture2DArray<uint> maxA;
RWTexture2DArray<uint> maxB;

StructuredBuffer<uint> mapAtoB;

int width;
int height;
int texturesCount;

int cellRadius;

[numthreads(16, 16, 4)]
void cs_clear_AA_maxA(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int cellDiameter = 2 * cellRadius + 1;
	int cellsAlongX = ceil((float)width / cellDiameter);
	int cellsAlongY = ceil((float)height / cellDiameter);

	int x = dispatchThreadID.x;
	if (x >= cellsAlongX)
		return;

	int y = dispatchThreadID.y;
	if (y >= cellsAlongY)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 location = uint3(x, y, indexOfPhoto);
	AA[location].r = 0;
	AAfraction[location].r = 0;
	maxA[location].r = 0;
}

[numthreads(16, 16, 4)]
void cs_clear_BB_AB_maxB(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int cellDiameter = 2 * cellRadius + 1;
	int cellsAlongX = ceil((float)width / cellDiameter);
	int cellsAlongY = ceil((float)height / cellDiameter);

	int x = dispatchThreadID.x;
	if (x >= cellsAlongX)
		return;

	int y = dispatchThreadID.y;
	if (y >= cellsAlongY)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 location = uint3(x, y, indexOfPhoto);
	BB[location].r = 0;
	BBfraction[location].r = 0;
	AB[location].r = 0;
	ABfraction[location].r = 0;
	maxB[location].r = 0;
}

[numthreads(16, 16, 4)]
void cs_AA_maxA(uint3 dispatchThreadID : SV_DispatchThreadID)
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
	uint4 colorA = photosIntegralsA[locationIn].xyzw;

	uint uiAA = dot(colorA, colorA);
	float fAA = (float)(uiAA) / 65025.0f;

	uiAA = floor(fAA);
	uint uiAAfraction = 1000000 * (fAA - uiAA);

	uint maxA_ = max(colorA.x, max(colorA.y, colorA.z));

	int cellDiameter = 2 * cellRadius + 1;
	x = x / cellDiameter;
	y = y / cellDiameter;

	uint3 locationOut = uint3(x, y, indexOfPhoto);

	uint originalValue = 0;
	InterlockedAdd(AA[locationOut].r, uiAA, originalValue);
	InterlockedAdd(AAfraction[locationOut].r, uiAAfraction, originalValue);
	InterlockedMax(maxA[locationOut].r, maxA_, originalValue);
}

[numthreads(16, 16, 4)]
void cs_BB_AB_maxB(uint3 dispatchThreadID : SV_DispatchThreadID)
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

	indexOfPhoto = mapAtoB[indexOfPhoto];

	uint3 locationIn = uint3(x, y, indexOfPhoto);
	uint4 colorA = photosIntegralsA[locationIn].xyzw;
	uint4 colorB = photosIntegralsB[locationIn].xyzw;
	
	uint uiBB = dot(colorB, colorB);
	float fBB = (float)(uiBB) / 65025.0f;

	uiBB = floor(fBB);
	uint uiBBfraction = 1000000 * (fBB - uiBB);

	uint uiAB = dot(colorA, colorB);
	float fAB = (float)(uiAB) / 65025.0f;

	uiAB = floor(fAB);
	uint uiABfraction = 1000000 * (fAB - uiAB);

	uint maxB_ = max(colorB.x, max(colorB.y, colorB.z));

	int cellDiameter = 2 * cellRadius + 1;
	x = x / cellDiameter;
	y = y / cellDiameter;
	
	uint3 locationOut = uint3(x, y, indexOfPhoto);
	
	uint originalValue = 0;
	InterlockedAdd(BB[locationOut].r, uiBB, originalValue);
	InterlockedAdd(BBfraction[locationOut].r, uiBBfraction, originalValue);
	InterlockedAdd(AB[locationOut].r, uiAB, originalValue);
	InterlockedAdd(ABfraction[locationOut].r, uiABfraction, originalValue);
	InterlockedMax(maxB[locationOut].r, maxB_, originalValue);
}

technique11 MakeOperationsOnGridIntegrals
{
	pass clear_AA_maxA
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_clear_AA_maxA()));
	}
	pass clear_BB_AB_maxB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_clear_BB_AB_maxB()));
	}
	pass AA_maxA
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_AA_maxA()));
	}
	pass BB_AB_maxB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_BB_AB_maxB()));
	}
};