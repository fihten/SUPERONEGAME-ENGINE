Texture2D arrow;

SamplerState ssLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float2 pos : Position;
};

int2 centerOfArrow;
int radiusOfArrow;

float angle0;
float scale0;

float angle1;
float scale1;

int width;
int height;
float ndcOffsetY;

struct VertexOut
{
	float4 ndc : SV_Position;
	float2 uv : TexCoord;
};

VertexOut vs(VertexIn vin)
{
	VertexOut vout;

	float3x3 m;
	m[0][0] = radiusOfArrow * scale0 * cos(angle0);
	m[0][1] = radiusOfArrow * scale0 * sin(angle0);
	m[0][2] = 0;
	m[1][0] = -radiusOfArrow * scale1 * sin(angle1);
	m[1][1] = radiusOfArrow * scale1 * cos(angle1);
	m[1][2] = 0;
	m[2][0] = centerOfArrow.x; m[2][1] = centerOfArrow.y; m[2][2] = 1;

	float3 posInPixelSpace = mul(float3(vin.pos, 1.0f), m);
	
	float3x3 toUV = {
		1.0f / width,0.0f,0.0f,
		0.0f,1.0f / height,0.0f,
		0.5f / width,0.5f / height,1.0f
	};
	float3 uv = mul(posInPixelSpace, toUV);

	float3x4 toNDC = {
		2.0f, 0.0f,0.0f,0.0f,
		0.0f, -1.0f,0.0f,0.0f,
		-1.0f,1.0f + ndcOffsetY,0.0f,1.0f
	};
	vout.ndc = mul(uv, toNDC);

	float3x3 toTex = {
		0.5f,0.0f,0.0f,
		0.0f,0.5f,0.0f,
		0.5f,0.5f,1.0f
	};
	vout.uv = mul(float3(vin.pos, 1.0f), toTex).xy;

	return vout;
}

float4 ps(VertexOut pin) : SV_Target
{
	float4 color = arrow.Sample(ssLinear, pin.uv);
	color.a = 0.5f;

	return color;
}

technique11 VisualizeAreaOfIntegration
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, vs()));
		SetPixelShader(CompileShader(ps_5_0, ps()));
	}
};