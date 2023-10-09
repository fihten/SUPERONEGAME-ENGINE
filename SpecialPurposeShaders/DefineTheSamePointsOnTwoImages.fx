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
	out float2x2 transformBtoA,
	out float specCoeffA
)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;

	float4 a00 = get(Ar, Ag, Ab, Aa, uint3(posInA, 0));
	float4 a10 = get(Ar, Ag, Ab, Aa, uint3(posInA, 1));
	float4 a01 = get(Ar, Ag, Ab, Aa, uint3(posInA, maxOrderOfDerivatives + 1));

	float4 b00 = get(Br, Bg, Bb, Ba, uint3(posInB, 0));
	float4 b10 = get(Br, Bg, Bb, Ba, uint3(posInB, 1));
	float4 b01 = get(Br, Bg, Bb, Ba, uint3(posInB, maxOrderOfDerivatives + 1));

	specCoeffA = dot(a00.xyz, b00.xyz) / dot(a00.xyz, a00.xyz);
	a10 *= specCoeffA;
	a01 *= specCoeffA;

	float l00 = dot(b10.xyz, b10.xyz);
	float l01 = dot(b10.xyz, b01.xyz);
	float l10 = l01;
	float l11 = dot(b01.xyz, b01.xyz);

	float r0 = dot(a10.xyz, b10.xyz);
	float r1 = dot(a10.xyz, b01.xyz);

	float d = l00 * l11 - l01 * l10;
	float d0 = r0 * l11 - l01 * r1;
	float d1 = l00 * r1 - r0 * l10;

	float mxx = d != 0 ? d0 / d : 1;
	float mxy = d != 0 ? d1 / d : 0;

	r0 = dot(b10.xyz, a01.xyz);
	r1 = dot(a01.xyz, b01.xyz);

	d = l00 * l11 - l01 * l10;
	d0 = r0 * l11 - l01 * r1;
	d1 = l00 * r1 - r0 * l10;

	float myx = d != 0 ? d0 / d : 0;
	float myy = d != 0 ? d1 / d : 1;

	transformBtoA = float2x2(
		mxx, mxy,
		myx, myy
		);
}

int binomial_coefficient(int n, int k)
{
	int a = 1;
	int b = 1;
	for (int i = 1; i <= k; i++)
	{
		a *= n + 1 - i;
		b *= i;
	}
	return a / b;
}

float4 differential(
	Texture2DArray<float> r,
	Texture2DArray<float> g,
	Texture2DArray<float> b,
	Texture2DArray<float> a,
	int2 ij, float specCoeff,
	int order, float dx, float dy
)
{
	float4 ret = 0;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	r.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;

	int binomial = 1;
	for (int k = 0; k <= order; k++)
	{
		int l = order - k;
		int index = l * (maxOrderOfDerivatives + 1) + k;

		float4 derivative_k_l = specCoeff * get(r, g, b, a, uint3(ij, index));
		ret += binomial * derivative_k_l * pow(dx, k) * pow(dy, l);

		binomial *= l;
		binomial /= k + 1;
	}
	return ret;
}

// c*(mxx*x+myx*y)^k*(mxy*x+myy*y)^l
float findCoefficientsNearXofSpecifiedDegree(
	int numerator, int denominator, 
	float mxx, float myx, 
	float mxy, float myy, 
	int k, int l, 
	int degree
)
{
	int degree_factorial = 1;
	int i = 1;
	for (; i <= degree; i++)
		degree_factorial *= i;

	float coefficient = 0;

	// i <= k && i >= 0 && degree - i <= l && degree - i >= 0
	// ->
	// max(0, degree - l) <= i <= min(k, degree)
	int i0 = max(0, degree - l);
	int i1 = min(k, degree);

	int k_factorial = 1;
	float pow_myx = 1;
	i = 1;
	for (; i < k - i0 + 1; i++)
		pow_myx *= myx;
	float div_myx = myx != 0 ? 1 / myx : 0;
	for (; i <= k; i++)
		k_factorial *= i;

	int l_factorial = 1;
	float pow_myy = 1;
	i = 1;
	for (; i < l - degree + i0 + 1; i++)
		pow_myy *= myy;
	float pow_mxy = 1;
	for (; i <= l; i++)
	{
		pow_mxy *= mxy;
		l_factorial *= i;
	}
	float div_mxy = mxy != 0 ? 1 / mxy : 0;

	int b = 1;
	float pow_mxx = 1;
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
		
		float fc = pow_myx * pow_myy * pow_mxx * pow_mxy;
		int ic = (b * k_factorial * l_factorial * numerator) / denominator / degree_factorial;
		coefficient += ic * fc;

		b *= j;
		b /= i + 1;

		k_factorial *= k - i;
		l_factorial /= l - j + 1;

		pow_myx *= div_myx;
		pow_myy *= myy;
		pow_mxx *= mxx;
		pow_mxy *= div_mxy;
	}
	return coefficient;
}

