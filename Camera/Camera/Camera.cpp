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

}