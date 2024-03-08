cbuffer cbPerInstance
{
	float4x4 w;
	float4x4 vp;
	float4x4 wInvTr;
	float3 eyeDirW;
	float3 eyePosW;
};

struct VertexIn
{
	float3 posL : POSITION;
	float3 normalL : NORMAL;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float3 normalW : NORMAL;
};

VertexOut vs(VertexIn vin)
{
	VertexOut vout;
	
	float4x4 wvp = mul(w, vp);
	vout.posH = mul(float4(vin.posL, 1.0f), wvp);
	vout.normalW = mul(vin.normalL, wInvTr);

	return vout;
}

float4 ps(VertexOut pin) : SV_TARGET
{
	float3 normal = normalize(pin.normalW);

	float4 geoColor = float4(0.5f,0.5f,0.5f,0.0f);
	float4 lightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);

	float cosFactor = -dot(eyeDirW, normal);
	cosFactor = saturate(cosFactor);

	float4 resColor = geoColor * lightColor * cosFactor;

	return resColor;
}

technique11 GeometryOfEditor
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, vs()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, ps()));
	}
}