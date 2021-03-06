cbuffer cbPerObject
{
	float4x4 WVP;
}

void VSdemo(float3 iPosL:POSITION, float4 iColor:COLOR, out float4 oPosH:SV_POSITION, out float4 oColor:COLOR)
{
	float i = (k * 3 + 1) * 5+10;
	oPosH = mul(float4(iPosL,1), WVP);
	oColor = iColor;	
}

float4 PSdemo(float4 posH:SV_POSITION, float4 color:COLOR):SV_Target
{
	return color;
}

technique11 Demo
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSdemo()));
		SetPixelShader(CompileShader(ps_5_0, PSdemo()));
	}
}
