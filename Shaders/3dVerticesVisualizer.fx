cbuffer cbPerFrame
{
	float4x4 vp;
};

struct VertexIn
{
	float3 vertex : POSITION;
};

struct VertexOut
{
	float3 vertex : POSITION;
};

struct GeoOut
{
	float4 pos : SV_POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.vertex = vin.vertex;
	return vout;
}

[maxvertexcount(4)]
void GS(point VertexOut gin[1],
	inout LineStream<GeoOut> lineStream)
{
	float4 p = float4(gin[0].vertex, 1);
	p = mul(p, vp);
	p /= p.w;

	float4 p0 = p;
	float4 p1 = p;
	float4 p2 = p;
	float4 p3 = p;

	float sz = 0.01f;
	p0.x -= sz;
	p1.x += sz;
	p2.y -= sz;
	p3.y += sz;

	GeoOut go0;
	go0.pos = p0;
	GeoOut go1;
	go1.pos = p1;
	lineStream.Append(go0);
	lineStream.Append(go1);

	lineStream.RestartStrip();

	GeoOut go2;
	go2.pos = p2;
	GeoOut go3;
	go3.pos = p3;
	lineStream.Append(go2);
	lineStream.Append(go3);
}

float4 PS(GeoOut pin) : SV_TARGET
{
	return float4(1,0,0);
}

technique11 tech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};