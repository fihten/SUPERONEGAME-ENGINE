cbuffer cbPerFrame
{
	float4 color;
	float4x4 areaTransform;
};

float4 vs(float4 ndc : NDC):SV_POSITION
{
	return mul(ndc, areaTransform);
}

float4 ps() : SV_TARGET
{
	return color;
}

technique11 AreaOfSelection
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, vs()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ps()));
	}
};