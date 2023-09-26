#include "constants.hlsl"

#define SIZE 10

Texture2DArray<float> Ar;
Texture2DArray<float> Ag;
Texture2DArray<float> Ab;
Texture2DArray<float> Aa;

Texture2DArray<float> Br;
Texture2DArray<float> Bg;
Texture2DArray<float> Bb;
Texture2DArray<float> Ba;

RWTexture2D<uint> mapAtoB;
RWTexture2D<uint> error;

float4 get(
	Texture2DArray<float> r,
	Texture2DArray<float> g,
	Texture2DArray<float> b,
	Texture2DArray<float> a,
	uint3 pos
)
{
	float4 res;
	res.r = r[pos];
	res.g = g[pos];
	res.b = b[pos];
	res.a = a[pos];
	return res;
}

float4 sampleImage(
	Texture2DArray<float> r,
	Texture2DArray<float> g,
	Texture2DArray<float> b,
	Texture2DArray<float> a,
	float2 fPos
)
{
	uint2 uiPos = floor(fPos);

	float4 a00 = get(r, g, b, a, uint3(uiPos, 0));
	float4 a10 = get(r, g, b, a, uint3(uiPos, 1));
	float4 a20 = get(r, g, b, a, uint3(uiPos, 2));
	float4 a30 = get(r, g, b, a, uint3(uiPos, 3));
	float4 a40 = get(r, g, b, a, uint3(uiPos, 4));
	float4 a50 = get(r, g, b, a, uint3(uiPos, 5));

	float4 a01 = get(r, g, b, a, uint3(uiPos, 6));
	float4 a11 = get(r, g, b, a, uint3(uiPos, 7));
	float4 a21 = get(r, g, b, a, uint3(uiPos, 8));
	float4 a31 = get(r, g, b, a, uint3(uiPos, 9));
	float4 a41 = get(r, g, b, a, uint3(uiPos, 10));
	float4 a51 = get(r, g, b, a, uint3(uiPos, 11));

	float4 a02 = get(r, g, b, a, uint3(uiPos, 12));
	float4 a12 = get(r, g, b, a, uint3(uiPos, 13));
	float4 a22 = get(r, g, b, a, uint3(uiPos, 14));
	float4 a32 = get(r, g, b, a, uint3(uiPos, 15));
	float4 a42 = get(r, g, b, a, uint3(uiPos, 16));
	float4 a52 = get(r, g, b, a, uint3(uiPos, 17));

	float4 a03 = get(r, g, b, a, uint3(uiPos, 18));
	float4 a13 = get(r, g, b, a, uint3(uiPos, 19));
	float4 a23 = get(r, g, b, a, uint3(uiPos, 20));
	float4 a33 = get(r, g, b, a, uint3(uiPos, 21));
	float4 a43 = get(r, g, b, a, uint3(uiPos, 22));
	float4 a53 = get(r, g, b, a, uint3(uiPos, 23));

	float4 a04 = get(r, g, b, a, uint3(uiPos, 24));
	float4 a14 = get(r, g, b, a, uint3(uiPos, 25));
	float4 a24 = get(r, g, b, a, uint3(uiPos, 26));
	float4 a34 = get(r, g, b, a, uint3(uiPos, 27));
	float4 a44 = get(r, g, b, a, uint3(uiPos, 28));
	float4 a54 = get(r, g, b, a, uint3(uiPos, 29));

	float4 a05 = get(r, g, b, a, uint3(uiPos, 30));
	float4 a15 = get(r, g, b, a, uint3(uiPos, 31));
	float4 a25 = get(r, g, b, a, uint3(uiPos, 32));
	float4 a35 = get(r, g, b, a, uint3(uiPos, 33));
	float4 a45 = get(r, g, b, a, uint3(uiPos, 34));
	float4 a55 = get(r, g, b, a, uint3(uiPos, 35));

	float2 xy = fPos - uiPos;
	
	float x = xy.x;
	float x2 = x * x;
	float x3 = x2 * x;
	float x4 = x3 * x;
	float x5 = x4 * x;

	float y = xy.y;
	float y2 = y * y;
	float y3 = y2 * y;
	float y4 = y3 * y;
	float y5 = y4 * y;

	float4 color = 0;

	color += a00 + a10 * x + a20 * x2 + a30 * x3 + a40 * x4 + a50 * x5;
	color += (a01 + a11 * x + a21 * x2 + a31 * x3 + a41 * x4 + a51 * x5) * y;
	color += (a02 + a12 * x + a22 * x2 + a32 * x3 + a42 * x4 + a52 * x5) * y2;
	color += (a03 + a13 * x + a23 * x2 + a33 * x3 + a43 * x4 + a53 * x5) * y3;
	color += (a04 + a14 * x + a24 * x2 + a34 * x3 + a44 * x4 + a54 * x5) * y4;
	color += (a05 + a15 * x + a25 * x2 + a35 * x3 + a45 * x4 + a55 * x5) * y5;

	return color;
}

