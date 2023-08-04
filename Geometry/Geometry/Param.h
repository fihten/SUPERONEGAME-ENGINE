#pragma once
#include "StringManager.h"
#include "Vec2d.h"
#include "Vec3d.h"
#include "Vec4d.h"
#include "Matrix4x4.h"

struct ParamKey
{
	string_id name = string_id(-1);
	int index = -1;
	string_id field = string_id(-1);
};

bool operator<(const ParamKey& p0, const ParamKey& p1)
{
	if (p0.name > p1.name)
		return false;
	if (p0.name < p1.name)
		return true;
	if (p0.index > p1.index)
		return false;
	if (p0.index < p1.index)
		return true;
	if (p0.field < p1.field)
		return true;
	return false;
}

union ParamValue
{
	string_id s;
	float f;
	flt2 f2;
	flt3 f3;
	flt4 f4;
	flt4x4 f4x4;
};