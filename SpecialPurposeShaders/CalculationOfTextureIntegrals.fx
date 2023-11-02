Texture2D<float4> tex;
RWTexture2DArray<float4> integrals;

int radius0;
int radius1;

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
	float m00 = scale0 * cos(angle0); float m01 = scale0 * sin(angle0);
	float m10 = -scale1 * sin(angle1); float m11 = scale1 * cos(angle1);

	float det = m00 * m11 - m01 * m10;
	float2x2 mInv = float2x2(
		m11, -m01,
		-m10, m00
		);
	mInv /= det;

	int2 xy0 = dispatchThreadID.xy;
	int radius = radius0 + dispatchThreadID.z;
	if (radius > radius1)
		return;

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
			float2 xy_ = mul(xy, mInv);
			if (length(xy_) > radius)
				continue;

			xy += xy0;

			if (xy.x < 0)
				continue;
			if (xy.x > width - 1)
				continue;
			if (xy.y < 0)
				continue;
			if (xy.y > height - 1)
				continue;

			integral += tex[xy];
		}
	}

	integrals[uint3(xy0, radius - radius0)] = integral;
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