void sampleImage(
	Texture2DArray<float> r,
	Texture2DArray<float> g,
	Texture2DArray<float> b,
	Texture2DArray<float> a,
	float2 fPos,
	out float3 color,
	out float2 grad_color_r,
	out float2 grad_color_g,
	out float2 grad_color_b,
	out float2x2 hessian_color_r,
	out float2x2 hessian_color_g,
	out float2x2 hessian_color_b
	)
{
	uint2 uiPos = floor(fPos);

	float4 a00 = get(r, g, b, a, uint3(uiPos, 0));
	float4 a10 = get(r, g, b, a, uint3(uiPos, 1));
	float4 a20 = get(r, g, b, a, uint3(uiPos, 2));
	float4 a30 = get(r, g, b, a, uint3(uiPos, 3));
	float4 a40 = get(r, g, b, a, uint3(uiPos, 4));
	float4 a50 = get(r, g, b, a, uint3(uiPos, 5));

	float4 a01 = get(r, g, b, a, uint3(uiPos, 6));
	float4 a11 = get(r, g, b, a, uint3(uiPos, 7));
	float4 a21 = get(r, g, b, a, uint3(uiPos, 8));
	float4 a31 = get(r, g, b, a, uint3(uiPos, 9));
	float4 a41 = get(r, g, b, a, uint3(uiPos, 10));
	float4 a51 = get(r, g, b, a, uint3(uiPos, 11));

	float4 a02 = get(r, g, b, a, uint3(uiPos, 12));
	float4 a12 = get(r, g, b, a, uint3(uiPos, 13));
	float4 a22 = get(r, g, b, a, uint3(uiPos, 14));
	float4 a32 = get(r, g, b, a, uint3(uiPos, 15));
	float4 a42 = get(r, g, b, a, uint3(uiPos, 16));
	float4 a52 = get(r, g, b, a, uint3(uiPos, 17));

	float4 a03 = get(r, g, b, a, uint3(uiPos, 18));
	float4 a13 = get(r, g, b, a, uint3(uiPos, 19));
	float4 a23 = get(r, g, b, a, uint3(uiPos, 20));
	float4 a33 = get(r, g, b, a, uint3(uiPos, 21));
	float4 a43 = get(r, g, b, a, uint3(uiPos, 22));
	float4 a53 = get(r, g, b, a, uint3(uiPos, 23));

	float4 a04 = get(r, g, b, a, uint3(uiPos, 24));
	float4 a14 = get(r, g, b, a, uint3(uiPos, 25));
	float4 a24 = get(r, g, b, a, uint3(uiPos, 26));
	float4 a34 = get(r, g, b, a, uint3(uiPos, 27));
	float4 a44 = get(r, g, b, a, uint3(uiPos, 28));
	float4 a54 = get(r, g, b, a, uint3(uiPos, 29));

	float4 a05 = get(r, g, b, a, uint3(uiPos, 30));
	float4 a15 = get(r, g, b, a, uint3(uiPos, 31));
	float4 a25 = get(r, g, b, a, uint3(uiPos, 32));
	float4 a35 = get(r, g, b, a, uint3(uiPos, 33));
	float4 a45 = get(r, g, b, a, uint3(uiPos, 34));
	float4 a55 = get(r, g, b, a, uint3(uiPos, 35));

	float2 xy = fPos - uiPos;

	float x = xy.x;
	float x2 = x * x;
	float x3 = x2 * x;
	float x4 = x3 * x;
	float x5 = x4 * x;

	float y = xy.y;
	float y2 = y * y;
	float y3 = y2 * y;
	float y4 = y3 * y;
	float y5 = y4 * y;

	color = 0;

	color += a00 + a10 * x + a20 * x2 + a30 * x3 + a40 * x4 + a50 * x5;
	color += (a01 + a11 * x + a21 * x2 + a31 * x3 + a41 * x4 + a51 * x5) * y;
	color += (a02 + a12 * x + a22 * x2 + a32 * x3 + a42 * x4 + a52 * x5) * y2;
	color += (a03 + a13 * x + a23 * x2 + a33 * x3 + a43 * x4 + a53 * x5) * y3;
	color += (a04 + a14 * x + a24 * x2 + a34 * x3 + a44 * x4 + a54 * x5) * y4;
	color += (a05 + a15 * x + a25 * x2 + a35 * x3 + a45 * x4 + a55 * x5) * y5;

	float x_dx = 1;
	float x2_dx = 2 * x;
	float x3_dx = 3 * x2;
	float x4_dx = 4 * x3;
	float x5_dx = 5 * x4;

	float4 color_dx = 0;

	color_dx += a10 * x_dx + a20 * x2_dx + a30 * x3_dx + a40 * x4_dx + a50 * x5_dx;
	color_dx += (a11 * x_dx + a21 * x2_dx + a31 * x3_dx + a41 * x4_dx + a51 * x5_dx) * y;
	color_dx += (a12 * x_dx + a22 * x2_dx + a32 * x3_dx + a42 * x4_dx + a52 * x5_dx) * y2;
	color_dx += (a13 * x_dx + a23 * x2_dx + a33 * x3_dx + a43 * x4_dx + a53 * x5_dx) * y3;
	color_dx += (a14 * x_dx + a24 * x2_dx + a34 * x3_dx + a44 * x4_dx + a54 * x5_dx) * y4;
	color_dx += (a15 * x_dx + a25 * x2_dx + a35 * x3_dx + a45 * x4_dx + a55 * x5_dx) * y5;

	float y_dy = 1;
	float y2_dy = 2 * y;
	float y3_dy = 3 * y2;
	float y4_dy = 4 * y3;
	float y5_dy = 5 * y4;

	float4 color_dy = 0;

	color_dy += (a01 + a11 * x + a21 * x2 + a31 * x3 + a41 * x4 + a51 * x5) * y_dy;
	color_dy += (a02 + a12 * x + a22 * x2 + a32 * x3 + a42 * x4 + a52 * x5) * y2_dy;
	color_dy += (a03 + a13 * x + a23 * x2 + a33 * x3 + a43 * x4 + a53 * x5) * y3_dy;
	color_dy += (a04 + a14 * x + a24 * x2 + a34 * x3 + a44 * x4 + a54 * x5) * y4_dy;
	color_dy += (a05 + a15 * x + a25 * x2 + a35 * x3 + a45 * x4 + a55 * x5) * y5_dy;

	grad_color_r = float2(color_dx.r, color_dy.r);
	grad_color_g = float2(color_dx.g, color_dy.g);
	grad_color_b = float2(color_dx.b, color_dy.b);

	float x2_dx_dx = 2;
	float x3_dx_dx = 6 * x;
	float x4_dx_dx = 12 * x2;
	float x5_dx_dx = 20 * x3;

	float4 color_dx_dx = 0;

	color_dx_dx += a20 * x2_dx_dx + a30 * x3_dx_dx + a40 * x4_dx_dx + a50 * x5_dx_dx;
	color_dx_dx += (a21 * x2_dx_dx + a31 * x3_dx_dx + a41 * x4_dx_dx + a51 * x5_dx_dx) * y;
	color_dx_dx += (a22 * x2_dx_dx + a32 * x3_dx_dx + a42 * x4_dx_dx + a52 * x5_dx_dx) * y2;
	color_dx_dx += (a23 * x2_dx_dx + a33 * x3_dx_dx + a43 * x4_dx_dx + a53 * x5_dx_dx) * y3;
	color_dx_dx += (a24 * x2_dx_dx + a34 * x3_dx_dx + a44 * x4_dx_dx + a54 * x5_dx_dx) * y4;
	color_dx_dx += (a25 * x2_dx_dx + a35 * x3_dx_dx + a45 * x4_dx_dx + a55 * x5_dx_dx) * y5;

	float y2_dy_dy = 2;
	float y3_dy_dy = 6 * y;
	float y4_dy_dy = 12 * y2;
	float y5_dy_dy = 20 * y3;

	float4 color_dy_dy = 0;

	color_dy_dy += (a02 + a12 * x + a22 * x2 + a32 * x3 + a42 * x4 + a52 * x5) * y2_dy_dy;
	color_dy_dy += (a03 + a13 * x + a23 * x2 + a33 * x3 + a43 * x4 + a53 * x5) * y3_dy_dy;
	color_dy_dy += (a04 + a14 * x + a24 * x2 + a34 * x3 + a44 * x4 + a54 * x5) * y4_dy_dy;
	color_dy_dy += (a05 + a15 * x + a25 * x2 + a35 * x3 + a45 * x4 + a55 * x5) * y5_dy_dy;

	float4 color_dx_dy = 0;

	color_dx_dy += (a11 * x_dx + a21 * x2_dx + a31 * x3_dx + a41 * x4_dx + a51 * x5_dx) * y_dy;
	color_dx_dy += (a12 * x_dx + a22 * x2_dx + a32 * x3_dx + a42 * x4_dx + a52 * x5_dx) * y2_dy;
	color_dx_dy += (a13 * x_dx + a23 * x2_dx + a33 * x3_dx + a43 * x4_dx + a53 * x5_dx) * y3_dy;
	color_dx_dy += (a14 * x_dx + a24 * x2_dx + a34 * x3_dx + a44 * x4_dx + a54 * x5_dx) * y4_dy;
	color_dx_dy += (a15 * x_dx + a25 * x2_dx + a35 * x3_dx + a45 * x4_dx + a55 * x5_dx) * y5_dy;

	hessian_color_r = float2x2(
		color_dx_dx.r, color_dx_dy.r,
		color_dx_dy.r, color_dy_dy.r
		);
	hessian_color_g = float2x2(
		color_dx_dx.g, color_dx_dy.g,
		color_dx_dy.g, color_dy_dy.g
		);
	hessian_color_b = float2x2(
		color_dx_dx.b, color_dx_dy.b,
		color_dx_dy.b, color_dy_dy.b
		);
}

