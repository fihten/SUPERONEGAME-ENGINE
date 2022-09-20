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

	
}