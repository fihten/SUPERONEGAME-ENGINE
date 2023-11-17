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
extern bool operator==(const ParamKey& p0, const ParamKey& p1);

struct ParamValue
{
	ParamValue() :valid(false) {}
	ParamValue(string_id s) :s(s), valid(true) {}
	ParamValue(float f) :f(f), valid(true) {}
	ParamValue(const flt2& f2) :f2(f2), valid(true) {}
	ParamValue(const flt3& f3) :f3(f3), valid(true) {}
	ParamValue(const flt4& f4) :f4(f4), valid(true) {}
	ParamValue(const flt4x4& f4x4) :f4x4(f4x4), valid(true) {}
	ParamValue(int i) :i(i), valid(true) {}
	ParamValue(const Vec2d<int>& i2) :i2(i2), valid(true) {}

	string_id s = string_id(-1);
	float f = 0;
	flt2 f2;
	flt3 f3;
	flt4 f4;
	flt4x4 f4x4;
	int i = 0;
	Vec2d<int> i2;

	bool valid = false;
};