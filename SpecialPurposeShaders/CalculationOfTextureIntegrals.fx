Texture2D<float4> tex;
RWTexture2DArray<float4> integrals;

int x_left;
int x_right;
int y_bottom;
int y_top;

float angle0;
float scale0;

float angle1;
float scale1;

[numthreads(16, 16, 4)]
void cs(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	float2x2 m = float2x2(
		scale0 * cos(angle0), scale0 * sin(angle0),
		-scale1 * sin(angle1), scale1 * cos(angle1)
		);

	int2 xy0 = dispatchThreadID.xy;
	int radius = dispatchThreadID.z + 1;
}

technique11 CalculationOfTextureIntegrals
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs_integrate_along_u_axis()));
	}
};