#define max_count_of_derivatives 20
void findDerivativesOfSpecifiedOrderInNewBasis(
	float4 oldDerivatives[max_count_of_derivatives], int order, float2x2 transformToNewBasis,
	out float4 newDerivatives[max_count_of_derivatives]
)
{
	int b_new = 1;
	for (int degreeOfXinNewBasis = 0; degreeOfXinNewBasis <= order; degreeOfXinNewBasis++)
	{
		int b_old = 1;
		newDerivatives[degreeOfXinNewBasis] = float4(0, 0, 0, 0);
		for (int degreeOfXinOldBasis = 0; degreeOfXinOldBasis <= order; degreeOfXinOldBasis++)
		{
			int degreeOfYinOldBasis = order - degreeOfXinOldBasis;
			float coeff = findCoefficientsNearXofSpecifiedDegree(
					b_old, b_new,
					transformToNewBasis[0][0], transformToNewBasis[1][0],
					transformToNewBasis[0][1], transformToNewBasis[1][1],
					degreeOfXinOldBasis, degreeOfYinOldBasis,
					degreeOfXinNewBasis
				);
			newDerivatives[degreeOfXinNewBasis] += coeff * oldDerivatives[degreeOfXinOldBasis];

			b_old *= degreeOfYinOldBasis;
			b_old /= degreeOfXinOldBasis + 1;
		}

		int degreeOfYinNewBasis = order - degreeOfXinNewBasis;
		b_new *= degreeOfYinNewBasis;
		b_new /= degreeOfXinNewBasis + 1;
	}
}

float calculateDiscrepancyOfSpecifiedOrder(
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
	float2x2 transformAtoB,
	float specCoeffB,
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

	float4 derivativesA[max_count_of_derivatives];
	float4 derivativesB[max_count_of_derivatives];
	float4 derivativesBtheoretical[max_count_of_derivatives];
	for (int i = 0; i <= order; i++)
	{
		int j = order - i;
		int index = j * (maxOrderOfDerivatives + 1) + i;

		derivativesA[i] = get(Ar, Ag, Ab, Aa, uint3(posInA, index));
		derivativesB[i] = specCoeffB * get(Br, Bg, Bb, Ba, uint3(posInB, index));
		derivativesBtheoretical[i] = float4(0, 0, 0, 0);
	}

	findDerivativesOfSpecifiedOrderInNewBasis(
		derivativesA, order, transformAtoB,
		derivativesBtheoretical
	);

	float maxDiscrepancy = 0;
	for (int i = 0; i <= order; i++)
	{
		float4 d = abs((derivativesBtheoretical[i] - derivativesB[i]) / derivativesBtheoretical[i]);
		maxDiscrepancy = max(maxDiscrepancy, max(d.r, max(d.g, d.b)));
	}

	return maxDiscrepancy;
}

float calculateDiscrepancy(
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
	float2x2 transformAtoB,
	float specCoeffB
)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	Ar.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;
	float maxDiscrepancy = 0;
	for (int order = 0; order <= maxOrderOfDerivatives; order++)
	{
		float d = calculateDiscrepancyOfSpecifiedOrder(
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