#ifndef INTERSECTION_UTILS
#define INTERSECTION_UTILS

#include "Triangle.hlsl"
#include "Segment.hlsl"

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

bool checkSingleIntersection(Triangle tri, Segment seg)
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

bool doesContain(Envelope envelope, float3 pt, float threshold)
{
	if (envelope.min.x - threshold <= pt.x && pt.x <= envelope.max.x + threshold &&
		envelope.min.y - threshold <= pt.y && pt.y <= envelope.max.y + threshold &&
		envelope.min.z - threshold <= pt.z && pt.z <= envelope.max.z + threshold)
	{
		return true;
	}

	return false;
}

bool doesContain(Envelope envelope, Triangle tri, float threshold)
{
	if (doesContain(envelope, tri.v0, threshold) &&
		doesContain(envelope, tri.v1, threshold) &&
		doesContain(envelope, tri.v2, threshold))
	{
		return true;
	}
	return false;
}

#define FRONT_PLANE    0
#define BACK_PLANE     1
#define BOTTOM_PLANE   2
#define TOP_PLANE      3
#define LEFT_PLANE     4
#define RIGHT_PLANE    5

#define PLANE_NUMBER   6

bool checkIntersectionBySide(Envelope env, Triangle tri, float threshold)
{
	float3 n[PLANE_NUMBER];
	n[FRONT_PLANE] = float3(0, 0, -1);
	n[BACK_PLANE] = float3(0, 0, +1);
	n[BOTTOM_PLANE] = float3(0, -1, 0);
	n[TOP_PLANE] = float3(0, +1, 0);
	n[LEFT_PLANE] = float3(-1, 0, 0);
	n[RIGHT_PLANE] = float3(+1, 0, 0);

	float3 r0[PLANE_NUMBER];
	r0[FRONT_PLANE] = float3(0, 0, 0);
	r0[BACK_PLANE] = float3(0, 0, 1);
	r0[BOTTOM_PLANE] = float3(0, env.min.y, 0);
	r0[TOP_PLANE] = float3(0, env.max.y, 0);
	r0[LEFT_PLANE] = float3(env.min.x, 0, 0);
	r0[RIGHT_PLANE] = float3(env.max.x, 0, 0);

	Envelope envelopes[PLANE_NUMBER];

	envelopes[FRONT_PLANE].min = float3(env.min.xy, 0);
	envelopes[FRONT_PLANE].max = float3(env.max.xy, 0);

	envelopes[BACK_PLANE].min = float3(env.min.xy, 1.0f);
	envelopes[BACK_PLANE].max = float3(env.max.xy, 1.0f);

	envelopes[BOTTOM_PLANE].min = float3(env.min.xy, 0);
	envelopes[BOTTOM_PLANE].max = float3(env.max.x, env.min.y, 1.0f);

	envelopes[TOP_PLANE].min = float3(env.min.x, env.max.y, 0);
	envelopes[TOP_PLANE].max = float3(env.max.xy, 1.0f);

	envelopes[LEFT_PLANE].min = float3(env.min.xy, 0);
	envelopes[LEFT_PLANE].max = float3(env.min.x, env.max.y, 1.0f);

	envelopes[RIGHT_PLANE].min = float3(env.max.x, env.min.y, 0);
	envelopes[RIGHT_PLANE].max = float3(env.max.xy, 1.0f);

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
		float4 plane = float4(n[pi], -dot(n[pi], r0[pi]));
		for (int si = 0; si < 3; ++si)
		{
			float t = 0;
			uint intersectionInfo = findIntersection(plane, segments[si], t);
			if (intersectionInfo != SEGMENT_SHARES_SINGLE_POINT_WITH_PLANE)
				continue;

			float3 intersection = t * (segments[si].v1 - segments[si].v0) + segments[si].v0;
			if (doesContain(envelopes[pi], intersection, threshold))
				return true;
		}
	}
	return false;
}

bool checkIntersectionByDiagonal(Envelope env, Triangle tri, float threshold)
{
	Segment diagonals[4];

	diagonals[0].v0 = env.min;
	diagonals[0].v1 = env.max;

	diagonals[1].v0 = float3(env.min.x, env.min.y, env.max.z);
	diagonals[1].v1 = float3(env.max.x, env.max.y, env.min.z);

	diagonals[2].v0 = float3(env.max.x, env.min.y, env.min.z);
	diagonals[2].v1 = float3(env.min.x, env.max.y, env.max.z);

	diagonals[3].v0 = float3(env.max.x, env.min.y, env.max.z);
	diagonals[3].v1 = float3(env.min.x, env.max.y, env.min.z);

	[unroll]
	for (int di = 0; di < 4; ++di)
	{
		if (checkSingleIntersection(tri, diagonals[di]))
			return true;
	}

	return false;
}

bool checkIntersection(Envelope env, Triangle tri, float threshold)
{
	if (doesContain(env, tri, threshold))
		return true;
	if (checkIntersectionBySide(env, tri, threshold))
		return true;
	if (checkIntersectionByDiagonal(env, tri, threshold))
		return true;
	return false;
}

#endif