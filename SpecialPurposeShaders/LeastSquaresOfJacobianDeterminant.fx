Texture2DArray<uint> AA;
Texture2DArray<uint> AB;
Texture2DArray<uint> BB;

Texture2DArray<uint> AAfraction;
Texture2DArray<uint> ABfraction;
Texture2DArray<uint> BBfraction;

Texture2DArray<uint> maxA;
Texture2DArray<uint> maxB;

int width;
int height;
int texturesCount;

[numthreads(16, 16, 4)]
void cs_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int x = dispatchThreadID.x;
	if (x >= width)
		return;

	int y = dispatchThreadID.y;
	if (y >= height)
		return;

	int indexOfPhoto = dispatchThreadID.z;
	if (indexOfPhoto >= texturesCount)
		return;

	uint3 locationIn = uint3(x, y, indexOfPhoto);
	
	float AA_ = AA[locationIn].r;
	float AAfraction_ = AAfraction[locationIn].r;
	AA_ = AA_ + AAfraction_ / 1000000.0f;

	float BB_ = BB[locationIn].r;
	float BBfraction_ = BBfraction[locationIn].r;
	BB_ = BB_ + BBfraction_ / 1000000.0f;

	float AB_ = AB[locationIn].r;
	float ABfraction_ = ABfraction[locationIn].r;
	AB_ = AB_ + ABfraction_ / 1000000.0f;

	float maxA_ = maxA[locationIn].r;
	maxA_ /= 255.0f;

	float maxB_ = maxB[locationIn].r;
	maxB_ /= 255.0f;

	AA_ /= maxA_ * maxA_;
	BB_ /= maxB_ * maxB_;
	AB_ /= maxA_ * maxB_;

	float J = AB_ / AA_;
	float err = J * J * AA_ + BB_ - 2 * J * AB_;
}