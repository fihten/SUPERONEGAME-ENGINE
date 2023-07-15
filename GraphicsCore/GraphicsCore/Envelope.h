#pragma once

#include "Vec3d.h"
#include "Matrix4x4.h"

#define MaxEnvelopesCount 512
struct Envelope
{
	flt3 min;
	flt3 max;
	flt4x4 transform;
};