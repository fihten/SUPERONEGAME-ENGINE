#ifndef INTERSECTION_UTILS
#define INTERSECTION_UTILS

#include "Triangle.hlsl"
#include "Segment.hlsl"

uint findIntersection(float4 plane, Segment seg, out float t)
{
	float N = dot(plane.xyz, seg.v0) + plane.w;
	float D = dot(plane.xyz, seg.v1 - seg.v0);

	if (N == 0 && D == 0)
		return 2;

	if (D == 0)
		return 0;

	t = -N / D;
	return 1;
}

bool checkIntersection(Envelope env, Triangle tri)
{

}

#endif