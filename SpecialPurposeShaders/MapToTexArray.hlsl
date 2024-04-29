uint3 mapToTexArray(
	uint3 uvw,
	uint2 dims0,
	uint2 dims1
)
{
	uint pixelIndex = uvw.z * dims0.x * dims0.y;
	pixelIndex += uvw.y * dims0.x;
	pixelIndex += uvw.x;

	uint3 res;
	uint pixels = dims1.x * dims1.y;
	res.z = pixelIndex / pixels;
	pixelIndex -= res.z * pixels;
	res.y = pixelIndex / dims1.x;
	res.x = pixelIndex - res.y * dims1.x;

	return res;
}