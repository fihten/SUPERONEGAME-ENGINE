#pragma once

#include "Vec4d.h"

template<class value_type>
class Matrix4x4
{
	value_type m[16];

public:
	Matrix4x4(
		const value_type& m00, const value_type& m01, const value_type& m02, const value_type& m03,
		const value_type& m10, const value_type& m11, const value_type& m12, const value_type& m13,
		const value_type& m20, const value_type& m21, const value_type& m22, const value_type& m23,
		const value_type& m30, const value_type& m31, const value_type& m32, const value_type& m33
	)
	{
		m[0] = m00;		m[1] = m01;		m[2] = m02;		m[3] = m03;
		m[4] = m10;		m[5] = m11;		m[6] = m12;		m[7] = m13;
		m[8] = m20;		m[9] = m21;		m[10] = m22;	m[11] = m23;
		m[12] = m30;	m[13] = m31;	m[14] = m32;	m[15] = m33;
	};

	friend Vec4d<value_type> operator*(const Vec4d<value_type>& v, const Matrix4x4<value_type>& m)
	{
		Vec4d<value_type> r;

		r.x() = v.x() * m.m[0] + v.y() * m.m[4] + v.z() * m.m[8] + v.w() * m.m[12];
		r.y() = v.x() * m.m[1] + v.y() * m.m[5] + v.z() * m.m[9] + v.w() * m.m[13];
		r.z() = v.x() * m.m[2] + v.y() * m.m[6] + v.z() * m.m[10] + v.w() * m.m[14];
		r.w() = v.x() * m.m[3] + v.y() * m.m[7] + v.z() * m.m[11] + v.w() * m.m[15];

		return Vec4d<value_type>();
	}
};