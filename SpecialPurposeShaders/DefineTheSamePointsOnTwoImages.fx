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
	out float2x2 transform,
	out float specCoeff
)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint elements = 0;
	uint mips = 0;
	r.GetDimensions(mip, width, height, elements, mips);

	int maxOrderOfDerivatives = sqrt(elements) - 1;

	float4 a00 = get(Ar, Ag, Ab, Aa, uint3(posInA, 0));
	float4 a10 = get(Ar, Ag, Ab, Aa, uint3(posInA, 1));
	float4 a01 = get(Ar, Ag, Ab, Aa, uint3(posInA, maxOrderOfDerivatives + 1));

	float4 b00 = get(Br, Bg, Bb, Ba, uint3(posInB, 0));
	float4 b10 = get(Br, Bg, Bb, Ba, uint3(posInB, 1));
	float4 b01 = get(Br, Bg, Bb, Ba, uint3(posInB, maxOrderOfDerivatives + 1));

	k = dot(a00.xyz, b00.xyz) / dot(a00.xyz, a00.xyz);
	a10 *= k;
	a01 *= k;

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

	transform = float2x2(
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
	int ij, float specCoeff,
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

	int b = 1;
	for (int k = 0; k <= order; k++)
	{
		int l = order - k;
		int index = l * (maxOrderOfDerivatives + 1) + k;

		float4 derivative_k_l = specCoeff * get(r, g, b, a, uint3(ij, index));
		ret += b * derivative_k_l * pow(dx, k) * pow(dy, l);

		b *= l;
		b /= k + 1;
	}
	return ret;
}

// c*(mxx*x+myx*y)^k*(mxy*x+myy*y)^l
float4 findCoefficientsNearXofSomeDegree(
	float4 c, 
	float mxx, float myx, 
	float mxy, float myy, 
	int k, int l, 
	int degree
)
{
	float4 coefficients = float4(0, 0, 0, 0);

	// i <= k && degree - i <= l && degree - i >= 0
	// ->
	// degree - l <= i <= min(k, degree)
	int i0 = max(0, degree - l);
	int i1 = min(k, degree);

	int k_factorial = 1;
	int i = k - i0 + 1;
	for (; i <= k; i++)
		k_factorial *= i;

	int l_factorial = 1;
	i = l - degree + i0 + 1;
	for (; i <= l; i++)
		l_factorial *= i;

	int b = 1;
	i = 0;
	for (; i < i0; i++)
	{
		b *= degree - i;
		b /= i + 1;
	}

	for (; i <= i1; i++)
	{
		int j = degree - i;
		float4 coeffsToAdd = b * k_factorial * l_factorial * pow(myx, k - i) * pow(myy, l - j) * pow(mxx, i) * pow(mxy, j);
		coefficients += coeffsToAdd;

		b *= j;
		b /= i + 1;

		k_factorial *= k - i;
		l_factorial /= l - j + 1;
	}
	return c * coefficients;
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

	uint wXh = width * height;
	if (dispatchThreadID.x >= wXh)
		return;
	if (dispatchThreadID.y >= wXh)
		return;

	int2 posInA = int2(dispatchThreadID.x % width, dispatchThreadID.x / width);
	int2 posInB = int2(dispatchThreadID.y % width, dispatchThreadID.y / width);
	
	uint original_value;
	InterlockedExchange(mapAtoB[posInA].r, UINT_MAX, original_value);

	float sizeX = size;
	float sizeY = size;
	float4 params = 0;
	float k = 0;
	float discrepancy = 0;
	calculateInitialTransformParams(
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
		params,
		k,
		discrepancy
	);

	uint err = 1000000 * discrepancy;
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

	uint wXh = width * height;
	if (dispatchThreadID.x >= wXh)
		return;
	if (dispatchThreadID.y >= wXh)
		return;

	int2 posInA = int2(dispatchThreadID.x % width, dispatchThreadID.x / width);
	int2 posInB = int2(dispatchThreadID.y % width, dispatchThreadID.y / width);

	float sizeX = size;
	float sizeY = size;
	float4 params = 0;
	float k = 0;
	float discrepancy = 0;
	calculateInitialTransformParams(
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
		params,
		k,
		discrepancy
	);

	uint err = 1000000 * discrepancy;
	if (err == error[posInA].r)
	{
		uint original_value;
		InterlockedExchange(mapAtoB[posInA].r, dispatchThreadID.y, original_value);
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