float3x3 calculateTransform(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		s0 * cos(a0), s0 * sin(a0), 0,
		-s1 * sin(a1), s1 * cos(a1), 0,
		t.x, t.y, 1
		);
}

float3x3 calculateTransform_da0(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		-s0 * sin(a0), s0 * cos(a0), 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_da0_da0(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		-s0 * cos(a0), -s0 * sin(a0), 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_da0_ds0(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		-sin(a0), cos(a0), 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_da0_da1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_da0_ds1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_ds0(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		cos(a0), sin(a0), 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_ds0_da0(float2 t, float a0, float s0, float a1, float s1)
{
	return calculateTransform_da0_ds0(t, a0, s0, a1, s1);
}

float3x3 calculateTransform_ds0_ds0(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_ds0_da1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_ds0_ds1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_da1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		-s1 * cos(a1), -s1 * sin(a1), 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_da1_da0(float2 t, float a0, float s0, float a1, float s1)
{
	return calculateTransform_da0_da1(t, a0, s0, a1, s1);
}

float3x3 calculateTransform_da1_ds0(float2 t, float a0, float s0, float a1, float s1)
{
	return calculateTransform_ds0_da1(t, a0, s0, a1, s1);
}

float3x3 calculateTransform_da1_da1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		s1 * sin(a1), -s1 * cos(a1), 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_da1_ds1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		-cos(a1), -sin(a1), 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_ds1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		-sin(a1), cos(a1), 0,
		0, 0, 0
		);
}

float3x3 calculateTransform_ds1_da0(float2 t, float a0, float s0, float a1, float s1)
{
	return calculateTransform_da0_ds1(t, a0, s0, a1, s1);
}

float3x3 calculateTransform_ds1_ds0(float2 t, float a0, float s0, float a1, float s1)
{
	return calculateTransform_ds0_ds1(t, a0, s0, a1, s1);
}

float3x3 calculateTransform_ds1_da1(float2 t, float a0, float s0, float a1, float s1)
{
	return calculateTransform_da1_ds1(t, a0, s0, a1, s1);
}

float3x3 calculateTransform_ds1_ds1(float2 t, float a0, float s0, float a1, float s1)
{
	return float3x3(
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
		);
}

float calculateDiscrepancy(
	Texture2DArray<float> Ar,
	Texture2DArray<float> Ag,
	Texture2DArray<float> Ab,
	Texture2DArray<float> Aa,
	Texture2DArray<float> Br,
	Texture2DArray<float> Bg,
	Texture2DArray<float> Bb,
	Texture2DArray<float> Ba,
	uint2 posInA, 
	float3x3 transform, 
	int sizeX, 
	int sizeY
)
{
	float discrepancy = 0.0f;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);
	for (int i = -sizeX; i <= sizeX; i++)
	{
		if (posInA.x + i < 0)
			continue;
		if (posInA.x + i >= width)
			break;

		for (int j = -sizeY; j <= sizeY; j++)
		{
			if (posInA.y + j < 0)
				continue;
			if (posInA.y + j >= height)
				break;

			uint2 posInAij = posInA + int2(i, j);

			float4 colorInAij = get(Ar, Ag, Ab, Aa, uint3(posInAij, 0));

			float2 posInB = posInA + mul(float3(i, j, 1), transform).xy;
			float4 colorInBij = sampleImage(Br, Bg, Bb, Ba, posInB);

			float4 diff = colorInBij - colorInAij;
			discrepancy += dot(diff, diff);
		}
	}

	discrepancy /= (2 * sizeX + 1) * (2 * sizeY + 1);

	return discrepancy;
}

void calculateGradientsOfCell(
	Texture2DArray<float> Ar,
	Texture2DArray<float> Ag,
	Texture2DArray<float> Ab,
	Texture2DArray<float> Aa,
	uint2 originInA,
	int2 relativePosInA,
	Texture2DArray<float> Br,
	Texture2DArray<float> Bg,
	Texture2DArray<float> Bb,
	Texture2DArray<float> Ba,
	int2 translation,
	float4 paramsOfTransform, // {angle0,scale0,angle1,scale1}
	out float4 gradient,
	out float4 gradientOfGradientSquaredLength
)
{
	uint2 posInA = originInA + relativePosInA;
	float3 A = get(Ar, Ag, Ab, Aa, uint3(posInA, 0)).xyz;

	float3x3 transform = calculateTransform(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	);
	float2 posInB = originInA + mul(float3(relativePosInA, 1), transform).xy;

	float3 B;
	float2 grad_B[3];
	float2x2 hessian_B[3];
	sampleImage(
		Br,
		Bg,
		Bb,
		Ba,
		posInB,
		B,
		grad_B[0],
		grad_B[1],
		grad_B[2],
		hessian_B[0],
		hessian_B[1],
		hessian_B[2]
	);

	float3x3 transform_dParam[4] = {
		calculateTransform_da0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_ds0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_da1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_ds1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	)
	};

	float2 pos_dParam[4];
	for (int j = 0; j < 4; j++)
	{
		pos_dParam[j] = mul(float3(relativePosInA, 1), transform_dParam[j]).xy;
		gradient[j] = 0;
		for (int i = 0; i < 3; i++)
			gradient[j] += 2 * (B[i] - A[i]) * dot(grad_B[i], pos_dParam[j]);
	}
	
	float3x3 transform_dParam2[16] = {
// Row0
		calculateTransform_da0_da0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_da0_ds0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_da0_da1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_da0_ds1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
// Row1
		calculateTransform_ds0_da0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_ds0_ds0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_ds0_da1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_ds0_ds1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
// Row2
		calculateTransform_da1_da0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_da1_ds0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_da1_da1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_da1_ds1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
// Row3
		calculateTransform_ds1_da0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_ds1_ds0(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_ds1_da1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	),
		calculateTransform_ds1_ds1(
		translation,
		paramsOfTransform[0],
		paramsOfTransform[1],
		paramsOfTransform[2],
		paramsOfTransform[3]
	)
	};

	for (int k = 0; k < 4; k++)
	{
		gradientOfGradientSquaredLength[k] = 0;
		for (int j = 0; j < 4; j++)
		{
			float x = 0;
			for (int i = 0; i < 3; i++)
			{
				x += dot(mul(pos_dParam[k], hessian_B[i]), pos_dParam[j]);

				float2 pos_dParam_j_k = mul(float3(relativePosInA, 1), transform_dParam2[j * 4 + k]).xy;
				x += dot(grad_B[i], pos_dParam_j_k);

				x *= B[i] - A[i];

				x += dot(grad_B[i], pos_dParam[k]) * dot(grad_B[i], pos_dParam[j]);
			}
			x *= 4 * gradient[j];
			gradientOfGradientSquaredLength[k] += x;
		}
	}
}

void calculateGradients(
	Texture2DArray<float> Ar,
	Texture2DArray<float> Ag,
	Texture2DArray<float> Ab,
	Texture2DArray<float> Aa,
	uint2 originInA,
	Texture2DArray<float> Br,
	Texture2DArray<float> Bg,
	Texture2DArray<float> Bb,
	Texture2DArray<float> Ba,
	int2 translation,
	float4 paramsOfTransform, // {angle0,scale0,angle1,scale1}
	int sizeX,
	int sizeY,
	out float4 gradient,
	out float4 gradientOfGradientSquaredLength
)
{
	gradient = 0;
	gradientOfGradientSquaredLength = 0;
	for (int i = -sizeX; i <= sizeX; i++)
	{
		for (int j = -sizeY; j <= sizeY; j++)
		{
			int2 relativePosInA = int2(i, j);
			float4 gradient_cell = 0;
			float4 gradientOfGradientSquaredLength_cell = 0;
			calculateGradientsOfCell(
				Ar,
				Ag,
				Ab,
				Aa,
				originInA,
				relativePosInA,
				Br,
				Bg,
				Bb,
				Ba,
				translation,
				paramsOfTransform, // {angle0,scale0,angle1,scale1}
				gradient_cell,
				gradientOfGradientSquaredLength_cell
			);

			gradient += gradient_cell;
			gradientOfGradientSquaredLength += gradientOfGradientSquaredLength_cell;
		}
	}
	gradient /= (2 * sizeX + 1) * (2 * sizeY + 1);
	gradientOfGradientSquaredLength /= (2 * sizeX + 1) * (2 * sizeY + 1);
}

void calculateInitialTransformParams(
	Texture2DArray<float> Ar,
	Texture2DArray<float> Ag,
	Texture2DArray<float> Ab,
	Texture2DArray<float> Aa,
	uint2 posInA,
	Texture2DArray<float> Br,
	Texture2DArray<float> Bg,
	Texture2DArray<float> Bb,
	Texture2DArray<float> Ba,
	uint2 posInB,
	out float params[5]
)
{
	float4 a00 = get(Ar, Ag, Ab, Aa, uint3(uiPos, 0));
	float4 a10 = get(Ar, Ag, Ab, Aa, uint3(uiPos, 1));
	float4 a01 = get(Ar, Ag, Ab, Aa, uint3(uiPos, 6));

	float4 b00 = get(Br, Bg, Bb, Ba, uint3(uiPos, 0));
	float4 b10 = get(Br, Bg, Bb, Ba, uint3(uiPos, 1));
	float4 b01 = get(Br, Bg, Bb, Ba, uint3(uiPos, 6));

	params[4] = dot(a00.xyz, b00.xyz) / dot(a00.xyz, a00.xyz);
	a10 *= params[4];
	a01 *= params[4];


}

// return error
float fittingTransformByGradientDescent(
	Texture2DArray<float> Ar,
	Texture2DArray<float> Ag,
	Texture2DArray<float> Ab,
	Texture2DArray<float> Aa,
	uint2 posInA,
	Texture2DArray<float> Br,
	Texture2DArray<float> Bg,
	Texture2DArray<float> Bb,
	Texture2DArray<float> Ba,
	uint2 posInB,
	uint sizeX,
	uint sizeY,
	inout float4 params
)
{
	float error = FLT_MAX;
	for (int i = 0; i < 5; i++)
	{
		float4 gradient;
		float4 gradientOfGradientSquaredLength;
		calculateGradients(
			Ar,
			Ag,
			Ab,
			Aa,
			posInA,
			Br,
			Bg,
			Bb,
			Ba,
			posInB - posInA,
			params, // {angle0,scale0,angle1,scale1}
			sizeX,
			sizeY,
			gradient,
			gradientOfGradientSquaredLength
		);
		
		float gradient2 = dot(gradient, gradient);
		error = min(error, gradient2);

		float gradientOfGradientSquaredLength2 = dot(gradientOfGradientSquaredLength, gradientOfGradientSquaredLength);
		if (gradientOfGradientSquaredLength2 == 0)
			return error;

		float c = -gradient2 / gradientOfGradientSquaredLength2;
		params += c * gradientOfGradientSquaredLength;
	}
	return error;
}

[numthreads(32,32,1)]
void CS_calculate_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);

	uint wXh = width * height;
	if (dispatchThreadID.x >= wXh)
		return;
	if (dispatchThreadID.y >= wXh)
		return;

	uint2 posInA = uint2(dispatchThreadID.x % width, dispatchThreadID.x / width);
	uint2 posInB = uint2(dispatchThreadID.y % width, dispatchThreadID.y / width);
	
	float sizeX = SIZE;
	float sizeY = SIZE;
	float4 params = 0;
	float2 translation = 0;
	uint typeOfMapping = calculateCoarseTransformParams_NetMethod(
		Ar,
		Ag,
		Ab,
		Aa,
		posInA,
		Br,
		Bg,
		Bb,
		Ba,
		posInB,
		sizeX,
		sizeY,
		params,
		translation
	);

	uint err = UINT_MAX;
	if (typeOfMapping == MAP_A_TO_B)
	{
		err = 1000000 * fittingTransformByGradientDescent(
			Ar,
			Ag,
			Ab,
			Aa,
			posInA,
			Br,
			Bg,
			Bb,
			Ba,
			posInB,
			sizeX,
			sizeY,
			params
		);
	}
	if (typeOfMapping == MAP_B_TO_A)
	{
		err = 1000000 * fittingTransformByGradientDescent(
			Br,
			Bg,
			Bb,
			Ba,
			posInB,
			Ar,
			Ag,
			Ab,
			Aa,
			posInA,
			sizeX,
			sizeY,
			params
		);
	}

	InterlockedMin(error[posInA], err);

	uint original_value;
	InterlockedExchange(mapAtoB[posInA], UINT_MAX, original_value);
}

