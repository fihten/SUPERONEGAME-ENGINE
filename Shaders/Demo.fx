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
	vout.posH = mul(float4(vin.posL,1), WVP);
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