cbuffer cbPerFrame
{
	float4x4 gViewProg;
};

struct VertexIn
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
	float size : SIZE;
	float3 color : COLOR;
};

struct VertexOut
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
	float size : SIZE;
	float3 color : COLOR;
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
	vout.size = vin.size;
	vout.color = vin.color;
	return vout;
}

[maxvertexcount(48)]
void GS(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
	float3 angleIndices[8] =
	{
		float3(-1, -1, -1),
		float3(-1, -1,  1),
		float3(-1,  1, -1),
		float3(-1,  1,  1),
		float3( 1, -1, -1),
		float3( 1, -1,  1),
		float3( 1,  1, -1),
		float3( 1,  1,  1)
	};
	float3 axis[3] = {
			gin[0].axis0,
			gin[0].axis1,
			gin[0].axis2
	};
	for (int i = 0; i < 8; ++i)
	{
		float3 v0 = gin[0].posW;
		v0 +=
			angleIndices[i].x * axis[0] +
			angleIndices[i].y * axis[1] +
			angleIndices[i].z * axis[2];

		GeometryOut gout0;
		gout0.posH = mul(float4(v0, 1), gViewProj);
		gout0.color = gin[0].color;

		for (int j = 0; j < 3; ++j)
		{
			float3 v1 = v0 - angleIndices[i][j] * gin[0].size * axis[j];

			GeometryOut gout1;
			gout1.posH = mul(float4(v1, 1), gViewProj);
			gout1.color = gin[0].color;

			lineStream.Append(gout0);
			lineStream.Append(gout1);

			lineStream.RestartStrip();
		}
	}
}