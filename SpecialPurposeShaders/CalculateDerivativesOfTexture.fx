int maxOrderOfDerivatives;
int orderOfDerivative;

Texture2D<float4> tex;

RWTexture2DArray<float> r;
RWTexture2DArray<float> g;
RWTexture2DArray<float> b;
RWTexture2DArray<float> a;

RWTexture2DArray<float> r_tmp;
RWTexture2DArray<float> g_tmp;
RWTexture2DArray<float> b_tmp;
RWTexture2DArray<float> a_tmp;

// x - d Surf(x0, y0) / dx
// y - d Surf(x0, y0) / dy
// z - Surf(x0, y0)
float3 lsm(float values[9])
{
	float x[9] = { -1,0,1,-1,0,1,-1,0,1 };
	float y[9] = { -1,-1,-1,0,0,0,1,1,1 };

	float3 L[3] = { float3(0,0,0),float3(0,0,0),float3(0,0,0) };
	float3 R = float3(0, 0, 0);
	for (int i = 0; i < 9; i++)
	{
		L[0].x += x[i] * x[i];
		L[0].y += x[i] * y[i];
		L[0].z += x[i];

		L[1].x += x[i] * y[i];
		L[1].y += y[i] * y[i];
		L[1].z += y[i];

		L[2].x += x[i];
		L[2].y += y[i];
		L[2].z += 1;

		R.x += value[i] * x[i];
		R.y += value[i] * y[i];
		R.z += value[i];
	}

	float d = dot(L[0], cross(L[1], L[2]));
	float d0 = dot(R, cross(L[1], L[2]));
	float d1 = dot(L[0], cross(R, L[2]));
	float d2 = dot(L[0], cross(L[1], R));

	return float3(d0 / d, d1 / d, d2 / d);
}

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
void CS_U_AXIS_at_first(uint3 dispatchThreadID : SV_DispatchThreadID)
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

	uint3 ij_s[9] = {
		uint3(ij.x - 1, ij.y - 1, orderOfDerivative - 1),
		uint3(ij.x, ij.y - 1, orderOfDerivative - 1),
		uint3(ij.x + 1, ij.y - 1, orderOfDerivative - 1),
		uint3(ij.x - 1, ij.y, orderOfDerivative - 1),
		uint3(ij.x, ij.y, orderOfDerivative - 1),
		uint3(ij.x + 1, ij.y, orderOfDerivative - 1),
		uint3(ij.x - 1, ij.y + 1, orderOfDerivative - 1),
		uint3(ij.x, ij.y + 1, orderOfDerivative - 1),
		uint3(ij.x + 1, ij.y + 1, orderOfDerivative - 1)
	};

	float crs[9];
	float cgs[9];
	float cbs[9];
	for (int i = 0; i < 9; i++)
	{
		float4 color = get(r, g, b, a, ij_s[i]);
		crs[i] = color.r;
		cgs[i] = color.g;
		cbs[i] = color.b;
	}

	float4 derivative;
	derivative.x = lsm(crs).x;
	derivative.y = lsm(cgs).x;
	derivative.z = lsm(cbs).x;
	derivative.w = 0;

	set(r, g, b, a, uint3(ij, orderOfDerivative), derivative);
}

[numthreads(16, 16, 2)]
void CS_U_AXIS_at_the_second(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint2 ij = dispatchThreadID.xy;
	uint row = dispatchThreadID.z;

	uint width = 0;
	uint height = 0;
	uint elements = 0;
	r.GetDimensions(width, height, elements);

	if (ij.x > width - 2 * orderOfDerivative - 1)
		return;
	if (ij.y > height - 2 * orderOfDerivative - 1)
		return;
	if (row > maxOrderOfDerivatives)
		return;

	ij.x += orderOfDerivative;
	ij.y += orderOfDerivative;

	uint3 ij_s[9] = {
		uint3(ij.x - 1, ij.y - 1, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1),
		uint3(ij.x, ij.y - 1, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1),
		uint3(ij.x + 1, ij.y - 1, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1),
		uint3(ij.x - 1, ij.y, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1),
		uint3(ij.x, ij.y, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1),
		uint3(ij.x + 1, ij.y, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1),
		uint3(ij.x - 1, ij.y + 1, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1),
		uint3(ij.x, ij.y + 1, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1),
		uint3(ij.x + 1, ij.y + 1, (maxOrderOfDerivatives + 1) * row + orderOfDerivative - 1)
	};

	float crs[9];
	float cgs[9];
	float cbs[9];
	for (int i = 0; i < 9; i++)
	{
		float4 color = get(r_tmp, g_tmp, b_tmp, a_tmp, ij_s[i]);
		crs[i] = color.r;
		cgs[i] = color.g;
		cbs[i] = color.b;
	}

	float4 derivative;
	derivative.x = lsm(crs).x;
	derivative.y = lsm(cgs).x;
	derivative.z = lsm(cbs).x;
	derivative.w = 0;

	set(r_tmp, g_tmp, b_tmp, a_tmp, uint3(ij, (maxOrderOfDerivatives + 1) * row + orderOfDerivative), derivative);
}

