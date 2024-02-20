#include "ObjectInfo.hlsl"

RWStructuredBuffer<float4> boundingSpheres;
RWStructuredBuffer<uint> radiuses;
uint spheresCount;
float convertToUINT;

StructuredBuffer<float3> vertices;
StructuredBuffer<ObjectInfo> objectsInfo;
uint objectsCount;

StructuredBuffer<uint> roughlySelectedObjects;

[numthreads(256, 1, 1)]
void CS_clearRadius(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint objectIndex = dispatchThreadID.x;
	if (objectIndex >= objectsCount)
		return;
	radiuses[objectIndex] = 0;
}

[numthreads(1, 256, 1)]
void CS_calculateRadius(
	uint3 dispatchThreadID : SV_DispatchThreadID,
	uniform bool bCheckSelectionStatus
)
{
	uint objectIndex = dispatchThreadID.x;
	if (objectIndex >= objectsCount)
		return;

	if (bCheckSelectionStatus)
	{
		if (roughlySelectedObjects[objectIndex] == 0)
			return;
	}

	uint vertexIndex = dispatchThreadID.y;
	if (vertexIndex >= objectsInfo[objectIndex].verticesCount)
		return;
	vertexIndex += objectsInfo[objectIndex].verticesOffset;

	float3 vertex = vertices[vertexIndex];
	float3 vertexW = mul(float4(vertex, 1.0f), objectsInfo[objectIndex].world).xyz;

	float3 c = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), objectsInfo[objectIndex].world).xyz;
	float r = length(vertexW - c);

	uint uiR = ceil(r * convertToUINT);
	uint originalValue = 0;
	InterlockedMax(radiuses[objectIndex], uiR, originalValue);
}

[numthreads(256, 1, 1)]
void CS_calculateSpheres(
	uint3 dispatchThreadID : SV_DispatchThreadID,
	uniform bool bCheckSelectionStatus
	)
{
	uint objectIndex = dispatchThreadID.x;
	if (objectIndex >= objectsCount)
		return;

	if (bCheckSelectionStatus)
	{
		if (roughlySelectedObjects[objectIndex] == 0)
			return;
	}

	boundingSpheres[objectIndex] = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), objectsInfo[objectIndex].world);
	boundingSpheres[objectIndex].w = (float)radiuses[objectIndex] / convertToUINT;
}

technique11 ClearRadius
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_clearRadius()));
	}
};

technique11 CalculateRadius
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_calculateRadius(false)));
	}
};

technique11 CalculateRadius_checkSelectionStatus
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_calculateRadius(true)));
	}
};

technique11 CalculateSpheres
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_calculateSpheres(false)));
	}
};

technique11 CalculateSpheres_checkSelectionStatus
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, CS_calculateSpheres(true)));
	}
};