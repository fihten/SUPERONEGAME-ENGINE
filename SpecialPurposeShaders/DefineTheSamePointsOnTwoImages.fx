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
	inout double a00a00, inout double a00b00,
	inout double l00, inout double l01,
	inout double l10, inout double l11,
	inout double r00, inout double r10,
	inout double r01, inout double r11,
	inout double c,
	out int2 nextPosInA, out int2 nextPosInB,
	out int2 prevPosInA, out int2 prevPosInB,
	out double2 gradA, out double2 gradB
)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	chanelOfA.GetDimensions(mip, width, height, elements, mips);

	if (posInA.x < 0)
		return;
	if (posInA.x > width - 1)
		return;
	if (posInA.y < 0)
		return;
	if (posInA.y > height - 1)
		return;

	if (posInB.x < 0)
		return;
	if (posInB.x > width - 1)
		return;
	if (posInB.y < 0)
		return;
	if (posInB.y > height - 1)
		return;

	int maxOrderOfDerivatives = sqrt(elements) - 1;

	double a00 = chanelOfA[int3(posInA, 0)].r;
	double b00 = chanelOfB[int3(posInB, 0)].r;

	double a10 = chanelOfA[int3(posInA, 1)].r;
	double a01 = chanelOfA[int3(posInA, maxOrderOfDerivatives + 1)].r;

	double b10 = chanelOfB[int3(posInB, 1)].r;
	double b01 = chanelOfB[int3(posInB, maxOrderOfDerivatives + 1)].r;

	a00a00 += a00 * a00;
	a00b00 += a00 * b00;

	l00 += b10 * b10;
	l01 += b10 * b01;
	l10 += b10 * b01;
	l11 += b01 * b01;

	r00 += a10 * b10;
	r10 += a10 * b01;

	r01 += b10 * a01;
	r11 += a01 * b01;

	gradA = double2(a10, a01);
	c += dot(gradA, gradA);

	gradB = double2(b10, b01);

	double hAmin = 0.501f / (float)(max(abs(gradA.x), abs(gradA.y)));
	double hBmin = 0.501f / (float)(max(abs(gradB.x), abs(gradB.y)));

	double hA = hAmin;
	double Da00 = hA * length(gradA);
	double hB = (float)((b00 / a00) * Da00) / (float)(length(gradB));

	double coeff = max((float)(hBmin) / (float)(hB), 1);
	hA *= coeff;
	hB *= coeff;

	nextPosInA = round(posInA + hA * gradA);
	nextPosInB = round(posInB + hB * gradB);

	prevPosInA = round(posInA - hA * gradA);
	prevPosInB = round(posInB - hB * gradB);
}

#define FETCHING_NUMBER 1
void calculateTransform(
	int2 posInA,
	int2 posInB,
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

	double a00a00 = 0;
	double a00b00 = 0;

	double l00 = 0;
	double l01 = 0;
	double l10 = 0;
	double l11 = 0;

	double r00 = 0;
	double r10 = 0;

	double r01 = 0;
	double r11 = 0;

	double c = 0;

	double2 positiveDirectionA = 0;
	double2 positiveDirectionB = 0;

	int2 prevPosInA = 0;
	int2 nextPosInA = 0;

	int2 prevPosInB = 0;
	int2 nextPosInB = 0;
	
	updateCoefficientsOfLinearSystemForCalculationOfTransform(
		Ar, posInA, Br, posInB,
		a00a00, a00b00,
		l00, l01, l10, l11,
		r00, r10, r01, r11,
		c,
		nextPosInA, nextPosInB,
		prevPosInA, prevPosInB,
		positiveDirectionA, positiveDirectionB
	);

	int2 posInAtmp0 = nextPosInA;
	int2 posInAtmp1 = prevPosInA;
	int2 posInBtmp0 = nextPosInB;
	int2 posInBtmp1 = prevPosInB;
	double2 gradA = 0;
	double2 gradB = 0;
	for (int i = 0; i < FETCHING_NUMBER; i++)
	{
		updateCoefficientsOfLinearSystemForCalculationOfTransform(
			Ar, posInAtmp0, Br, posInBtmp0,
			a00a00, a00b00,
			l00, l01, l10, l11,
			r00, r10, r01, r11,
			c,
			nextPosInA, nextPosInB,
			prevPosInA, prevPosInB,
			gradA, gradB
		);

		if (dot(gradA, positiveDirectionA) > 0)
		{
			posInAtmp0 = nextPosInA;
			positiveDirectionA = gradA;
		}
		else
		{
			posInAtmp0 = prevPosInA;
			positiveDirectionA = -gradA;
		}

		if (dot(gradB, positiveDirectionB) > 0)
		{
			posInBtmp0 = nextPosInB;
			positiveDirectionB = gradB;
		}
		else
		{
			posInBtmp0 = prevPosInB;
			positiveDirectionB = -gradB;
		}
	}

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