#pragma once

#define _USE_MATH_DEFINES // for C++
#include <cmath>
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

		return r;
	};

	friend Vec4d<value_type> operator*(const Matrix4x4<value_type>& m, const Vec4d<value_type>& v)
	{
		Vec4d<value_type> r;

		r.x() = m.m[0] * v.x() + m.m[1] * v.y() + m.m[2] * v.z() + m.m[3] * v.w();
		r.y() = m.m[4] * v.x() + m.m[5] * v.y() + m.m[6] * v.z() + m.m[7] * v.w();
		r.z() = m.m[8] * v.x() + m.m[9] * v.y() + m.m[10] * v.z() + m.m[11] * v.w();
		r.w() = m.m[12] * v.x() + m.m[13] * v.y() + m.m[14] * v.z() + m.m[15] * v.w();

		return r;
	};
};

template<class value_type>
Matrix4x4<value_type> makePerspectiveProjection(const value_type& aspectRatio, const value_type& fovy, const value_type& near, const value_type& far)
{
	value_type fovy_ = fovy * M_PI / 180;
	value_type tinv = 1 / tan(0.5 * fovy_);
	value_type m22 = far / (far - near);

	return Matrix4x4<value_type>(
		tinv / aspectRatio, value_type(0), value_type(0), value_type(0),
		value_type(0), tinv, value_type(0), value_type(0),
		value_type(0), value_type(0), m22, value_type(0),
		value_type(0), value_type(0), -near * m22, value_type(0)
		);
}

template<class value_type>
Matrix4x4<value_type> makeOrthographicProjection(const value_type& left, const value_type& right, const value_type& bottom, const value_type& top, const value_type& near, const value_type& far)
{
	return Matrix4x4<value_type>(
		2.0 / (right - left), 0, 0, 0,
		0, 2.0 / (top - bottom), 0, 0,
		0, 0, 1.0 / (far - near), 0,
		-(right + left) / (right - left), -(top + bottom) / (top - bottom), -near / (far - near), 1
		);
}

typedef Matrix4x4<float> flt4x4;
typedef Matrix4x4<double> dbl4x4;