#include "pch.h"
#include "MathUtils.h"
#include <algorithm>

float distanceBetweenLines(
	const flt3& pt0, const flt3& dir0,
	const flt3& pt1, const flt3& dir1,
	float* pT0, float* pT1
)
{
	float l00 = dot(dir0, dir0);
	float l01 = -dot(dir0, dir1);

	float l10 = -l01;
	float l11 = -dot(dir1, dir1);

	float r0 = dot(dir0, pt1 - pt0);
	float r1 = dot(dir1, pt1 - pt0);

	float det = l00 * l11 - l01 * l10;
	float det0 = r0 * l11 - r1 * l01;
	float det1 = l00 * r1 - l10 * r0;

	float t0 = det0 / det;
	float t1 = det1 / det;

	if (pT0)
		* pT0 = t0;
	if (pT1)
		* pT1 = t1;

	return ((pt1 + dir1 * t1) - (pt0 + dir0 * t0)).length();
}

float distanceBetweenLineAndPoint(const flt3& pt, const flt3& dir, const flt3& point)
{
	return cross(dir, point - pt).length() / dir.length();
}

float distanceBetweenLineAndSegment(
	const flt3& pt, const flt3& dir,
	const flt3& a, const flt3& b
)
{
	float distance = distanceBetweenLineAndPoint(pt, dir, a);
	distance = std::min<float>(
		distance, distanceBetweenLineAndPoint(pt, dir, b));

	float t0 = 0;
	float t1 = 0;
	float d = distanceBetweenLines(pt, dir, a, b - a, &t0, &t1);
	if (0.0f <= t1 && t1 <= 1.0f)
		distance = d;

	return distance;
}

bool linePlaneIntersection(
	const flt3& pt, const flt3& dir,
	const flt3& o, const flt3& e1, const flt3& e2,
	flt3& pointOfIntersection
)
{
	float det = -dot(cross(e1, e2), dir);
	float det0 = -dot(cross(pt - o, e2), dir);
	float det1 = -dot(cross(e1, pt - o), dir);
	float det2 = dot(cross(e1, e2), pt - o);

	if (det == 0)
		return false;

	pointOfIntersection.x() = det0 / det;
	pointOfIntersection.y() = det1 / det;
	pointOfIntersection.z() = det2 / det;

	return true;
}

bool checkIntersection(
	const flt3& triV0, const flt3& triV1, const flt3& triV2,
	const flt3& segV0, const flt3& segV1,
	float& t, flt2& barycentricCoords)
{
	flt3 t0 = triV1 - triV0;
	flt3 t1 = triV2 - triV0;

	flt3 n = cross(t0, t1);
	n.normalize();

	flt4 plane = flt4(n, -dot(n, triV0));
	t = 0;
	uint32_t intersectionInfo = findIntersection(plane, segV0, segV1, t);
	if (intersectionInfo != SEGMENT_SHARES_SINGLE_POINT_WITH_PLANE)
		return false;

	flt3 intersection = t * (segV1 - segV0) + segV0;

	float m00 = dot(triV0 - triV2, triV0 - triV2);
	float m01 = dot(triV1 - triV2, triV0 - triV2);

	float m10 = m01;
	float m11 = dot(triV1 - triV2, triV1 - triV2);

	float r0 = dot(intersection - triV2, triV0 - triV2);
	float r1 = dot(intersection - triV2, triV1 - triV2);

	float d = m00 * m11 - m01 * m10;
	float d0 = r0 * m11 - r1 * m01;
	float d1 = r1 * m00 - r0 * m10;

	float a = d0 / d;
	float b = d1 / d;
	float c = 1.0f - a - b;

	barycentricCoords.x() = a;
	barycentricCoords.y() = b;

	return
		0.0f <= a && a <= 1.0f &&
		0.0f <= b && b <= 1.0f &&
		0.0f <= c && c <= 1.0f;
}

uint32_t findIntersection(
	const flt4& plane,
	const flt3& segV0, const flt3& segV1,
	float& t
)
{
	t = 0;

	float N = dot(plane.xyz(), segV0) + plane.w();
	float D = dot(plane.xyz(), segV1 - segV0);

	if (N == 0 && D == 0)
		return SEGMENT_LIES_IN_PLANE;

	if (D == 0)
		return SEGMENT_DOESNT_SHARE_ANY_POINT_WITH_PLANE;

	t = -N / D;
	if (t < 0 || 1 < t)
		return SEGMENT_DOESNT_SHARE_ANY_POINT_WITH_PLANE;

	return SEGMENT_SHARES_SINGLE_POINT_WITH_PLANE;
}