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