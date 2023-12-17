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
	float scale : SCALE;
};

struct VertexOut
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
	float4 scale : SCALE;
};

struct GeometryOut
{
	float4 posH : SV_POSITION;
	float3 color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.posW = vin.posW;
	vout.axis0 = vin.axis0;
	vout.axis1 = vin.axis1;
	vout.axis2 = vin.axis2;
	vout.scale = vin.scale;
	return vout;
}

[maxvertexcount(6)]
void GS(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
	GeometryOut gout;

	float3 pt = gin[0].posW + gin[0].scale * gin[0].axis0;
	
	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	gout.color = float4(1, 0, 0, 1);
	lineStream.Append(gout);

	pt = gin[0].posW - gin[0].scale * gin[0].axis0;
	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();
	
	////////////////////////////////////////////////

	pt = gin[0].posW + gin[0].scale * gin[0].axis1;

	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	gout.color = float4(0, 1, 0, 1);
	lineStream.Append(gout);

	pt = gin[0].posW - gin[0].scale * gin[0].axis1;
	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	////////////////////////////////////////////////

	pt = gin[0].posW + gin[0].scale * gin[0].axis2;

	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	gout.color = float4(0, 0, 1, 1);
	lineStream.Append(gout);

	pt = gin[0].posW - gin[0].scale * gin[0].axis2;
	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();
}

float4 PS(GeometryOut pin) : SV_TARGET
{
	return float4(pin.color,1);
}

technique11 FrameOfReference
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};