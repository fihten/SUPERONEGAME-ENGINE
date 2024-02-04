cbuffer cbPerFrame
{
	float4x4 gViewProj;
	float3 eyePosW;
};

void buildFrameOfReferences(
	float3 pt0,
	float3 pt1,
	out float3 axis0,
	out float3 axis1,
	out float3 axis2
)
{
	axis2 = pt1 - pt0;
	normalize(axis2);

	axis0 = 0;
	axis0.x = 1;

	float m = abs(axis2.x);
	
	float m0 = abs(axis2.y);
	if (m0 < m)
	{
		axis0 = 0;
		axis0.y = 1;
		m = m0;
	}

	m0 = abs(axis2.z);
	if (m0 < m)
	{
		axis0 = 0;
		axis0.z = 1;
		m = m0;
	}

	axis1 = cross(axis2, axis0);
	axis1 = normalize(axis1);

	axis0 = cross(axis1, axis2);
	axis0 = normalize(axis0);
}

struct VertexIn
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
	float handleRadius : HANDLE_RADIUS;
};

struct VertexOut
{
	float3 posW : POSITION;
	float3 axis0 : AXIS0;
	float3 axis1 : AXIS1;
	float3 axis2 : AXIS2;
	float handleRadius : HANDLE_RADIUS;
};

struct GeometryOut
{
	float4 posH : SV_POSITION;
	float3 color : COLOR;
};

void buildSphere(
	float3 eyePosW,
	float3 centerOfSphere,
	float radius,
	int segments,
	float3 color,
	inout LineStream<GeometryOut> lineStream
)
{
	float3 axis0;
	float3 axis1;
	float3 axis2;

	buildFrameOfReferences(
		eyePosW, centerOfSphere,
		axis0, axis1, axis2
	);

	float dPhi = 6.28 / segments;
	for (int vi = 0; vi < segments + 1; ++vi)
	{
		float3 e0 = axis2;
		
		float phi = vi * dPhi;
		float3 e1 = cos(phi) * axis0 + sin(phi) * axis1;

		float len = length(eyePosW - centerOfSphere);
		float sinus = radius / len;

		float distToPointSq = len * len - radius * radius;
		
		float y = distToPointSq * sinus * sinus;
		float x = distToPointSq - y;

		x = sqrt(x);
		y = sqrt(y);

		float3 pt = eyePosW + x * e0 + y * e1;

		GeometryOut gout;
		gout.posH = mul(float4(pt, 1), gViewProj);
		gout.color = color;

		lineStream.Append(gout);
	}
	lineStream.RestartStrip();
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.posW = vin.posW;
	vout.axis0 = vin.axis0;
	vout.axis1 = vin.axis1;
	vout.axis2 = vin.axis2;
	vout.handleRadius = vin.handleRadius;
	return vout;
}

[maxvertexcount(84)]
void GS(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
	GeometryOut gout;
	int segments = 12;

	float3 pt = gin[0].posW + gin[0].axis0;

	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	gout.color = float4(1, 0, 0, 1);
	lineStream.Append(gout);

	pt = gin[0].posW - gin[0].axis0;
	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	buildSphere(
		eyePosW,
		gin[0].posW - gin[0].axis0,
		gin[0].handleRadius,
		segments,
		gout.color,
		lineStream
	);
	buildSphere(
		eyePosW,
		gin[0].posW + gin[0].axis0,
		gin[0].handleRadius,
		segments,
		gout.color,
		lineStream
	);

	////////////////////////////////////////////////

	pt = gin[0].posW + gin[0].axis1;

	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	gout.color = float4(0, 1, 0, 1);
	lineStream.Append(gout);

	pt = gin[0].posW - gin[0].axis1;
	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	buildSphere(
		eyePosW,
		gin[0].posW - gin[0].axis1,
		gin[0].handleRadius,
		segments,
		gout.color,
		lineStream
	);
	buildSphere(
		eyePosW,
		gin[0].posW + gin[0].axis1,
		gin[0].handleRadius,
		segments,
		gout.color,
		lineStream
	);

	////////////////////////////////////////////////

	pt = gin[0].posW + gin[0].axis2;

	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	gout.color = float4(0, 0, 1, 1);
	lineStream.Append(gout);

	pt = gin[0].posW - gin[0].axis2;
	gout.posH = mul(float4(pt, 1.0f), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	buildSphere(
		eyePosW,
		gin[0].posW - gin[0].axis2,
		gin[0].handleRadius,
		segments,
		gout.color,
		lineStream
	);
	buildSphere(
		eyePosW,
		gin[0].posW + gin[0].axis2,
		gin[0].handleRadius,
		segments,
		gout.color,
		lineStream
	);
}

float4 PS(GeometryOut pin) : SV_TARGET
{
	return float4(pin.color,1);
}

technique11 ScaleFrameOfReference
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(CompileShader(gs_5_0, GS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};