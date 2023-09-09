Texture2D tex;
RWTexture2DArray<float4> coeffs;

[numthreads(32, 32, 1)]
void CS_U_AXIS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint2 ij = dispatchThreadID.xy;

	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint mips = 0;
	tex.GetDimensions(mip, width, height, mips);

	if (ij.x >= width)
		return;
	if (ij.y >= height)
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

	coeffs[0] = f0;
	coeffs[1] = f01;
	coeffs[2] = 0.5 * f02;
	
}