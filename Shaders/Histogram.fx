float4 vs(float2 pos : Position) : SV_Position
{
	return float4(pos, 0, 1);
}

float4 ps() : SV_Target
{
	return float4(1,0,0,1);
}

technique11 histogram
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, vs()));
		SetPixelShader(CompileShader(ps_5_0, ps()));
	}
};