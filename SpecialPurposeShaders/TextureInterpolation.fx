#define COLUMNS 6

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

	if (ij.x > width - 1)
		return;
	if (ij.y > height - 1)
		return;

	float4 c = tex[ij];

	uint2 ij_prev_prev = ij;
	ij_prev_prev.x = max(ij.x - 2, 0);
	float4 c_prev_prev = tex[ij_prev_prev];

	uint2 ij_prev = ij;
	ij_prev.x = max(ij.x - 1, 0);
	float4 c_prev = tex[ij_prev];

	uint2 ij_next = ij;
	ij_next.x = min(ij_next.x + 1, width - 1);
	float4 c_next = tex[ij_next];

	uint2 ij_next_next = ij;
	ij_next_next.x = min(ij_next_next.x + 2, width - 1);
	float4 c_next_next = tex[ij_next_next];

	float4 f0 = c;
	float4 f1 = c_next;

	bool4 extremum0 = bool4(
		(c_prev.x - c.x) * (c_next.x - c.x) >= 0,
		(c_prev.y - c.y) * (c_next.y - c.y) >= 0,
		(c_prev.z - c.z) * (c_next.z - c.z) >= 0,
		(c_prev.w - c.w) * (c_next.w - c.w) >= 0
		);
	float4 f01 = float4(
		extremum0.x ? 0 : 0.5 * (c_next.x - c_prev.x),
		extremum0.y ? 0 : 0.5 * (c_next.y - c_prev.y),
		extremum0.z ? 0 : 0.5 * (c_next.z - c_prev.z),
		extremum0.w ? 0 : 0.5 * (c_next.w - c_prev.w)
		);
	float4 f02 = c_prev + c_next - 2 * c;

	bool4 extremum1 = bool4(
		(c_next_next.x - c_next.x) * (c.x - c_next.x) >= 0,
		(c_next_next.y - c_next.y) * (c.y - c_next.y) >= 0,
		(c_next_next.z - c_next.z) * (c.z - c_next.z) >= 0,
		(c_next_next.w - c_next.w) * (c.w - c_next.w) >= 0
		);
	float4 f11 = float4(
		extremum1.x ? 0 : 0.5 * (c_next_next.x - c.x),
		extremum1.y ? 0 : 0.5 * (c_next_next.y - c.y),
		extremum1.z ? 0 : 0.5 * (c_next_next.z - c.z),
		extremum1.w ? 0 : 0.5 * (c_next_next.w - c.w)
		);
	float4 f12 = c + c_next_next - 2 * c_next;

	set(r, g, b, a, uint3(ij, 0), f0);
	set(r, g, b, a, uint3(ij, 1), f01);
	set(r, g, b, a, uint3(ij, 2), 0.5 * f02);
	set(r, g, b, a, uint3(ij, 3), 10 * f1 - 10 * f0 - 6 * f01 - 1.5 * f02 - 4 * f11 + 0.5 * f12);
	set(r, g, b, a, uint3(ij, 4), -15 * f1 + 15 * f0 + 8 * f01 + 7 * f11 + 1.5 * f02 - f12);
	set(r, g, b, a, uint3(ij, 5), 6 * f1 - 6 * f0 - 3 * f01 - 3 * f11 - 0.5 * f02 + 0.5 * f12);
}

[numthreads(16, 16, 4)]
void CS_V_AXIS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint3 ijk = dispatchThreadID.xyz;
	uint col = dispatchThreadID.z;

	uint width = 0;
	uint height = 0;
	uint elements = 0;
	r.GetDimensions(width, height, elements);

	if (ijk.x > width - 1)
		return;
	if (ijk.y > height - 1)
		return;
	if (col > 5)
		return;

	float4 a_ = get(r, g, b, a, ijk);

	uint3 ijk_prev_prev = ijk;
	ijk_prev_prev.y = max(ijk.y - 2, 0);
	float4 a_prev_prev = get(r, g, b, a, ijk_prev_prev); 

	uint3 ijk_prev = ijk;
	ijk_prev.y = max(ijk.y - 1, 0);
	float4 a_prev = get(r, g, b, a, ijk_prev); 

	uint3 ijk_next = ijk;
	ijk_next.y = min(ijk_next.y + 1, height - 1);
	float4 a_next = get(r, g, b, a, ijk_next); 

	uint3 ijk_next_next = ijk;
	ijk_next_next.y = min(ijk_next_next.y + 2, height - 1);
	float4 a_next_next = get(r, g, b, a, ijk_next_next); 

	float4 f0 = a_;
	float4 f1 = a_next;

	bool4 extremum0 = bool4(
		(a_prev.x - a_.x) * (a_next.x - a_.x) >= 0,
		(a_prev.y - a_.y) * (a_next.y - a_.y) >= 0,
		(a_prev.z - a_.z) * (a_next.z - a_.z) >= 0,
		(a_prev.w - a_.w) * (a_next.w - a_.w) >= 0
		);
	float4 f01 = float4(
		extremum0.x ? 0 : 0.5 * (a_next.x - a_prev.x),
		extremum0.y ? 0 : 0.5 * (a_next.y - a_prev.y),
		extremum0.z ? 0 : 0.5 * (a_next.z - a_prev.z),
		extremum0.w ? 0 : 0.5 * (a_next.w - a_prev.w)
		);
	float4 f02 = a_prev + a_next - 2 * a_;

	bool4 extremum1 = bool4(
		(a_next_next.x - a_next.x) * (a_.x - a_next.x) >= 0,
		(a_next_next.y - a_next.y) * (a_.y - a_next.y) >= 0,
		(a_next_next.z - a_next.z) * (a_.z - a_next.z) >= 0,
		(a_next_next.w - a_next.w) * (a_.w - a_next.w) >= 0
		);
	float4 f11 = float4(
		extremum1.x ? 0 : 0.5 * (a_next_next.x - a_.x),
		extremum1.y ? 0 : 0.5 * (a_next_next.y - a_.y),
		extremum1.z ? 0 : 0.5 * (a_next_next.z - a_.z),
		extremum1.w ? 0 : 0.5 * (a_next_next.w - a_.w)
		);
	float4 f12 = a_ + a_next_next - 2 * a_next;

	uint2 ij = ijk.xy;
	set(r, g, b, a, uint3(ij, 1 * COLUMNS + col), f01);
	set(r, g, b, a, uint3(ij, 2 * COLUMNS + col), 0.5 * f02);
	set(r, g, b, a, uint3(ij, 3 * COLUMNS + col), 10 * f1 - 10 * f0 - 6 * f01 - 1.5 * f02 - 4 * f11 + 0.5 * f12);
	set(r, g, b, a, uint3(ij, 4 * COLUMNS + col), -15 * f1 + 15 * f0 + 8 * f01 + 7 * f11 + 1.5 * f02 - f12);
	set(r, g, b, a, uint3(ij, 5 * COLUMNS + col), 6 * f1 - 6 * f0 - 3 * f01 - 3 * f11 - 0.5 * f02 + 0.5 * f12);
}

technique11 TextureInterpolation
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