#pragma once

#include "Vec3d.h"

struct ScaleFrameOfReference
{
	ScaleFrameOfReference() :
		axis0(1, 0, 0),
		axis1(0, 1, 0),
		axis2(0, 0, 1),
		handleRadius(0.1f) 
	{}

	flt3 posW;
	flt3 axis0;
	flt3 axis1;
	flt3 axis2;
	float handleRadius;
};