#ifndef INTERSECTION_UTILS
#define INTERSECTION_UTILS

#include "Triangle.hlsl"
#include "Segment.hlsl"

uint findIntersection(float4 plane, Segment seg, out float t)
{
	t = 0;

	float N = dot(plane.xyz, seg.v0) + plane.w;
	float D = dot(plane.xyz, seg.v1 - seg.v0);

	if (N == 0 && D == 0)
		return 2;

	if (D == 0)
		return 0;

	t = -N / D;
	return 1;
}

#define FRONT_PLANE    0
#define BACK_PLANE     1
#define BOTTOM_PLANE   2
#define TOP_PLANE      3
#define LEFT_PLANE     4
#define RIGHT_PLANE    5

#define PLANE_NUMBER   6

bool checkIntersection(Envelope env, Triangle tri, float threshold)
{
	float3 n[PLANE_NUMBER];
	n[FRONT_PLANE]  = float3( 0,  0, -1);
	n[BACK_PLANE]   = float3( 0,  0, +1);
	n[BOTTOM_PLANE] = float3( 0, -1,  0);
	n[TOP_PLANE]    = float3( 0, +1,  0);
	n[LEFT_PLANE]   = float3(-1,  0,  0);
	n[RIGHT_PLANE]  = float3(+1,  0,  0);

	float3 r0[PLANE_NUMBER];
	r0[FRONT_PLANE]  =  float3(0, 0, 0);
	r0[BACK_PLANE]   =  float3(0, 0, 1);
	r0[BOTTOM_PLANE] =  float3(0, env.min.y, 0);
	r0[TOP_PLANE]    =  float3(0, env.max.y, 0);
	r0[LEFT_PLANE]   =  float3(env.min.x, 0, 0);
	r0[RIGHT_PLANE]  =  float3(env.max.x, 0, 0);

	float3 min[PLANE_NUMBER];
	min[FRONT_PLANE]  = float3(env.min.xy - threshold, -threshold);
	min[BACK_PLANE]   = float3(env.min.xy - threshold, 1.0f - threshold);
	min[BOTTOM_PLANE] = float3(env.min.xy - threshold, -threshold);
	min[TOP_PLANE]    = float3(env.min.x - threshold, env.max.y - threshold, -threshold);
	min[LEFT_PLANE]   = float3(env.min.xy - threshold, -threshold);
	min[RIGHT_PLANE]  = float3(env.max.x - threshold, end.min.y - threshold, -threshold);

	float3 max[PLANE_NUMBER];
	max[FRONT_PLANE]  = float3(env.max.xy + threshold, threshold);
	max[BACK_PLANE]   = float3(env.max.xy + threshold, 1.0f + threshold);
	max[BOTTOM_PLANE] = float3(env.max.x + threshold, env.min.y + threshold, 1.0f + threshold);
	max[TOP_PLANE]    = float3(env.max.xy + threshold, 1.0f + threshold);
	max[LEFT_PLANE]   = float3(env.min.x + threshold, env.max.y + threshold, 1.0f + threshold);
	max[RIGHT_PLANE]  = float3(env.max.xy + threshold, 1.0f + threshold);

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
			if (findIntersection(plane, segments[si], t) == 0)
				continue;


		}
	}
}

#endif