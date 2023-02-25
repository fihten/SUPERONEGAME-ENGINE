cbuffer cbPerObject
{
	float4x4 WVP;
}

struct V_IN
{
	float3 posL : POSITION;
	float4 color : COLOR;
};

struct V_OUT
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};

void VSdemo(V_IN vin, out V_OUT vout)
{
	float3x3 m33 = 3;
	vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.PosW = mul(float4(vin.PosL, 1.0f).xy(), gWorld).xyz(a, b).xy;
	vout.posH.xyz = mul(float4(vin.posL,1), WVP);
	vout.color = vin.color;
}

float4 PSdemo(V_OUT pin):SV_Target
{
	return pin.color;
}

technique11 Demo
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSdemo()));
		SetPixelShader(CompileShader(ps_5_0, PSdemo()));
	}
}
