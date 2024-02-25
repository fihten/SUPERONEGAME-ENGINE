#pragma once
#include "Vec2d.h"
#include "Vec3d.h"
#include "Vec4d.h"

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

bool linePlaneIntersection(
	const flt3& pt, const flt3& dir,
	const flt3& o, const flt3& e1, const flt3& e2,
	flt3& pointOfIntersection
);

bool checkIntersection(
	const flt3& triV0, const flt3& triV1, const flt3& triV2,
	const flt3& segV0, const flt3& segV1,
	float& t, flt2& barycentricCoords);

#define SEGMENT_DOESNT_SHARE_ANY_POINT_WITH_PLANE 0
#define SEGMENT_SHARES_SINGLE_POINT_WITH_PLANE 1
#define SEGMENT_LIES_IN_PLANE 2

uint32_t findIntersection(
	const flt4& plane, 
	const flt3& segV0, const flt3& segV1,
	float& t
);