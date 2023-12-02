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

float3 srgbToRaw(float3 srgb)
{
	float3 xyz;
	xyz.r = srgb.r <= 0.04045f ? srgb.r / 12.92f : pow((srgb.r + 0.055f) / 1.055f, 2.4f);
	xyz.g = srgb.g <= 0.04045f ? srgb.g / 12.92f : pow((srgb.g + 0.055f) / 1.055f, 2.4f);
	xyz.b = srgb.b <= 0.04045f ? srgb.b / 12.92f : pow((srgb.b + 0.055f) / 1.055f, 2.4f);

	float3x3 m = {
		0.4124f, 0.3576f, 0.1805f,
		0.2126f, 0.7152f, 0.0722f,
		0.0193f, 0.1192f, 0.9505f
	};
	return mul(m, xyz);
}

#endif