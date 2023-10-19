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

[numthreads(32,32,1)]
void CS_calculate_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint integralsNumber = 0;
	uint mips = 0;
	integralsOfA.GetDimensions(mip, width, height, integralsNumber, mips);

	int2 posInA = int2(dispatchThreadID.x % width, dispatchThreadID.x / width);
	if (posInA.y > height - 1)
		return;
	int2 posInB = int2(dispatchThreadID.y % width, dispatchThreadID.y / width);
	if (posInB.y > height - 1)
		return;

	uint original_value;
	InterlockedExchange(mapAtoB[posInA].r, UINT_MAX, original_value);
	
	float a = 0;
	float b = 0;
	float c = 0;
	for (int i = 0; i < integralsNumber; i++)
	{
		float4 integralsA = integralsOfA[int3(posInA, i)];
		float4 integralsB = integralsOfB[int3(posInB, i)];

		a += dot(integralsA.xyz, integralsA.xyz);
		b += dot(integralsA.xyz, integralsB.xyz);
		c += dot(integralsB.xyz, integralsB.xyz);
	}
	float JacobianDeterminant = b / a;
	float ferr = (a * c - b * b) / a;
	ferr /= 3 * integralsNumber;
	ferr = sqrt(ferr);

	uint err = 1000000 * ferr;

	InterlockedMin(error[posInA].r, err);
}

[numthreads(32, 32, 1)]
void CS_map_A_onto_B(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint mip = 0;
	uint width = 0;
	uint height = 0;
	uint integralsNumber = 0;
	uint mips = 0;
	integralsOfA.GetDimensions(mip, width, height, integralsNumber, mips);

	int2 posInA = int2(dispatchThreadID.x % width, dispatchThreadID.x / width);
	if (posInA.y > height - 1)
		return;
	int2 posInB = int2(dispatchThreadID.y % width, dispatchThreadID.y / width);
	if (posInB.y > height - 1)
		return;

	float a = 0;
	float b = 0;
	float c = 0;
	for (int i = 0; i < integralsNumber; i++)
	{
		float4 integralsA = integralsOfA[int3(posInA, i)];
		float4 integralsB = integralsOfB[int3(posInB, i)];

		a += dot(integralsA.xyz, integralsA.xyz);
		b += dot(integralsA.xyz, integralsB.xyz);
		c += dot(integralsB.xyz, integralsB.xyz);
	}
	float JacobianDeterminant = b / a;
	float ferr = (a * c - b * b) / a;
	ferr /= 3 * integralsNumber;
	ferr = sqrt(ferr);

	uint err = 1000000 * ferr;
	if (err == error[posInA].r)
	{
		uint original_value;
		uint new_value = posInB.y * width + posInB.x;
		InterlockedExchange(mapAtoB[posInA].r, new_value, original_value);
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