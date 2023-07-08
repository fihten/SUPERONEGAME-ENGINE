#pragma once

#include "Vec3d.h"
#include "Matrix4x4.h"

struct Envelope
{
	flt3 min;
	float pad0;

	flt3 max;
	float pad1;

	flt4x4 transform;
};