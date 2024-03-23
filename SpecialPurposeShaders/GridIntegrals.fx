Texture2DArray<float4> photos;
RWTexture2DArray<uint4> photosIntegrals;

int width;
int height;
int texturesCount;

float angle0;
float scale0;

float angle1;
float scale1;

int offsetX;
int offsetY;

int cellDimensionX;
int cellDimensionY;

[numthreads(16, 16, 4)]
void cs_clear(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int cellsAlongX = ceil((float)width / cellDimensionX);
	int cellsAlongY = ceil((float)height / cellDimensionY);

	int x = dispatchThreadID.x;
	if (x >= cellsAlongX)
		return;

	int y = dispatchThreadID.y;
	if (y >= cellsAlongY)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	photosIntegrals[int3(x, y, indexOfPhoto)].xyzw = uint4(0, 0, 0, 0);
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

	float det = m00 * m11 - m01 * m10;
	float2x2 mInv = float2x2(
		m11, -m01,
		-m10, m00
		);
	mInv /= det;

	float2 r = float2(x, y);
	float2 r_ = mul(r, mInv);
	r_.x += offsetX;
	r_.y += offsetY;

	int cellIndexX = floor(r_.x / cellDimensionX);
	int cellIndexY = floor(r_.y / cellDimensionY);

	int cellsAlongX = ceil((float)width / cellDimensionX);
	int cellsAlongY = ceil((float)height / cellDimensionY);

	if (cellIndexX < 0 || cellIndexX >= cellsAlongX)
		return;
	if (cellIndexY < 0 || cellIndexY >= cellsAlongY)
		return;

	uint3 color = round(255 * srgbToRaw(photos[int3(x, y, indexOfPhoto)].rgb));

	uint originalValue;
	InterlockedAdd(
		photosIntegrals[int3(cellIndexX, cellIndexY, indexOfPhoto)].x,
		color.x, originalValue);
	InterlockedAdd(
		photosIntegrals[int3(cellIndexX, cellIndexY, indexOfPhoto)].y,
		color.y, originalValue);
	InterlockedAdd(
		photosIntegrals[int3(cellIndexX, cellIndexY, indexOfPhoto)].z,
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