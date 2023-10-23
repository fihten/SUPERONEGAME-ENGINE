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

	int width = 0;
	int height = 0;
	int maxRadius = 0;
	integrals.GetDimensions(width, height, maxRadius);

	if (xy0.x > width - 1)
		return;
	if (xy0.y > height - 1)
		return;
	if (radius > maxRadius)
		return;

	float4 integral = float4(0, 0, 0, 0);
	for (int x = x_left; x <= x_right; x++)
	{
		for (int y = y_bottom; y <= y_top; y++)
		{
			int2 xy = int2(x, y);
			float2 xy_ = mul(xy, m);
			if (xy_.x < -radius)
				continue;
			if (xy_.x > radius)
				continue;
			if (xy_.y < -radius)
				continue;
			if (xy_.y > radius)
				continue;
			xy += xy0;

			xy.x -= x_left;
			xy.y -= y_bottom;

			integral += tex[xy];
		}
	}

	integrals[uint3(xy0, radius - 1)] = integral;
}

technique11 CalculationOfTextureIntegrals
{
	pass P0
	{
		SetVertexShader(NULL);
		SetPixelShader(NULL);
		SetComputeShader(CompileShader(cs_5_0, cs()));
	}
};