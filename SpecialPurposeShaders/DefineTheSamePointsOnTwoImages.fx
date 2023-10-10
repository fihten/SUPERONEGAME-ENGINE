#include "constants.hlsl"

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

void calculateInitialTransformParams(
	Texture2DArray<float> Ar,
	Texture2DArray<float> Ag,
	Texture2DArray<float> Ab,
	Texture2DArray<float> Aa,
	int2 posInA,
	Texture2DArray<float> Br,
	Texture2DArray<float> Bg,
	Texture2DArray<float> Bb,
	Texture2DArray<float> Ba,
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

	double4 a00 = get(Ar, Ag, Ab, Aa, uint3(posInA, 0));
	double4 a10 = get(Ar, Ag, Ab, Aa, uint3(posInA, 1));
	double4 a01 = get(Ar, Ag, Ab, Aa, uint3(posInA, maxOrderOfDerivatives + 1));

	double4 b00 = get(Br, Bg, Bb, Ba, uint3(posInB, 0));
	double4 b10 = get(Br, Bg, Bb, Ba, uint3(posInB, 1));
	double4 b01 = get(Br, Bg, Bb, Ba, uint3(posInB, maxOrderOfDerivatives + 1));

	specCoeffA = (float)(dot(a00.xyz, b00.xyz)) / (float)(dot(a00.xyz, a00.xyz));
	a10 *= specCoeffA;
	a01 *= specCoeffA;

	double l00 = dot(b10.xyz, b10.xyz);
	double l01 = dot(b10.xyz, b01.xyz);
	double l10 = l01;
	double l11 = dot(b01.xyz, b01.xyz);

	double r0 = dot(a10.xyz, b10.xyz);
	double r1 = dot(a10.xyz, b01.xyz);

	double d = l00 * l11 - l01 * l10;
	double d0 = r0 * l11 - l01 * r1;
	double d1 = l00 * r1 - r0 * l10;

	double mxx = d != 0 ? (float)(d0) / (float)(d) : 1;
	double mxy = d != 0 ? (float)(d1) / (float)(d) : 0;

	r0 = dot(b10.xyz, a01.xyz);
	r1 = dot(a01.xyz, b01.xyz);

	d = l00 * l11 - l01 * l10;
	d0 = r0 * l11 - l01 * r1;
	d1 = l00 * r1 - r0 * l10;

	double myx = d != 0 ? (float)(d0) / (float)(d) : 0;
	double myy = d != 0 ? (float)(d1) / (float)(d) : 1;

	transformBtoA = double2x2(
		mxx, mxy,
		myx, myy
		);
}

// (mxx*x+myx*y)^k*(mxy*x+myy*y)^l
double findCoefficientsNearXofSpecifiedDegree(
	double mxx, double myx, 
	double mxy, double myy, 
	int k, int l, 
	int degree
)
{
	int degree_factorial = 1;
	int i = 1;
	for (; i <= degree; i++)
		degree_factorial *= i;

	double coefficient = 0;

	// i <= k && i >= 0 && degree - i <= l && degree - i >= 0
	// ->
	// max(0, degree - l) <= i <= min(k, degree)
	int i0 = max(0, degree - l);
	int i1 = min(k, degree);

	int k_factorial = 1;
	double pow_myx = 1;
	i = 1;
	for (; i < k - i0 + 1; i++)
		pow_myx *= myx;
	for (; i <= k; i++)
		k_factorial *= i;

	int l_factorial = 1;
	double pow_myy = 1;
	i = 1;
	for (; i < l - degree + i0 + 1; i++)
		pow_myy *= myy;
	double pow_mxy = 1;
	for (; i <= l; i++)
	{
		pow_mxy *= mxy;
		l_factorial *= i;
	}

	int b = 1;
	double pow_mxx = 1;
	i = 0;
	for (; i < i0; i++)
	{
		pow_mxx *= mxx;
		b *= degree - i;
		b /= i + 1;
	}

	for (; i <= i1; i++)
	{
		int j = degree - i;

		if (k - i == 0)
			pow_myx = 1;
		if (j == 0)
			pow_mxy = 1;
		
		double fc = pow_myx * pow_myy * pow_mxx * pow_mxy;
		int ic = (b * k_factorial * l_factorial) / degree_factorial;
		coefficient += ic * fc;

		b *= j;
		b /= i + 1;

		k_factorial *= k - i;
		l_factorial /= l - j + 1;

		pow_myx = myx != 0 ? (float)(pow_myx) / (float)(myx) : pow_myx;
		pow_myy *= myy;
		pow_mxx *= mxx;
		pow_mxy = mxy != 0 ? (float)(pow_mxy) / (float)(mxy) : pow_mxy;
	}
	return coefficient;
}

#define max_count_of_derivatives 20
void findDerivativesOfSpecifiedOrderInNewBasis(
	double4 oldDerivatives[max_count_of_derivatives], int order, double2x2 transformToNewBasis,
	out double4 newDerivatives[max_count_of_derivatives]
)
{
	int b_new = 1;
	for (int degreeOfXinNewBasis = 0; degreeOfXinNewBasis <= order; degreeOfXinNewBasis++)
	{
		int b_old = 1;
		newDerivatives[degreeOfXinNewBasis] = double4(0, 0, 0, 0);
		for (int degreeOfXinOldBasis = 0; degreeOfXinOldBasis <= order; degreeOfXinOldBasis++)
		{
			int degreeOfYinOldBasis = order - degreeOfXinOldBasis;
			double coeff = b_old*findCoefficientsNearXofSpecifiedDegree(
					transformToNewBasis[0][0], transformToNewBasis[1][0],
					transformToNewBasis[0][1], transformToNewBasis[1][1],
					degreeOfXinOldBasis, degreeOfYinOldBasis,
					degreeOfXinNewBasis
				);
			newDerivatives[degreeOfXinNewBasis] += coeff * oldDerivatives[degreeOfXinOldBasis];

			b_old *= degreeOfYinOldBasis;
			b_old /= degreeOfXinOldBasis + 1;
		}
		newDerivatives[degreeOfXinNewBasis] = float4(newDerivatives[degreeOfXinNewBasis]) / (float)(b_new);

		int degreeOfYinNewBasis = order - degreeOfXinNewBasis;
		b_new *= degreeOfYinNewBasis;
		b_new /= degreeOfXinNewBasis + 1;
	}
}

