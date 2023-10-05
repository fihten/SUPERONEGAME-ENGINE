int maxOrderOfDerivatives;
int orderOfDerivative;

Texture2D<float4> tex;

RWTexture2DArray<float> r;
RWTexture2DArray<float> g;
RWTexture2DArray<float> b;
RWTexture2DArray<float> a;

void set(
	RWTexture2DArray<float> coeffs_r,
	RWTexture2DArray<float> coeffs_g,
	RWTexture2DArray<float> coeffs_b,
	RWTexture2DArray<float> coeffs_a,
	uint3 pos,
	float4 value
	)
{
	coeffs_r[pos].r = value.r;
	coeffs_g[pos].r = value.g;
	coeffs_b[pos].r = value.b;
	coeffs_a[pos].r = value.a;
}

float4 get(
	RWTexture2DArray<float> coeffs_r,
	RWTexture2DArray<float> coeffs_g,
	RWTexture2DArray<float> coeffs_b,
	RWTexture2DArray<float> coeffs_a,
	uint3 pos
)
{
	float4 res;
	res.r = coeffs_r[pos].r;
	res.g = coeffs_g[pos].r;
	res.b = coeffs_b[pos].r;
	res.a = coeffs_a[pos].r;
	return res;
}

[numthreads(32, 32, 1)]
void CS_U_AXIS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint2 ij = dispatchThreadID.xy;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint mips = 0;
	tex.GetDimensions(mip, width, height, mips);

	if (ij.x > width - 2 * orderOfDerivative - 1)
		return;
	if (ij.y > height - 2 * orderOfDerivative - 1)
		return;

	ij.x += orderOfDerivative;
	ij.y += orderOfDerivative;

	float4 c = tex[ij];
	if (orderOfDerivative == 0)
	{
		set(r, g, b, a, uint3(ij, 0), c);
		return;
	}

	uint2 ij_prev = ij;
	ij_prev.x -= 1;
	float4 c_prev = get(r, g, b, a, uint3(ij_prev, orderOfDerivative - 1));

	uint2 ij_next = ij;
	ij_next.x += 1;
	float4 c_next = get(r, g, b, a, uint3(ij_next, orderOfDerivative - 1));

	float4 derivative = 0.5 * (c_next - c_prev);
	set(r, g, b, a, uint3(ij, orderOfDerivative), derivative);
}

[numthreads(16, 16, 4)]
void CS_V_AXIS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint2 ij = dispatchThreadID.xy;
	uint col = dispatchThreadID.z;

	uint width = 0;
	uint height = 0;
	uint elements = 0;
	r.GetDimensions(width, height, elements);

	if (ij.x > width - 2 * orderOfDerivative - 1)
		return;
	if (ij.y > height - 2 * orderOfDerivative - 1)
		return;
	if (col > maxOrderOfDerivatives)
		return;

	ij.x += orderOfDerivative;
	ij.y += orderOfDerivative;

	uint2 ij_prev = ij;
	ij_prev.y -= 1;
	float4 c_prev = get(r, g, b, a, uint3(ij_prev, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col));

	uint2 ij_next = ij;
	ij_next.y += 1;
	float4 c_next = get(r, g, b, a, uint3(ij_next, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col));

	float4 derivative = 0.5 * (c_next - c_prev);
	set(r, g, b, a, uint3(ij, (maxOrderOfDerivatives + 1) * orderOfDerivative + col), derivative);
}

technique11 CalculateTextureDerivative
{
	pass AlongAxisU
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_U_AXIS()));
	}
	pass AlongAxisV
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_V_AXIS()));
	}
};