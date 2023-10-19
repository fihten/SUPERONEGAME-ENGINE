#define CHANEL_INTENSITY 0
#define RADIUS_OF_AREA_IN_TEXELS 10

Texture2D<float4> tex;

RWTexture2DArray<float4> horisontalIntegrals;
Texture2DArray<float4> horisontalIntegralsInput;
RWTexture2DArray<float4> verticalIntegrals;

[numthreads(16, 16, 4)]
void cs_integrate_along_u_axis(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int2 ij = dispatchThreadID.xy;
	int integralType = dispatchThreadID.z;

	uint width = 0;
	uint height = 0;
	uint elements = 0;
	horisontalIntegrals.GetDimensions(width, height, elements);

	if (ij.x > width - 1)
		return;
	if (ij.y > height - 1)
		return;

	ij.x += RADIUS_OF_AREA_IN_TEXELS;
	float4 integral = float4(0, 0, 0, 0);
	for (int i = -RADIUS_OF_AREA_IN_TEXELS; i <= RADIUS_OF_AREA_IN_TEXELS; i++)
	{
		int2 ij_ = ij;
		ij_.x += i;
		float4 color = tex[ij_].xyzw;
		integral += color;
	}

	ij.x -= RADIUS_OF_AREA_IN_TEXELS;
	horisontalIntegrals[int3(ij, integralType)].xyzw = integral;
}

[numthreads(16, 16, 4)]
void cs_integrate_along_v_axis(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int2 ij = dispatchThreadID.xy;
	int integralType = dispatchThreadID.z;

	uint width = 0;
	uint height = 0;
	uint elements = 0;
	verticalIntegrals.GetDimensions(width, height, elements);

	if (ij.x > width - 1)
		return;
	if (ij.y > height - 1)
		return;

	ij.y += RADIUS_OF_AREA_IN_TEXELS;
	float4 integral = float4(0, 0, 0, 0);
	for (int i = -RADIUS_OF_AREA_IN_TEXELS; i <= RADIUS_OF_AREA_IN_TEXELS; i++)
	{
		int2 ij_ = ij;
		ij_.y += i;
		float4 color;
		color = horisontalIntegralsInput[int3(ij_, integralType)].xyzw;
		integral += color;
	}

	ij.y -= RADIUS_OF_AREA_IN_TEXELS;
	verticalIntegrals[int3(ij, integralType)].xyzw = integral;
}

technique11 CalculationOfTextureIntegrals
{
	pass AlongUaxis
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_integrate_along_u_axis()));
	}
	pass AlongVaxis
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_integrate_along_v_axis()));
	}
};