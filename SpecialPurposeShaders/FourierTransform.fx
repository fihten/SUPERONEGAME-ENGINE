Texture2D tex;

RWStructuredBuffer<int> fourierCoefficientsAtCos_R;
RWStructuredBuffer<int> fourierCoefficientsAtCos_G;
RWStructuredBuffer<int> fourierCoefficientsAtCos_B;

RWStructuredBuffer<int> fourierCoefficientsAtSin_R;
RWStructuredBuffer<int> fourierCoefficientsAtSin_G;
RWStructuredBuffer<int> fourierCoefficientsAtSin_B;

int radius;
int thickness;

int width;
int height;

int numberOfElementsInFourierRow;

[numthreads(16,16,4)]
void cs_cos_r(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int r = dispatchThreadID.xy;
	r.x -= radius;
	r.y -= radius;

	if (r.x < -radius)
		return;
	if (r.y < -radius)
		return;

	if (r.x > radius)
		return;
	if (r.y > radius)
		return;

	int index = dispatchThreadID.z;
	if (index > numberOfElementsInFourierRow - 1)
		return;
}

