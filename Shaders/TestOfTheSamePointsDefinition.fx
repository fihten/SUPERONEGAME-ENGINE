cbuffer cbMapping
{
	float2 posInA;
	float2 mappedPos;
};

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

Texture2D imageA;
Texture2D imageB;

struct vs_out
{
	float4 ndc : SV_POSITION;
	float2 tex : TEX;
};

vs_out VS(float2 ndc : Position)
{
	vs_out vout;
	vout.ndc = float4(ndc, 0.1, 1);
	vout.tex = ndc;
	return vout;
}

float4 PS(vs_out pin) : SV_TARGET
{
	float2 uvA = float2(0.5f * (pin.tex.x + 1.0f), 1.0f - pin.tex.y);
	float4 colorA = imageA.Sample(samLinear, uvA);

	float2 uvB = float2(0.5f * (pin.tex.x + 1.0f), -pin.tex.y);
	float4 colorB = imageB.Sample(samLinear, uvB);

	int hereIsA = step(0, pin.tex.y);
	float4 color = lerp(colorB, colorA, hereIsA);

	float4 pointColor = float4(1, 0, 0, 1);
	float pointSize = 0.01f;

	float distanceInA = length(uvA - posInA);
	color = lerp(pointColor, color, step(pointSize, distanceInA));

	float distanceInB = length(uvB - mappedPos);
	color = lerp(pointColor, color, step(pointSize, distanceInB));

	return color;
}

technique11 TestOfTheSamePointsDefinition
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};