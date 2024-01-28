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
	float radius : RADIUS;
};

struct VertexOut
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
	float4 radius : RADIUS;
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
	vout.radius = vin.radius;
	return vout;
}

[maxvertexcount(39)]
void GS(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
	int segments = 12;
	float phi = 6.28 / segments;

	GeometryOut gout;

	// plane Oxy
	gout.color = float4(0, 0, 1, 0);
	for (int i = 0; i < segments + 1; ++i)
	{
		gout.posH.xyz = gin[0].posW;
		gout.posH.xyz += gin[0].radius * gin[0].axis0 * cos(i * phi);
		gout.posH.xyz += gin[0].radius * gin[0].axis1 * sin(i * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);
	}
	lineStream.RestartStrip();

	// plane Oxz
	gout.color = float4(0, 1, 0, 0);
	for (int j = 0; j < segments + 1; ++j)
	{
		gout.posH.xyz = gin[0].posW;
		gout.posH.xyz += gin[0].radius * gin[0].axis0 * cos(j * phi);
		gout.posH.xyz += gin[0].radius * gin[0].axis2 * sin(j * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);
	}
	lineStream.RestartStrip();

	// plane Oyz
	gout.color = float4(1, 0, 0, 0);
	for (int k = 0; k < segments + 1; ++k)
	{
		gout.posH.xyz = gin[0].posW;
		gout.posH.xyz += gin[0].radius * gin[0].axis1 * cos(k * phi);
		gout.posH.xyz += gin[0].radius * gin[0].axis2 * sin(k * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);
	}
	lineStream.RestartStrip();
}

float4 PS(GeometryOut pin) : SV_TARGET
{
	return float4(pin.color,1);
}

technique11 SphericFrameOfReference
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};