[numthreads(16, 16, 4)]
void CS_V_AXIS_at_first(uint3 dispatchThreadID : SV_DispatchThreadID)
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

	uint3 ij_s[9] = {
		uint3(ij.x - 1, ij.y - 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col),
		uint3(ij.x, ij.y - 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col),
		uint3(ij.x + 1, ij.y - 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col),
		uint3(ij.x - 1, ij.y, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col),
		uint3(ij.x, ij.y, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col),
		uint3(ij.x + 1, ij.y, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col),
		uint3(ij.x - 1, ij.y + 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col),
		uint3(ij.x, ij.y + 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col),
		uint3(ij.x + 1, ij.y + 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1) + col)
	};

	float crs[9];
	float cgs[9];
	float cbs[9];
	for (int i = 0; i < 9; i++)
	{
		float4 color = get(r, g, b, a, ij_s[i]);
		crs[i] = color.r;
		cgs[i] = color.g;
		cbs[i] = color.b;
	}

	float4 derivative;
	derivative.x = lsm(crs).y;
	derivative.y = lsm(cgs).y;
	derivative.z = lsm(cbs).y;
	derivative.w = 0;

	set(r, g, b, a, uint3(ij, (maxOrderOfDerivatives + 1) * orderOfDerivative + col), derivative);
}

[numthreads(32, 32, 1)]
void CS_V_AXIS_at_the_second(uint3 dispatchThreadID : SV_DispatchThreadID)
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
		set(r_tmp, g_tmp, b_tmp, a_tmp, uint3(ij, 0), c);
		return;
	}

	uint3 ij_s[9] = {
		uint3(ij.x - 1, ij.y - 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1)),
		uint3(ij.x, ij.y - 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1)),
		uint3(ij.x + 1, ij.y - 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1)),
		uint3(ij.x - 1, ij.y, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1)),
		uint3(ij.x, ij.y, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1)),
		uint3(ij.x + 1, ij.y, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1)),
		uint3(ij.x - 1, ij.y + 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1)),
		uint3(ij.x, ij.y + 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1)),
		uint3(ij.x + 1, ij.y + 1, (maxOrderOfDerivatives + 1) * (orderOfDerivative - 1))
	};

	float crs[9];
	float cgs[9];
	float cbs[9];
	for (int i = 0; i < 9; i++)
	{
		float4 color = get(r_tmp, g_tmp, b_tmp, a_tmp, ij_s[i]);
		crs[i] = color.r;
		cgs[i] = color.g;
		cbs[i] = color.b;
	}

	float4 derivative;
	derivative.x = lsm(crs).y;
	derivative.y = lsm(cgs).y;
	derivative.z = lsm(cbs).y;
	derivative.w = 0;

	set(r_tmp, g_tmp, b_tmp, a_tmp, uint3(ij, (maxOrderOfDerivatives + 1) * orderOfDerivative), derivative);
}

[numthreads(16, 16, 4)]
void CS_AVERAGE_DERIVATIVES_OF_THE_SAME_ORDER(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint3 ijk = dispatchThreadID.xyz;

	uint width = 0;
	uint height = 0;
	uint elements = 0;
	r.GetDimensions(width, height, elements);

	if (ijk.x > width - 1)
		return;
	if (ijk.y > height - 1)
		return;
	if (ijk.z > (maxOrderOfDerivatives + 1) * (maxOrderOfDerivatives + 1))
		return;

	float4 c = get(r, g, b, a, ijk);
	float4 c_tmp = get(r_tmp, g_tmp, b_tmp, a_tmp, ijk);
	float4 c_avg = 0.5f * (c + c_tmp);
	set(r, g, b, a, ijk, c_avg);
}

technique11 CalculateTextureDerivative
{
	pass AlongAxisU_atFirst
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_U_AXIS_at_first()));
	}
	pass AlongAxisV_atFirst
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_V_AXIS_at_first()));
	}
	pass AlongAxisV_atTheSecond
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_V_AXIS_at_the_second()));
	}
	pass AlongAxisU_atTheSecond
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_U_AXIS_at_the_second()));
	}
	pass AveragingDerivativesOfTheSameOrder
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_AVERAGE_DERIVATIVES_OF_THE_SAME_ORDER()));
	}
};