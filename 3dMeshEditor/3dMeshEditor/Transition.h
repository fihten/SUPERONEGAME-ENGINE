#pragma once
#include "Modifier.h"
#include "FrameOfReferenceState.h"

class Transition :public Modifier
{
	IntersectedAxis grabbedAxis = IntersectedAxis::NONE;
	float paramOnAxis;

	float mousePosX = 0;
	float mousePosY = 0;

public:
	void processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};