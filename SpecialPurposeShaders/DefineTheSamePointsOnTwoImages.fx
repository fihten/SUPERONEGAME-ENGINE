#include "constants.hlsl"

Texture2DArray<float4> integralsOfA;
Texture2DArray<float4> integralsOfB;

RWTexture2D<uint> mapAtoB;
RWTexture2D<uint> error;

[numthreads(32, 32, 1)]
void CS_initialize_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint2 ij = dispatchThreadID.xy;

	uint width = 0;
	uint height = 0;
	error.GetDimensions(width, height);

	if (ij.x > width - 1)
		return;
	if (ij.y > height - 1)
		return;
	
	error[ij].r = UINT_MAX;
}

[numthreads(32, 32, 1)]
void CS_initialize_mapping_from_A_to_B(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint2 ij = dispatchThreadID.xy;

	uint width = 0;
	uint height = 0;
	mapAtoB.GetDimensions(width, height);

	if (ij.x > width - 1)
		return;
	if (ij.y > height - 1)
		return;

	mapAtoB[ij].r = UINT_MAX;
}

[numthreads(32,32,1)]
void CS_calculate_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint mipA = 0;
	uint widthA = 0;
	uint heightA = 0;
	uint integralsNumber = 0;
	uint mipsA = 0;
	integralsOfA.GetDimensions(mipA, widthA, heightA, integralsNumber, mipsA);

	int2 posInA = int2(dispatchThreadID.x % widthA, dispatchThreadID.x / widthA);
	if (posInA.y > heightA - 1)
		return;

	uint mipB = 0;
	uint widthB = 0;
	uint heightB = 0;
	uint mipsB = 0;
	integralsOfB.GetDimensions(mipB, widthB, heightB, integralsNumber, mipsB);

	int2 posInB = int2(dispatchThreadID.y % widthB, dispatchThreadID.y / widthB);
	if (posInB.y > heightB - 1)
		return;

	int2 posInMap = { posInA.x + integralsNumber,posInA.y + integralsNumber };
	
	float l00 = 0; float l01 = 0; float r0 = 0;
	float l10 = 0; float l11 = 0; float r1 = 0;
	float yy = 0;

	float4 integralsA0 = integralsOfA[int3(posInA, integralsNumber - 1)];
	float maxIntegralA = max(integralsA0.x, max(integralsA0.y, integralsA0.z));

	float4 integralsB0 = integralsOfA[int3(posInB, integralsNumber - 1)];
	float maxIntegralB = max(integralsB0.x, max(integralsB0.y, integralsB0.z));

	for (int i = 0; i < integralsNumber; i++)
	{
		float4 integralsA = integralsOfA[int3(posInA, i)] / maxIntegralA;
		float4 integralsB = integralsOfB[int3(posInB, i)] / maxIntegralB;

		l00 += dot(integralsA.xyz, integralsA.xyz);
		l01 += dot(float4(1, 1, 1, 0), integralsA);
		l10 += dot(float4(1, 1, 1, 0), integralsA);
		l11 += 3;

		r0 += dot(integralsA.xyz, integralsB.xyz);
		r1 += dot(float4(1, 1, 1, 0), integralsB);

		yy += dot(integralsB.xyz, integralsB.xyz);
	}

	float det = l00 * l11 - l01 * l10;
	float det0 = r0 * l11 - r1 * l01;
	float det1 = l00 * r1 - l10 * r0;

	float JacobianDeterminant = det0 / det;
	float JacobianDeterminant0 = det1 / det;

	float ferr = yy - 2 * r0 * JacobianDeterminant -
		2 * r1 * JacobianDeterminant0 +
		2 * l01 * JacobianDeterminant * JacobianDeterminant0 +
		l00 * JacobianDeterminant * JacobianDeterminant +
		l11 * JacobianDeterminant0 * JacobianDeterminant0;
