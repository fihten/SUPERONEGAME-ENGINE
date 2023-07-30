#pragma once

#include "Vec4d.h"

#define FRONT_PLANE    0
#define BACK_PLANE     1
#define BOTTOM_PLANE   2
#define TOP_PLANE      3
#define LEFT_PLANE     4
#define RIGHT_PLANE    5

#define PLANE_NUMBER   6

struct Frustum
{
	flt4 plane[PLANE_NUMBER];
};