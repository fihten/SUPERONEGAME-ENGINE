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
	float3 color : COLOR;
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

void addBox(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
	GeometryOut gout;
	gout.color = float3(0, 1, 0);

	// top edges

	// first edge
	gout.posH = mul(float4(gin.posW + gin.axis2 - gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW + gin.axis2 - gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// second edge
	gout.posH = mul(float4(gin.posW + gin.axis2 + gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW + gin.axis2 + gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// third edge
	gout.posH = mul(float4(gin.posW + gin.axis2 - gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW + gin.axis2 + gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// fourth edge
	gout.posH = mul(float4(gin.posW + gin.axis2 - gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW + gin.axis2 + gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// bottom edges

	// first edge
	gout.posH = mul(float4(gin.posW - gin.axis2 - gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW - gin.axis2 - gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// second edge
	gout.posH = mul(float4(gin.posW - gin.axis2 + gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW - gin.axis2 + gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// third edge
	gout.posH = mul(float4(gin.posW - gin.axis2 - gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW - gin.axis2 + gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// fourth edge
	gout.posH = mul(float4(gin.posW - gin.axis2 - gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW - gin.axis2 + gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// side edges

	// first 
	gout.posH = mul(float4(gin.posW - gin.axis2 - gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW + gin.axis2 - gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// second 
	gout.posH = mul(float4(gin.posW - gin.axis2 - gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW + gin.axis2 - gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// third 
	gout.posH = mul(float4(gin.posW - gin.axis2 + gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW + gin.axis2 + gin.axis0 - gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();

	// fourth 
	gout.posH = mul(float4(gin.posW - gin.axis2 + gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	gout.posH = mul(float4(gin.posW + gin.axis2 + gin.axis0 + gin.axis1), gViewProj);
	lineStream.Append(gout);

	lineStream.RestartStrip();
}

void addSphere(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{

}

[maxvertexcount(99)]
void GS(point VertexOut gin[1], inout LineStream<GeometryOut> lineStream)
{
	
}