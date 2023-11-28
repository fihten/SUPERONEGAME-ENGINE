#ifndef COLOR_SPACE_HLSL
#define COLOR_SPACE_HLSL

float3 adobeToRaw(float3 rgb)
{
	float3x3 m = {
		0.60974f, 0.20528f, 0.14919f,
		0.31111f, 0.62567f, 0.06322f,
		0.01947f, 0.06087f, 0.74457f
	};
	return mul(m, pow(rgb, 563.0f / 256.0f));
}

#endif