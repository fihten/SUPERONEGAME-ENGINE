#pragma once
#include "Vec3d.h"

float distanceBetweenLines(
	const flt3& pt0, const flt3& dir0,
	const flt3& pt1, const flt3& dir1,
	float* pT0 = nullptr, float* pT1 = nullptr
);

float distanceBetweenLineAndPoint(
	const flt3& pt, const flt3& dir,
	const flt3& point
);

float distanceBetweenLineAndSegment(
	const flt3& pt, const flt3& dir,
	const flt3& a, const flt3& b
);