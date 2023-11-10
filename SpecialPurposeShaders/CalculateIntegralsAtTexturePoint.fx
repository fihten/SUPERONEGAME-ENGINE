Texture2D<uint4> tex;
RWStructuredBuffer<uint> Integrals;
RWStructuredBuffer<uint> Variances;

int radius0;
int radius1;

int leftX;
int rightX;

int bottomY;
int topY;

int x0;
int y0;

float angle0;
float scale0;

float angle1;
float scale1;

[numthreads(16, 16, 4)]
void cs_integral(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	int y = dispatchThreadID.y;
	int integralIndex = dispatchThreadID.z;

	if (leftX + x > rightX)
		return;
	if (bottomY + y > topY)
		return;

	int radius = radius0 + integralIndex;
	if (radius > radius1)
		return;

	x = leftX + x + x0;
	y = bottomY + y + y0;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint mips = 0;
	tex.GetDimensions(mip, width, height, mips);

	if (x < 0)
		return;
	if (x > width - 1)
		return;

	if (y < 0)
		return;
	if (y > height - 1)
		return;

	float2x2 m;
	m[0][0] = scale0 * cos(angle0); m[0][1] = scale0 * sin(angle0);
	m[1][0] = -scale1 * sin(angle1); m[1][1] = scale1 * cos(angle1);

	float det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
	float2x2 mInv;
	mInv[0][0] = m[1][1]; mInv[0][1] = -m[0][1];
	mInv[1][0] = -m[1][0]; mInv[1][1] = m[0][0];
	mInv /= det;

	float2 r = float2(x - x0, y - y0);
	float2 r_ = mul(r, mInv);
	if (dot(r_, r_) > radius)
		return;

	float4 color = tex[int2(x, y)];
	InterlockedAdd(Integrals[4 * integralIndex + 0], color.x);
	InterlockedAdd(Integrals[4 * integralIndex + 1], color.y);
	InterlockedAdd(Integrals[4 * integralIndex + 2], color.z);
	InterlockedAdd(Integrals[4 * integralIndex + 3]), 1);
}

[numthreads(16, 16, 4)]
void cs_variance(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	int y = dispatchThreadID.y;
	int varianceIndex = dispatchThreadID.z;

	if (leftX + x > rightX)
		return;
	if (bottomY + y > topY)
		return;

	int radius = radius0 + varianceIndex;
	if (radius > radius1)
		return;

	x = leftX + x + x0;
	y = bottomY + y + y0;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint mips = 0;
	tex.GetDimensions(mip, width, height, mips);

	if (x < 0)
		return;
	if (x > width - 1)
		return;

	if (y < 0)
		return;
	if (y > height - 1)
		return;

	float2x2 m;
	m[0][0] = scale0 * cos(angle0); m[0][1] = scale0 * sin(angle0);
	m[1][0] = -scale1 * sin(angle1); m[1][1] = scale1 * cos(angle1);

	float det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
	float2x2 mInv;
	mInv[0][0] = m[1][1]; mInv[0][1] = -m[0][1];
	mInv[1][0] = -m[1][0]; mInv[1][1] = m[0][0];
	mInv /= det;

	float2 r = float2(x - x0, y - y0);
	float2 r_ = mul(r, mInv);
	if (dot(r_, r_) > radius)
		return;

	uint area = Integrals[4 * varianceIndex + 3];
	uint meanValue0 = Integrals[4 * varianceIndex + 0] / area;
	uint meanValue1 = Integrals[4 * varianceIndex + 1] / area;
	uint meanValue2 = Integrals[4 * varianceIndex + 2] / area;

	float4 color = tex[int2(x, y)];
	uint value0 = color.x;
	uint value1 = color.y;
	uint value2 = color.z;

	InterlockedAdd(Variances[4 * varianceIndex + 0], (value0 - meanValue0) * (value0 - meanValue0));
	InterlockedAdd(Variances[4 * varianceIndex + 1], (value1 - meanValue1) * (value1 - meanValue1));
	InterlockedAdd(Variances[4 * varianceIndex + 2], (value2 - meanValue2) * (value2 - meanValue2));
	InterlockedAdd(Variances[4 * varianceIndex + 3], 1);
}

technique11 CalculateIntegralsAtTexturePoint
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_integral()));
	}
};

technique11 CalculateVarianceWithinAreaOfIntegration
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_variance()));
	}
};