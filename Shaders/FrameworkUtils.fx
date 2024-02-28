#ifndef FRAMEWORK_UTILS_FX
#define FRAMEWORK_UTILS_FX

cbuffer cbPerFrame
{
	float4x4 gViewProj;
};

struct SeedOfFramework
{
	float3 posW;
	float3 axis0;
	float3 axis1;
	float3 axis2;
	float scaleFromBottomToTop;
};

struct GeometryOut
{
	float4 posH : SV_POSITION;
	float3 color : COLOR;
};

void addBox(SeedOfFramework seed, inout LineStream<GeometryOut> lineStream)
{
	GeometryOut gout;
	gout.color = float3(0, 1, 0);

	float3 axis0 = 0.5f * seed.axis0;
	float3 axis1 = 0.5f * seed.axis1;
	float3 axis2 = 0.5f * seed.axis2;

	float3 center = seed.posW + axis0 + axis1 + axis2;

	// top edges
	gout.posH = mul(float4(center + axis2 - axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center + axis2 - axis0 + axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center + axis2 + axis0 + axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center + axis2 + axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center + axis2 - axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// bottom edges
	gout.posH = mul(float4(center - axis2 - axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center - axis2 - axis0 + axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center - axis2 + axis0 + axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center - axis2 + axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center - axis2 - axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// side edges

	// first 
	gout.posH = mul(float4(center - axis2 - axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center + axis2 - axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// second 
	gout.posH = mul(float4(center - axis2 - axis0 + axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center + axis2 - axis0 + axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// third 
	gout.posH = mul(float4(center - axis2 + axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center + axis2 + axis0 - axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// fourth 
	gout.posH = mul(float4(center - axis2 + axis0 + axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(center + axis2 + axis0 + axis1, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();
}

void addSphere(SeedOfFramework seed, inout LineStream<GeometryOut> lineStream)
{
	int segments = 16;
	float phi = 6.28 / segments;

	GeometryOut gout;
	gout.color = float4(0, 1, 0, 0);

	float3 axis0 = 0.5f * seed.axis0;
	float3 axis1 = 0.5f * seed.axis1;
	float3 axis2 = 0.5f * seed.axis2;

	float3 center = seed.posW + axis0 + axis1 + axis2;

	// plane Oxy
	for (int i = 0; i < segments + 1; ++i)
	{
		gout.posH.xyz = center;
		gout.posH.xyz += axis0 * cos(i * phi);
		gout.posH.xyz += axis1 * sin(i * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);
	}
	lineStream.RestartStrip();

	// plane Oxz
	for (int j = 0; j < segments + 1; ++j)
	{
		gout.posH.xyz = center;
		gout.posH.xyz += axis0 * cos(j * phi);
		gout.posH.xyz += axis2 * sin(j * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);
	}
	lineStream.RestartStrip();

	// plane Oyz
	for (int k = 0; k < segments + 1; ++k)
	{
		gout.posH.xyz = center;
		gout.posH.xyz += axis1 * cos(k * phi);
		gout.posH.xyz += axis2 * sin(k * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);
	}
	lineStream.RestartStrip();
}

void addCone(SeedOfFramework seed, inout LineStream<GeometryOut> lineStream)
{
	int segments = 32;
	float phi = 6.28 / segments;

	GeometryOut gout;
	gout.color = float4(0, 1, 0, 0);

	float3 axis0 = 0.5f * seed.axis0;
	float3 axis1 = 0.5f * seed.axis1;
	float3 axis2 = seed.axis2;

	float3 center = seed.posW + axis0 + axis1;

	// bottom circle
	for (int i = 0; i < segments + 1; ++i)
	{
		gout.posH.xyz = center;
		gout.posH.xyz += axis0 * cos(i * phi);
		gout.posH.xyz += axis1 * sin(i * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);
	}
	lineStream.RestartStrip();

	// top circle
	for (int i = 0; i < segments + 1; ++i)
	{
		gout.posH.xyz = center + axis2;
		gout.posH.xyz += seed.scaleFromBottomToTop * axis0 * cos(i * phi);
		gout.posH.xyz += seed.scaleFromBottomToTop * axis1 * sin(i * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);
	}
	lineStream.RestartStrip();
	
	// side
	for (int i = 0; i < segments; ++i)
	{
		gout.posH.xyz = center;
		gout.posH.xyz += axis0 * cos(i * phi);
		gout.posH.xyz += axis1 * sin(i * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);

		gout.posH.xyz = center + axis2;
		gout.posH.xyz += seed.scaleFromBottomToTop * axis0 * cos(i * phi);
		gout.posH.xyz += seed.scaleFromBottomToTop * axis1 * sin(i * phi);
		gout.posH.w = 1;
		gout.posH = mul(gout.posH, gViewProj);
		lineStream.Append(gout);

		lineStream.RestartStrip();
	}
}

#endif