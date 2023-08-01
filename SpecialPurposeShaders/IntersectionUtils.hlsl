#ifndef INTERSECTION_UTILS
#define INTERSECTION_UTILS

#include "Triangle.hlsl"
#include "Segment.hlsl"
#include "Frustum.hlsl"

#define SEGMENT_DOESNT_SHARE_ANY_POINT_WITH_PLANE 0
#define SEGMENT_SHARES_SINGLE_POINT_WITH_PLANE 1
#define SEGMENT_LIES_IN_PLANE 2

uint findIntersection(float4 plane, Segment seg, out float t)
{
	t = 0;

	float N = dot(plane.xyz, seg.v0) + plane.w;
	float D = dot(plane.xyz, seg.v1 - seg.v0);

	if (N == 0 && D == 0)
		return SEGMENT_LIES_IN_PLANE;

	if (D == 0)
		return SEGMENT_DOESNT_SHARE_ANY_POINT_WITH_PLANE;

	t = -N / D;
	if (t < 0 || 1 < t)
		return SEGMENT_DOESNT_SHARE_ANY_POINT_WITH_PLANE;

	return SEGMENT_SHARES_SINGLE_POINT_WITH_PLANE;
}

bool checkIntersection(Triangle tri, Segment seg)
{
	float3 t0 = tri.v1 - tri.v0;
	float3 t1 = tri.v2 - tri.v0;

	float3 n = cross(t0, t1);
	n = normalize(n);

	float4 plane = float4(n, -dot(n, tri.v0));
	float t = 0;
	uint intersectionInfo = findIntersection(plane, seg, t);
	if (intersectionInfo != SEGMENT_SHARES_SINGLE_POINT_WITH_PLANE)
		return false;

	float3 intersection = t * (seg.v1 - seg.v0) + seg.v0;

	float m00 = dot(tri.v0 - tri.v2, tri.v0 - tri.v2);
	float m01 = dot(tri.v1 - tri.v2, tri.v0 - tri.v2);

	float m10 = m01;
	float m11 = dot(tri.v1 - tri.v2, tri.v1 - tri.v2);

	float r0 = dot(intersection - tri.v2, tri.v0 - tri.v2);
	float r1 = dot(intersection - tri.v2, tri.v1 - tri.v2);

	float d = m00 * m11 - m01 * m10;
	float d0 = r0 * m11 - r1 * m01;
	float d1 = r1 * m00 - r0 * m10;

	float a = d0 / d;
	float b = d1 / d;
	float c = 1.0f - a - b;

	return 
		0.0f <= a && a <= 1.0f &&
		0.0f <= b && b <= 1.0f &&
		0.0f <= c && c <= 1.0f;
}

bool checkIntersection(Frustum frustum, float4 sphere)
{
	bool bIntersect = true;

	float4 center = float4(sphere.xyz, 1);
	float radius = sphere.w;

	[unroll]
	for (int pi = 0; pi < PLANE_NUMBER; ++pi)
		bIntersect = bIntersect && (dot(frustum.plane[pi], center) < radius);

	return bIntersect;
}

bool doesContain(Frustum frustum, int plane_number, float3 pt)
{
	bool bContain = true;
	[unroll]
	for (int pi = 0; pi < plane_number; ++pi)
		bContain = bContain && (dot(frustum.plane[pi], float4(pt, 1)) < 0);
	return bContain;
}

bool doesContain(Frustum frustum, int plane_number, Triangle tri)
{
	return
		doesContain(frustum, plane_number, tri.v0) &&
		doesContain(frustum, plane_number, tri.v1) &&
		doesContain(frustum, plane_number, tri.v2);
}

bool checkIntersectionBySide(Frustum frustum, Triangle tri, float threshold)
{
	Frustum frustums[PLANE_NUMBER];

	frustums[FRONT_PLANE].plane[0] = frustum.plane[LEFT_PLANE];
	frustums[FRONT_PLANE].plane[1] = frustum.plane[RIGHT_PLANE];
	frustums[FRONT_PLANE].plane[2] = frustum.plane[TOP_PLANE];
	frustums[FRONT_PLANE].plane[3] = frustum.plane[BOTTOM_PLANE];

	frustums[BACK_PLANE].plane[0] = frustum.plane[LEFT_PLANE];
	frustums[BACK_PLANE].plane[1] = frustum.plane[RIGHT_PLANE];
	frustums[BACK_PLANE].plane[2] = frustum.plane[TOP_PLANE];
	frustums[BACK_PLANE].plane[3] = frustum.plane[BOTTOM_PLANE];

	frustums[LEFT_PLANE].plane[0] = frustum.plane[FRONT_PLANE];
	frustums[LEFT_PLANE].plane[1] = frustum.plane[BACK_PLANE];
	frustums[LEFT_PLANE].plane[2] = frustum.plane[TOP_PLANE];
	frustums[LEFT_PLANE].plane[3] = frustum.plane[BOTTOM_PLANE];

	frustums[RIGHT_PLANE].plane[0] = frustum.plane[FRONT_PLANE];
	frustums[RIGHT_PLANE].plane[1] = frustum.plane[BACK_PLANE];
	frustums[RIGHT_PLANE].plane[2] = frustum.plane[TOP_PLANE];
	frustums[RIGHT_PLANE].plane[3] = frustum.plane[BOTTOM_PLANE];

	frustums[TOP_PLANE].plane[0] = frustum.plane[FRONT_PLANE];
	frustums[TOP_PLANE].plane[1] = frustum.plane[BACK_PLANE];
	frustums[TOP_PLANE].plane[2] = frustum.plane[LEFT_PLANE];
	frustums[TOP_PLANE].plane[3] = frustum.plane[RIGHT_PLANE];

	frustums[BOTTOM_PLANE].plane[0] = frustum.plane[FRONT_PLANE];
	frustums[BOTTOM_PLANE].plane[1] = frustum.plane[BACK_PLANE];
	frustums[BOTTOM_PLANE].plane[2] = frustum.plane[LEFT_PLANE];
	frustums[BOTTOM_PLANE].plane[3] = frustum.plane[RIGHT_PLANE];

	int plane_number = 4;

	Segment segments[3];

	segments[0].v0 = tri.v0;
	segments[0].v1 = tri.v1;

	segments[1].v0 = tri.v1;
	segments[1].v1 = tri.v2;

	segments[2].v0 = tri.v0;
	segments[2].v1 = tri.v2;

	[unroll]
	for (int pi = 0; pi < PLANE_NUMBER; ++pi)
	{
		[unroll]
		for (int si = 0; si < 3; ++si)
		{
			float t = 0;
			uint intersectionInfo = findIntersection(frustum.plane[pi], segments[si], t);
			if (intersectionInfo != SEGMENT_SHARES_SINGLE_POINT_WITH_PLANE)
				continue;

			float3 intersection = t * (segments[si].v1 - segments[si].v0) + segments[si].v0;
			if (doesContain(frustums[pi], plane_number, intersection))
				return true;
		}
	}
	return false;
}

bool checkIntersection(Frustum frustum, StructuredBuffer<Segment> diagonals, Triangle tri, float threshold)
{
	if (doesContain(frustum, PLANE_NUMBER, tri))
		return true;
	if (checkIntersectionBySide(frustum, tri, threshold))
		return true;
	
	[unroll]
	for (int di = 0; di < 4; di++)
	{
		if (checkIntersection(tri, diagonals[di]))
			return true;
	}

	return false;
}


#endif