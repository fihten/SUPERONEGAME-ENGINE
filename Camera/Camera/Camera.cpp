#include "pch.h"
#include "Camera.h"

const flt4x4& Camera::getView()
{
	if (changed)
		updateView();

	return view;
}

void Camera::updateView()
{
	flt3 up(0, 1, 0);
	flt3 right = cross(fwd, up);
	up = cross(right, fwd);

	fwd.normalize();
	right.normalize();
	up.normalize();

	view.m00() = right.x(); view.m01() = up.x(); view.m02() = fwd.x(); view.m03() = 0.0f;
	view.m10() = right.y(); view.m11() = up.y(); view.m12() = fwd.y(); view.m13() = 0.0f;
	view.m20() = right.z(); view.m21() = up.z(); view.m22() = fwd.z(); view.m23() = 0.0f;
	view.m30() = -dot(position, right); view.m31() = -dot(position, up); view.m32() = -dot(position, fwd); view.m33() = 1.0f;
}