double calculateDiscrepancyOfSpecifiedOrder(
	Texture2DArray<float> Ar,
	Texture2DArray<float> Ag,
	Texture2DArray<float> Ab,
	Texture2DArray<float> Aa,
	int2 posInA,
	Texture2DArray<float> Br,
	Texture2DArray<float> Bg,
	Texture2DArray<float> Bb,
	Texture2DArray<float> Ba,
	int2 posInB,
	double2x2 transformAtoB,
	double specCoeffB,
	int order
)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;

	double4 derivativesA[max_count_of_derivatives];
	double4 derivativesB[max_count_of_derivatives];
	double4 derivativesBtheoretical[max_count_of_derivatives];
	for (int i = 0; i <= order; i++)
	{
		int j = order - i;
		int index = j * (maxOrderOfDerivatives + 1) + i;

		derivativesA[i] = get(Ar, Ag, Ab, Aa, uint3(posInA, index));
		derivativesB[i] = specCoeffB * get(Br, Bg, Bb, Ba, uint3(posInB, index));
		derivativesBtheoretical[i] = double4(0, 0, 0, 0);
	}

	findDerivativesOfSpecifiedOrderInNewBasis(
		derivativesA, order, transformAtoB,
		derivativesBtheoretical
	);

	double maxDiscrepancy = 0;
	for (int i = 0; i <= order; i++)
	{
		double4 d = abs(float4(derivativesBtheoretical[i] - derivativesB[i]) / float4(derivativesBtheoretical[i]));
		maxDiscrepancy = max(maxDiscrepancy, max(d.r, max(d.g, d.b)));
	}

	return maxDiscrepancy;
}

double calculateDiscrepancy(
	Texture2DArray<float> Ar,
	Texture2DArray<float> Ag,
	Texture2DArray<float> Ab,
	Texture2DArray<float> Aa,
	int2 posInA,
	Texture2DArray<float> Br,
	Texture2DArray<float> Bg,
	Texture2DArray<float> Bb,
	Texture2DArray<float> Ba,
	int2 posInB,
	double2x2 transformAtoB,
	double specCoeffB
)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;
	double maxDiscrepancy = 0;
	for (int order = 0; order <= maxOrderOfDerivatives; order++)
	{
		double d = calculateDiscrepancyOfSpecifiedOrder(
			Ar, Ag, Ab, Aa, posInA,
			Br, Bg, Bb, Ba, posInB,
			transformAtoB, specCoeffB,
			order
		);
		maxDiscrepancy = max(d, maxDiscrepancy);
	}
	return maxDiscrepancy;
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
	int width_ = width - 2 * maxOrderOfDerivatives;
	int height_ = height - 2 * maxOrderOfDerivatives;

	int2 posInA = int2(dispatchThreadID.x % width_, dispatchThreadID.x / width_);
	if (posInA.y > height_ - 1)
		return;
	posInA.x += maxOrderOfDerivatives;
	posInA.y += maxOrderOfDerivatives;

	int2 posInB = int2(dispatchThreadID.y % width_, dispatchThreadID.y / width_);
	if (posInB.y > height_ - 1)
		return;
	posInB.x += maxOrderOfDerivatives;
	posInB.y += maxOrderOfDerivatives;

	uint original_value;
	InterlockedExchange(mapAtoB[posInA].r, UINT_MAX, original_value);

	double2x2 transformAtoB;
	double specCoeffB;
	calculateInitialTransformParams(
		Br, Bg, Bb, Ba, posInB,
		Ar, Ag, Ab, Aa, posInA,
		transformAtoB, specCoeffB
	);

	uint err = 1000000 * calculateDiscrepancy(
		Ar, Ag, Ab, Aa, posInA,
		Br, Bg, Bb, Ba, posInB,
		transformAtoB, specCoeffB
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
	int width_ = width - 2 * maxOrderOfDerivatives;
	int height_ = height - 2 * maxOrderOfDerivatives;

	int2 posInA = int2(dispatchThreadID.x % width_, dispatchThreadID.x / width_);
	if (posInA.y > height_ - 1)
		return;
	posInA.x += maxOrderOfDerivatives;
	posInA.y += maxOrderOfDerivatives;

	int2 posInB = int2(dispatchThreadID.y % width_, dispatchThreadID.y / width_);
	if (posInB.y > height_ - 1)
		return;
	posInB.x += maxOrderOfDerivatives;
	posInB.y += maxOrderOfDerivatives;

	float2x2 transformAtoB;
	float specCoeffB;
	calculateInitialTransformParams(
		Br, Bg, Bb, Ba, posInB,
		Ar, Ag, Ab, Aa, posInA,
		transformAtoB, specCoeffB
	);

	uint err = 1000000 * calculateDiscrepancy(
		Ar, Ag, Ab, Aa, posInA,
		Br, Bg, Bb, Ba, posInB,
		transformAtoB, specCoeffB
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