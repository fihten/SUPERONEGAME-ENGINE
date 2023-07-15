
float4x4 WVP;

float4 VS(float3 posL : POSITION) : SV_POSITION
{
	float4 posH = mul(float4(posL, 1.0f), WVP);
	return posH;
}

float4 PS() : SV_TARGET
{
	return float4(1, 0, 0, 1);
}

technique11 SelectedObject
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};