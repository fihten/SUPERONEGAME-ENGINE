#pragma once

#include "Vec3d.h"
#include "Matrix4x4.h"
#include <Windows.h>

class Camera
{
	flt3 position = flt3(0, 0, 0);
	flt3 fwd = flt3(0, 0, 1);

	float nearZ = 0.1;
	float farZ = 100;

	float aspectRatio = 4.0f / 3.0f;
	float fovY = 90;

	flt4x4 view;
	flt4x4 proj;

	bool changed = true;

	float walkVelocity = 1;
	float runVelocity = 5;

public:
	const flt4x4& getView();
	const flt4x4& getProj();

	UINT processKey(UINT msg, WPARAM wparam, LPARAM lparam);

private:
	void updateView();
	void updateProj();

	void walk(float dt);
	void run(float dt);
};
