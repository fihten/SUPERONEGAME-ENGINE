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

int r0;

[numthreads(16,16,4)]
void cs_cos_r(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int index = dispatchThreadID.z;
	if (index > numberOfElementsInFourierRow - 1)
		return;

	int2 r = dispatchThreadID.xy;
	r.x -= radius;
	r.y -= radius;

	float l = length(r);
	if (l < (float)(radius)-0.5f * (float)(thickness))
		return;
	if (l > (float)(radius)+0.5f * (float)(thickness))
		return;

	float t = atan2(r.y, r.x);

	r += r0;
	if (r.x < 0)
		return;
	if (r.x > width - 1)
		return;
	if (r.y < 0)
		return;
	if (r.y > height - 1)
		return;

	float r_chanel = tex[r].r;
	float f_el = r_chanel * cos(index * t);
	int i_el = round(128 * f_el);

	InterlockedAdd(fourierCoefficientsAtCos_R[index], i_el);
}

