#include "Utils.h"
#include "cameras.h"

void calculateRay(HWND hwnd, LPARAM lparam, flt3& segV0, flt3& segV1)
{
	auto mousePosX = LOWORD(lparam) + 0.5f;
	auto mousePosY = HIWORD(lparam) + 0.5f;

	RECT rect;
	GetClientRect(hwnd, &rect);

	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;

	mousePosX /= width;
	mousePosY /= height;

	mousePosX = 2 * (mousePosX - 0.5);
	mousePosY = 2 * (0.5 - mousePosY);

	auto& camera = cameras()[MAIN_CAMERA];

	float nearZ = camera.getNear();
	float fovY = camera.getFov() * M_PI / 180;
	float h = 2 * nearZ * std::tan(0.5 * fovY);

	float ar = camera.getAspectRatio();
	float w = ar * h;

	mousePosX *= 0.5 * w;
	mousePosY *= 0.5 * h;

	segV0 = flt3(mousePosX, mousePosY, nearZ);

	float farZ = camera.getFar();
	segV1 = flt3(farZ * mousePosX / nearZ, farZ * mousePosY / nearZ, farZ);

	flt4x4 viewInv = camera.getView().inverse();

	flt4 segV04(segV0, 1);
	segV0 = (segV04 * viewInv).xyz();

	flt4 segV14(segV1, 1);
	segV1 = (segV14 * viewInv).xyz();
}