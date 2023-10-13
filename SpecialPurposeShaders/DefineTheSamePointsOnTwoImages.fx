#include "constants.hlsl"

#define max_count_of_derivatives 121

uint size;
float threshold0;
float threshold1;

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
	res.r = r[pos].r;
	res.g = g[pos].r;
	res.b = b[pos].r;
	res.a = a[pos].r;
	return res;
}

void updateCoefficientsOfLinearSystemForCalculationOfTransform(
	Texture2DArray<float> chanelOfA,
	int2 posInA,
	Texture2DArray<float> chanelOfB,
	int2 posInB,
	bool positiveDirection,
	out float a00a00, out float a00b00,
	out float l00, out float l01,
	out float l10, out float l11,
	out float r00, out float r10,
	out float r01, out float r11,
	out int2 nextPosInA, out int2 nextPosInB
)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	chanelOfA.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;

	float a00 = chanelOfA[int3(posInA, 0)].r;
	float b00 = chanelOfB[int3(posInB, 0)].r;

	float a10 = chanelOfA[int3(posInA, 1)].r;
	float a01 = chanelOfA[int3(posInA, maxOrderOfDerivatives + 1)].r;

	float b10 = chanelOfB[int3(posInB, 1)].r;
	float b01 = chanelOfB[int3(posInB, maxOrderOfDerivatives + 1)].r;

	a00a00 += a00 * a00;
	a00b00 += a00 * b00;

	l00 += b10 * b10;
	l01 += b10 * b01;
	l11 += b01 * b01;

	r00 += a10 * b10;
	r10 += a10 * b01;

	r01 += b10 * a01;
	r11 += a01 * b01;

	float2 gradA = float2(a10, a01);
	float2 gradB = float2(b10, b01);
}

void calculateTransform(
	double4 Aderivatives[max_count_of_derivatives],
	double4 Bderivatives[max_count_of_derivatives],
	out double2x2 transformBtoA,
	out double specCoeffA
)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;

	double4 a00 = Aderivatives[0];
	double4 b00 = Bderivatives[0];
	specCoeffA = (float)(dot(a00.xyz, b00.xyz)) / (float)(dot(a00.xyz, a00.xyz));

	double l00 = 0;
	double l01 = 0;
	double l11 = 0;

	double r00 = 0;
	double r10 = 0;

	double r01 = 0;
	double r11 = 0;

	for (int i = 0; i < maxOrderOfDerivatives; i++)
	{
		for (int j = 0; j < maxOrderOfDerivatives; j++)
		{
			double4 a10 = specCoeffA * Aderivatives[(maxOrderOfDerivatives + 1) * j + i + 1];
			double4 a01 = specCoeffA * Aderivatives[(maxOrderOfDerivatives + 1) * (j + 1) + i];

			double4 b10 = Bderivatives[(maxOrderOfDerivatives + 1) * j + i + 1];
			double4 b01 = Bderivatives[(maxOrderOfDerivatives + 1) * (j + 1) + i];

			l00 += dot(b10.xyz, b10.xyz);
			l01 += dot(b10.xyz, b01.xyz);
			l11 += dot(b01.xyz, b01.xyz);

			r00 += dot(a10.xyz, b10.xyz);
			r10 += dot(a10.xyz, b01.xyz);

			r01 += dot(b10.xyz, a01.xyz);
			r11 += dot(a01.xyz, b01.xyz);
		}
	}
	double l10 = l01;

	double d = l00 * l11 - l01 * l10;
	double d0 = r00 * l11 - l01 * r10;
	double d1 = l00 * r10 - r00 * l10;

	double mxx = d != 0 ? (float)(d0) / (float)(d) : 1;
	double mxy = d != 0 ? (float)(d1) / (float)(d) : 0;

	d0 = r01 * l11 - l01 * r11;
	d1 = l00 * r11 - r01 * l10;

	double myx = d != 0 ? (float)(d0) / (float)(d) : 0;
	double myy = d != 0 ? (float)(d1) / (float)(d) : 1;

	transformBtoA = double2x2(
		mxx, mxy,
		myx, myy
		);
}

