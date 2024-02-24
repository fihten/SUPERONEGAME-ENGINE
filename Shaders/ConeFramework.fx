#include "FrameworkUtils.fx"

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
	float scaleFromBottomToTop : SCALE_FROM_BOTTOM_TO_TOP;
};

struct VertexOut
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
	float scaleFromBottomToTop : SCALE_FROM_BOTTOM_TO_TOP;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.posW = vin.posW;
	vout.axis0 = vin.axis0;
	vout.axis1 = vin.axis1;
	vout.axis2 = vin.axis2;
	vout.scaleFromBottomToTop = vin.scaleFromBottomToTop;
	return vout;
}

[maxvertexcount(148)]
void GS(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
	float k = max(1, gin[0].scaleFromBottomToTop);

	SeedOfFramework seed;
	seed.posW = gin[0].posW;
	seed.axis0 = k * gin[0].axis0;
	seed.axis1 = k * gin[0].axis1;
	seed.axis2 = gin[0].axis2;
	seed.scaleFromBottomToTop = gin[0].scaleFromBottomToTop;

	addBox(seed, lineStream);

	seed.axis0 = gin[0].axis0;
	seed.axis1 = gin[0].axis1;
	addCone(seed, lineStream);
}

technique11 ConeFramework
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};