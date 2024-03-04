cbuffer cbPerInstance
{
	float4x4 wvp;
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

	vout.posH = mul(float4(vin.posL, 1.0f), wvp);
	vout.normalW = mul(vin.normalL, wInvTr);

	return vout;
}

float4 ps(VertexOut pin) : SV_TARGET
{
	float4 geoColor = float4(0.1f,0.1f,0.1f,0.0f);
	float4 lightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);

	float cosFactor = -dot(eyeDirW, pin.normalW);

	float4 resColor = 0.0f;
	resColor += geoColor * lightColor * cosFactor;

	float3 reflected = reflect(pin.normalW, eyeDirW);

	float3 pt = pin.posH.xyz / pin.posH.w;
	float3 toEye = eyePosW - pt;
	normalize(toEye);

	cosFactor = dot(reflected, toEye);

	resColor += geoColor * lightColor * cosFactor;

	return resColor;
}

technique11 GeometryOfEditor
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, vs()));
		SetPixelShader(CompileShader(ps_5_0, ps()));
	}
}