double calculateError(
	double4 Aderivatives[max_count_of_derivatives],
	double4 Bderivatives[max_count_of_derivatives],
	double2x2 transformBtoA,
	double specCoeffA
)
{
	double maxError = 0;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;

	double4 a00 = Aderivatives[0];
	double4 b00 = Bderivatives[0];

	double4 e = abs(float4(b00 - specCoeffA * a00) / float4(b00));
	maxError = max(maxError, max(e.x, max(e.y, e.z)));

	for (int i = 0; i < maxOrderOfDerivatives; i++)
	{
		for (int j = 0; j < maxOrderOfDerivatives; j++)
		{
			double4 a10 = specCoeffA * Aderivatives[(maxOrderOfDerivatives + 1) * j + i + 1];
			double4 a01 = specCoeffA * Aderivatives[(maxOrderOfDerivatives + 1) * (j + 1) + i];

			double4 b10 = Bderivatives[(maxOrderOfDerivatives + 1) * j + i + 1];
			double4 b01 = Bderivatives[(maxOrderOfDerivatives + 1) * (j + 1) + i];

			double4 a10theoretical = transformBtoA[0][0] * b10 + transformBtoA[0][1] * b01;
			e = abs(float4(a10theoretical - a10) / float4(a10theoretical));
			maxError = max(maxError, max(e.x, max(e.y, e.z)));

			double4 a01theoretical = transformBtoA[1][0] * b10 + transformBtoA[1][1] * b01;
			e = abs(float4(a01theoretical - a10) / float4(a01theoretical));
			maxError = max(maxError, max(e.x, max(e.y, e.z)));
		}
	}
	return maxError;
}

[numthreads(32, 32, 1)]
void CS_initialize_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint2 ij = dispatchThreadID.xy;

	uint width = 0;
	uint height = 0;
	error.GetDimensions(width, height);

	if (ij.x > width - 1)
		return;
	if (ij.y > height - 1)
		return;
	
	error[ij].r = UINT_MAX;
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

	int maxOrderOfDerivatives = sqrt(elements) - 1;
	int width_ = width - 2 * maxOrderOfDerivatives - 2;
	int height_ = height - 2 * maxOrderOfDerivatives - 2;

	int2 posInA = int2(dispatchThreadID.x % width_, dispatchThreadID.x / width_);
	if (posInA.y > height_ - 1)
		return;
	posInA.x += maxOrderOfDerivatives;
	posInA.y += maxOrderOfDerivatives;

	int2 posInB = int2(dispatchThreadID.y % width_, dispatchThreadID.y / width_);
	if (posInB.y > height_ - 1)
		return;
	posInB.x += maxOrderOfDerivatives + 1;
	posInB.y += maxOrderOfDerivatives + 1;

	uint original_value;
	InterlockedExchange(mapAtoB[posInA].r, UINT_MAX, original_value);

	double4 Aderivatives[max_count_of_derivatives];
	double4 Bderivatives[max_count_of_derivatives];

	for (int i = 0; i < (maxOrderOfDerivatives + 1) * (maxOrderOfDerivatives + 1); i++)
	{
		Aderivatives[i] = get(Ar, Ag, Ab, Aa, uint3(posInA, i));
		Bderivatives[i] = get(Br, Bg, Bb, Ba, uint3(posInB, i));
	}

	double2x2 transformBtoA;
	double specCoeffA;
	calculateTransform(
		Aderivatives,
		Bderivatives,
		transformBtoA,
		specCoeffA
	);

	uint err = 1000000 * calculateError(
		Aderivatives,
		Bderivatives,
		transformBtoA,
		specCoeffA
	);

	InterlockedMin(error[posInA].r, err);
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

	int maxOrderOfDerivatives = sqrt(elements) - 1;
	int width_ = width - 2 * maxOrderOfDerivatives - 2;
	int height_ = height - 2 * maxOrderOfDerivatives - 2;

	int2 posInA = int2(dispatchThreadID.x % width_, dispatchThreadID.x / width_);
	if (posInA.y > height_ - 1)
		return;
	posInA.x += maxOrderOfDerivatives + 1;
	posInA.y += maxOrderOfDerivatives + 1;

	int2 posInB = int2(dispatchThreadID.y % width_, dispatchThreadID.y / width_);
	if (posInB.y > height_ - 1)
		return;
	posInB.x += maxOrderOfDerivatives;
	posInB.y += maxOrderOfDerivatives;

	double4 Aderivatives[max_count_of_derivatives];
	double4 Bderivatives[max_count_of_derivatives];

	for (int i = 0; i < (maxOrderOfDerivatives + 1) * (maxOrderOfDerivatives + 1); i++)
	{
		Aderivatives[i] = get(Ar, Ag, Ab, Aa, uint3(posInA, i));
		Bderivatives[i] = get(Br, Bg, Bb, Ba, uint3(posInB, i));
	}

	double2x2 transformBtoA;
	double specCoeffA;
	calculateTransform(
		Aderivatives,
		Bderivatives,
		transformBtoA,
		specCoeffA
	);

	uint err = 1000000 * calculateError(
		Aderivatives,
		Bderivatives,
		transformBtoA,
		specCoeffA
	);

	if (err == error[posInA].r)
	{
		uint original_value;
		uint new_value = posInB.y * width + posInB.x;
		InterlockedExchange(mapAtoB[posInA].r, new_value, original_value);
	}
}

technique11 DefineTheSamePointsOnTwoImages
{
	pass InitializeError
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_initialize_error()));
	}
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