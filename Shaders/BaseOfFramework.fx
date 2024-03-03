cbuffer cbPerFrame
{
	float4x4 gViewProj;
};

struct VertexIn
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
};

struct VertexOut
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
};

struct GeometryOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
	float alpha : ALPHA;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.posW = vin.posW;
	vout.axis0 = vin.axis0;
	vout.axis1 = vin.axis1;
	vout.axis2 = vin.axis2;
	return vout;
}

[maxvertexcount(84)]
void GS(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
	int n = 10;
	float h = 0.1f;
	
	for (int i = -n; i < n + 1; ++i)
	{
		float alpha = (float)(i + n) / (2 * n);

		float3 posW = gin[0].posW - n * h * gin[0].axis1 + i * h * gin[0].axis0;
		
		GeometryOut gout;
		
		gout.posH = mul(float4(posW, 1.0f), gViewProj);
		gout.color = float4(0.0f, 1.0f, 0.0f, 0.0f);
		gout.alpha = alpha;
		lineStream.Append(gout);

		posW = gin[0].posW + n * h * gin[0].axis1 + i * h * gin[0].axis0;

		gout.posH = mul(float4(posW, 1.0f), gViewProj);
		gout.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
		gout.alpha = alpha;
		lineStream.Append(gout);

		lineStream.RestartStrip();
	}

	for (int i = -n; i < n + 1; ++i)
	{
		float alpha = (float)(i + n) / (2 * n);

		float3 posW = gin[0].posW - n * h * gin[0].axis0 + i * h * gin[0].axis1;

		GeometryOut gout;

		gout.posH = mul(float4(posW, 1.0f), gViewProj);
		gout.color = float4(0.0f, 1.0f, 0.0f, 0.0f);
		gout.alpha = alpha;
		lineStream.Append(gout);

		posW = gin[0].posW + n * h * gin[0].axis0 + i * h * gin[0].axis1;

		gout.posH = mul(float4(posW, 1.0f), gViewProj);
		gout.color = float4(0.0f, 1.0f, 0.0f, 1.0f);
		gout.alpha = alpha;
		lineStream.Append(gout);

		lineStream.RestartStrip();
	}
}

float4 PS(GeometryOut pin) : SV_TARGET
{
	float a = pin.color.w;
	pin.color.w = pow(sin(3.14 * a), 2);

	a = pin.alpha;
	pin.color.w *= pow(sin(3.14 * a), 2);

	pin.color.w = 1 - pin.color.w;

	return pin.color;
}

technique11 BaseOfFramework
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};