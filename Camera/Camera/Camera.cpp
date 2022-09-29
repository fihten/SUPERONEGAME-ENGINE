#include "pch.h"
#include "Camera.h"

const flt4x4& Camera::getView()
{
	if (changed)
	{
		updateView();
		updateProj();
	}

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

	changed = false;
}

const flt4x4& Camera::getProj()
{
	if (changed)
	{
		updateView();
		updateProj();
	}

	return proj;
}

void Camera::updateProj()
{
	proj = makePerspectiveProjection<float>(aspectRatio, fovY, nearZ, farZ);
	changed = false;
}

UINT Camera::processKey(UINT msg, WPARAM wparam, LPARAM lparam)
{
	return 0;
}

void Camera::walk(float dt)
{
	float l = walkVelocity * dt;
	position = position + fwd * l;
	changed = true;
}

void Camera::run(float dt)
{
	float l = runVelocity * dt;
	position = position + fwd * l;
	changed = true;
}

void Camera::lookUp(float d)
{
	float angle = abs(rotateCoeff * d);
	flt3 right = cross(fwd, flt3(0, 1, 0));
	Matrix4x4<float> rot = makeRotate<float>(right, angle);
	fwd = fwd * rot;
}