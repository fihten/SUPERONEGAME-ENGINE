Texture2DArray<uint> error;
Texture2DArray<uint> AtoBx;
Texture2DArray<uint> AtoBy;
Texture2DArray<uint> AtoBz;
Texture2DArray<uint> AtoBw;
RWStructuredBuffer<uint> minDistanceOut;
StructuredBuffer<uint> minDistanceIn;
RWStructuredBuffer<uint> mappingOfPoint;

int width;
int height;
int index;

int2 pt;
uint threshold;

[numthreads(32, 32, 1)]
void cs_minDistance(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= width)
		return;

	int y = dispatchThreadID.y;
	if (y >= height)
		return;

	uint3 pos = uint3(x, y, index);
	uint err = error[pos].r;
	//if (err > threshold)
	//	return;

	uint2 mapping = uint2(AtoBx[pos].r, AtoBy[pos].r);

	uint3 posInA;
	posInA.x = mapping.x >> 16;
	posInA.y = mapping.x & 0xffff;
	posInA.z = mapping.y;

	uint distance2 = dot(pt - posInA.xy, pt - posInA.xy);
	uint originalValue = 0;
	InterlockedMin(minDistanceOut[0].r, distance2, originalValue);
}

[numthreads(32, 32, 1)]
void cs_mapping(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= width)
		return;

	int y = dispatchThreadID.y;
	if (y >= height)
		return;

	uint3 pos = uint3(x, y, index);
	uint err = error[pos].r;
	//if (err > threshold)
	//	return;

	uint4 mapping = uint4(AtoBx[pos].r, AtoBy[pos].r, AtoBz[pos].r, AtoBw[pos].r);

	uint3 posInA;
	posInA.x = mapping.x >> 16;
	posInA.y = mapping.x & 0xffff;
	posInA.z = mapping.y;

	uint distance2 = dot(pt - posInA.xy, pt - posInA.xy);
	if (distance2 != minDistanceIn[0].r)
		return;

	uint originalValue = 0;
	InterlockedExchange(mappingOfPoint[0].r, mapping.x, originalValue);
	InterlockedExchange(mappingOfPoint[1].r, mapping.y, originalValue);
	InterlockedExchange(mappingOfPoint[2].r, mapping.z, originalValue);
	InterlockedExchange(mappingOfPoint[3].r, mapping.w, originalValue);
}

technique11 findNearestDefinedPoint
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_minDistance()));
	}
	pass P1
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_mapping()));
	}
};