[numthreads(32, 32, 1)]
void CS_map_A_onto_B(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);

	uint wXh = width * height;
	if (dispatchThreadID.x >= wXh)
		return;
	if (dispatchThreadID.y >= wXh)
		return;

	uint2 posInA = uint2(dispatchThreadID.x % width, dispatchThreadID.x / width);
	uint2 posInB = uint2(dispatchThreadID.y % width, dispatchThreadID.y / width);

	float sizeX = SIZE;
	float sizeY = SIZE;
	float4 params = 0;
	float2 translation = 0;
	uint typeOfMapping = calculateCoarseTransformParams_NetMethod(
		Ar,
		Ag,
		Ab,
		Aa,
		posInA,
		Br,
		Bg,
		Bb,
		Ba,
		posInB,
		sizeX,
		sizeY,
		params,
		translation
	);

	uint err = UINT_MAX;
	if (typeOfMapping == MAP_A_TO_B)
	{
		err = 1000000 * fittingTransformByGradientDescent(
			Ar,
			Ag,
			Ab,
			Aa,
			posInA,
			Br,
			Bg,
			Bb,
			Ba,
			posInB,
			sizeX,
			sizeY,
			params
		);
	}
	if (typeOfMapping == MAP_B_TO_A)
	{
		err = 1000000 * fittingTransformByGradientDescent(
			Br,
			Bg,
			Bb,
			Ba,
			posInB,
			Ar,
			Ag,
			Ab,
			Aa,
			posInA,
			sizeX,
			sizeY,
			params
		);
	}

	if (err == error[posInA])
	{
		uint original_value;
		InterlockedExchange(mapAtoB[posInA], dispatchThreadID.y, original_value);
	}
}

technique11 DefineTheSamePointsOnTwoImages
{
	pass CalculateError
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_calculate_error()));
	}
	pass MapAontoB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_map_A_onto_B()));
	}
};