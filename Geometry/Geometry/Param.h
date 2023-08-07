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

extern bool operator<(const ParamKey& p0, const ParamKey& p1);

struct ParamValue
{
	ParamValue(){}
	ParamValue(string_id s) :s(s) {}
	ParamValue(float f) :f(f) {}
	ParamValue(flt2 f2) :f2(f2) {}
	ParamValue(flt3 f3) :f3(f3) {}
	ParamValue(flt4 f4) :f4(f4) {}
	ParamValue(flt4x4 f4x4) :f4x4(f4x4) {}

	string_id s = string_id(-1);
	float f = 0;
	flt2 f2;
	flt3 f3;
	flt4 f4;
	flt4x4 f4x4;
};