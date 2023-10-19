#define CHANEL_INTENSITY 0

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
	uint radiusOfArea = 0;
	horisontalIntegrals.GetDimensions(width, height, radiusOfArea);

	if (ij.x > width - 1)
		return;
	if (ij.y > height - 1)
		return;

	ij.x += radiusOfArea;
	float4 integral = float4(0, 0, 0, 0);
	for (int i = -radiusOfArea; i <= radiusOfArea; i++)
	{
		int2 ij_ = ij;
		ij_.x += i;
		float4 color = tex[ij_];
		if (integralType == CHANEL_INTENSITY)
			integral += exp(-i * i) * color;
	}

	ij.x -= radiusOfArea;
	horisontalIntegrals[int3(ij, integralType)] = integral;
}

[numthreads(16, 16, 4)]
void cs_integrate_along_v_axis(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int2 ij = dispatchThreadID.xy;
	int integralType = dispatchThreadID.z;

	uint width = 0;
	uint height = 0;
	uint radiusOfArea = 0;
	verticalIntegrals.GetDimensions(width, height, radiusOfArea);

	if (ij.x > width - 1)
		return;
	if (ij.y > height - 1)
		return;

	ij.y += radiusOfArea;
	float4 integral = float4(0, 0, 0, 0);
	for (int i = -radiusOfArea; i <= radiusOfArea; i++)
	{
		int2 ij_ = ij;
		ij_.y += i;
		float4 color;
		color = horisontalIntegralsInput[int3(ij_, integralType)];
		if (integralType == CHANEL_INTENSITY)
			integral += exp(-i * i) * color;
	}

	ij.y -= radiusOfArea;
	verticalIntegrals[int3(ij, integralType)] = integral;
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