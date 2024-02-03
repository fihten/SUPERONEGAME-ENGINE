#pragma once

#include "Modifier.h"
#include "FrameOfReferenceState.h"

class Rotation :public Modifier
{
	IntersectedCircleAxis grabbedAxis = IntersectedCircleAxis::NONE;
	float paramOnAxis;

	float mousePosX = 0;
	float mousePosY = 0;
public:
	Behaviour processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};