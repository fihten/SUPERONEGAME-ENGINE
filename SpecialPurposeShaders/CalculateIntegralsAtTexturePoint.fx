#include "constants.hlsl"
#include "ColorSpace.hlsl"

Texture2D<float4> tex;
RWStructuredBuffer<uint> Integrals;

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

int cellDimensionX;
int cellDimensionY;
int radius;

[numthreads(32, 32, 1)]
void cs_integral(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	int y = dispatchThreadID.y;

	if (leftX + x > rightX)
		return;
	if (bottomY + y > topY)
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
	
	r_.x = round(r_.x / cellDimensionX);
	if (r_.x < -radius)
		return;
	if (r_.x > radius)
		return;
	
	r_.y = round(r_.y / cellDimensionY);
	if (r_.y < -radius)
		return;
	if (r_.y > radius)
		return;

	uint integralIndex = (r_.y + radius) * (2 * radius + 1) + r_.x + radius;

	uint3 color = round(255 * srgbToRaw(tex[int2(x, y)].rgb));
	InterlockedAdd(Integrals[4 * integralIndex + 0], color.x);
	InterlockedAdd(Integrals[4 * integralIndex + 1], color.y);
	InterlockedAdd(Integrals[4 * integralIndex + 2], color.z);
	InterlockedAdd(Integrals[4 * integralIndex + 3], 1);
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
