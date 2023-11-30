#include "ColorSpace.hlsl"

Texture2D<float4> tex;

RWStructuredBuffer<int> fourierCoefficientsAtCos_R;
RWStructuredBuffer<int> fourierCoefficientsAtCos_G;
RWStructuredBuffer<int> fourierCoefficientsAtCos_B;

RWStructuredBuffer<int> fourierCoefficientsAtSin_R;
RWStructuredBuffer<int> fourierCoefficientsAtSin_G;
RWStructuredBuffer<int> fourierCoefficientsAtSin_B;

int radius;
int thickness;

int width;
int height;

int numberOfElementsInFourierRow;

int2 r0;

// Coefficients near cos

[numthreads(16,16,4)]
void cs_cos_r(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int index = dispatchThreadID.z;
	if (index > numberOfElementsInFourierRow - 1)
		return;

	int2 r = dispatchThreadID.xy;
	r.x -= radius;
	r.y -= radius;

	float l = length(r);
	if (l < (float)(radius)-0.5f * (float)(thickness))
		return;
	if (l > (float)(radius)+0.5f * (float)(thickness))
		return;

	float t = atan2(r.y, r.x);

	r += r0;
	if (r.x < 0)
		return;
	if (r.x > width - 1)
		return;
	if (r.y < 0)
		return;
	if (r.y > height - 1)
		return;

	float r_chanel = adobeToRaw(tex[r].rgb).r;
	float f_el = r_chanel * cos(index * t);
	int i_el = round(128 * f_el);

	InterlockedAdd(fourierCoefficientsAtCos_R[index], i_el);
}

[numthreads(16, 16, 4)]
void cs_cos_g(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int index = dispatchThreadID.z;
	if (index > numberOfElementsInFourierRow - 1)
		return;

	int2 r = dispatchThreadID.xy;
	r.x -= radius;
	r.y -= radius;

	float l = length(r);
	if (l < (float)(radius)-0.5f * (float)(thickness))
		return;
	if (l > (float)(radius)+0.5f * (float)(thickness))
		return;

	float t = atan2(r.y, r.x);

	r += r0;
	if (r.x < 0)
		return;
	if (r.x > width - 1)
		return;
	if (r.y < 0)
		return;
	if (r.y > height - 1)
		return;

	float g_chanel = adobeToRaw(tex[r].rgb).g;
	float f_el = g_chanel * cos(index * t);
	int i_el = round(128 * f_el);

	InterlockedAdd(fourierCoefficientsAtCos_G[index], i_el);
}

[numthreads(16, 16, 4)]
void cs_cos_b(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int index = dispatchThreadID.z;
	if (index > numberOfElementsInFourierRow - 1)
		return;

	int2 r = dispatchThreadID.xy;
	r.x -= radius;
	r.y -= radius;

	float l = length(r);
	if (l < (float)(radius)-0.5f * (float)(thickness))
		return;
	if (l > (float)(radius)+0.5f * (float)(thickness))
		return;

	float t = atan2(r.y, r.x);

	r += r0;
	if (r.x < 0)
		return;
	if (r.x > width - 1)
		return;
	if (r.y < 0)
		return;
	if (r.y > height - 1)
		return;

	float b_chanel = adobeToRaw(tex[r].rgb).b;
	float f_el = b_chanel * cos(index * t);
	int i_el = round(128 * f_el);

	InterlockedAdd(fourierCoefficientsAtCos_B[index], i_el);
}

// Coefficients near sin

[numthreads(16, 16, 4)]
void cs_sin_r(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int index = dispatchThreadID.z;
	if (index > numberOfElementsInFourierRow - 1)
		return;

	int2 r = dispatchThreadID.xy;
	r.x -= radius;
	r.y -= radius;

	float l = length(r);
	if (l < (float)(radius)-0.5f * (float)(thickness))
		return;
	if (l > (float)(radius)+0.5f * (float)(thickness))
		return;

	float t = atan2(r.y, r.x);

	r += r0;
	if (r.x < 0)
		return;
	if (r.x > width - 1)
		return;
	if (r.y < 0)
		return;
	if (r.y > height - 1)
		return;

	float r_chanel = adobeToRaw(tex[r].rgb).r;
	float f_el = r_chanel * sin(index * t);
	int i_el = round(128 * f_el);

	InterlockedAdd(fourierCoefficientsAtSin_R[index], i_el);
}

[numthreads(16, 16, 4)]
void cs_sin_g(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int index = dispatchThreadID.z;
	if (index > numberOfElementsInFourierRow - 1)
		return;

	int2 r = dispatchThreadID.xy;
	r.x -= radius;
	r.y -= radius;

	float l = length(r);
	if (l < (float)(radius)-0.5f * (float)(thickness))
		return;
	if (l > (float)(radius)+0.5f * (float)(thickness))
		return;

	float t = atan2(r.y, r.x);

	r += r0;
	if (r.x < 0)
		return;
	if (r.x > width - 1)
		return;
	if (r.y < 0)
		return;
	if (r.y > height - 1)
		return;

	float g_chanel = adobeToRaw(tex[r].rgb).g;
	float f_el = g_chanel * sin(index * t);
	int i_el = round(128 * f_el);

	InterlockedAdd(fourierCoefficientsAtSin_G[index], i_el);
}

[numthreads(16, 16, 4)]
void cs_sin_b(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int index = dispatchThreadID.z;
	if (index > numberOfElementsInFourierRow - 1)
		return;

	int2 r = dispatchThreadID.xy;
	r.x -= ceil((float)(radius)+0.5f * (float)(thickness));
	r.y -= ceil((float)(radius)+0.5f * (float)(thickness));

	float l = length(r);
	if (l < (float)(radius)-0.5f * (float)(thickness))
		return;
	if (l > (float)(radius)+0.5f * (float)(thickness))
		return;

	float t = atan2(r.y, r.x);

	r += r0;
	if (r.x < 0)
		return;
	if (r.x > width - 1)
		return;
	if (r.y < 0)
		return;
	if (r.y > height - 1)
		return;

	float b_chanel = adobeToRaw(tex[r].rgb).b;
	float f_el = b_chanel * sin(index * t);
	int i_el = round(128 * f_el);

	InterlockedAdd(fourierCoefficientsAtSin_B[index], i_el);
}

// Coefficients near cos

technique11 FourierTransform_cos_r
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_cos_r()));
	}
};

technique11 FourierTransform_cos_g
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_cos_g()));
	}
};

technique11 FourierTransform_cos_b
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_cos_b()));
	}
};

// Coefficients near sin

technique11 FourierTransform_sin_r
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_sin_r()));
	}
};

technique11 FourierTransform_sin_g
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_sin_g()));
	}
};

technique11 FourierTransform_sin_b
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_sin_b()));
	}
};