//	ferr /= 3 * integralsNumber;
//	ferr = sqrt(ferr);

	uint err = 100000000*ferr;
	if (ferr != 0)
		return;

	InterlockedMin(error[posInMap].r, err);
}

[numthreads(32, 32, 1)]
void CS_map_A_onto_B(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint mipA = 0;
	uint widthA = 0;
	uint heightA = 0;
	uint integralsNumber = 0;
	uint mipsA = 0;
	integralsOfA.GetDimensions(mipA, widthA, heightA, integralsNumber, mipsA);

	int2 posInA = int2(dispatchThreadID.x % widthA, dispatchThreadID.x / widthA);
	if (posInA.y > heightA - 1)
		return;

	uint mipB = 0;
	uint widthB = 0;
	uint heightB = 0;
	uint mipsB = 0;
	integralsOfB.GetDimensions(mipB, widthB, heightB, integralsNumber, mipsB);

	int2 posInB = int2(dispatchThreadID.y % widthB, dispatchThreadID.y / widthB);
	if (posInB.y > heightB - 1)
		return;

	float l00 = 0; float l01 = 0; float r0 = 0;
	float l10 = 0; float l11 = 0; float r1 = 0;
	float yy = 0;

	float4 integralsA0 = integralsOfA[int3(posInA, integralsNumber - 1)];
	float maxIntegralA = max(integralsA0.x, max(integralsA0.y, integralsA0.z));

	float4 integralsB0 = integralsOfA[int3(posInB, integralsNumber - 1)];
	float maxIntegralB = max(integralsB0.x, max(integralsB0.y, integralsB0.z));

	for (int i = 0; i < integralsNumber; i++)
	{
		float4 integralsA = integralsOfA[int3(posInA, i)] / maxIntegralA;
		float4 integralsB = integralsOfB[int3(posInB, i)] / maxIntegralB;

		l00 += dot(integralsA.xyz, integralsA.xyz);
		l01 += dot(float4(1, 1, 1, 0), integralsA);
		l10 += dot(float4(1, 1, 1, 0), integralsA);
		l11 += 3;

		r0 += dot(integralsA.xyz, integralsB.xyz);
		r1 += dot(float4(1, 1, 1, 0), integralsB);

		yy += dot(integralsB.xyz, integralsB.xyz);
	}

	float det = l00 * l11 - l01 * l10;
	float det0 = r0 * l11 - r1 * l01;
	float det1 = l00 * r1 - l10 * r0;

	float JacobianDeterminant = det0 / det;
	float JacobianDeterminant0 = det1 / det;

	float ferr = yy - 2 * r0 * JacobianDeterminant -
		2 * r1 * JacobianDeterminant0 +
		2 * l01 * JacobianDeterminant * JacobianDeterminant0 +
		l00 * JacobianDeterminant * JacobianDeterminant +
		l11 * JacobianDeterminant0 * JacobianDeterminant0;
//	ferr /= 3 * integralsNumber;
//	ferr = sqrt(ferr);


	uint err = 100000000 * ferr;
	if (ferr != 0)
		return;

	int2 posInMap = { posInA.x + integralsNumber,posInA.y + integralsNumber };
	if (err == error[posInMap].r)
	{
		uint original_value;
		uint offsetX = (widthA + 2 * integralsNumber - widthB) / 2;
		uint offsetY = (heightA + 2 * integralsNumber - heightB) / 2;
		uint x = posInB.x + offsetX;
		uint y = posInB.y + offsetY;
		uint width = widthA + 2 * integralsNumber;
		uint new_value = y * width + x;
		InterlockedExchange(mapAtoB[posInMap].r, new_value, original_value);
	}
}

technique11 DefineTheSamePointsOnTwoImages
{
	pass InitializeError
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_initialize_error()));
	}
	pass InitializeMapping
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_initialize_mapping_from_A_to_B()));
	}
	pass CalculateError
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_calculate_error()));
	}
	pass MapAontoB
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_map_A_onto_B()));
	}
};