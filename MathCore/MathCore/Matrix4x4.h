#pragma once
#include "StringToNumbers.h"

#include <cmath>
#include "Vec3d.h"
#include "Vec4d.h"

template<class value_type>
class Matrix4x4
{
	value_type m[16];

public:
	Matrix4x4()
	{
		m[0] = 1;		m[1] = 0;		m[2] = 0;		m[3] = 0;
		m[4] = 0;		m[5] = 1;		m[6] = 0;		m[7] = 0;
		m[8] = 0;		m[9] = 0;		m[10] = 1;	    m[11] = 0;
		m[12] = 0;	    m[13] = 0;	    m[14] = 0;	    m[15] = 1;
	};

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

	Matrix4x4(const std::string& str) {
		stringToNumbers(str, dynamic_cast<float*>(m), sizeof m / sizeof * m);
	};

	const value_type* getBuf() const { return m; };

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

	friend Vec3d<value_type> operator*(Vec3d<value_type>& v, Matrix4x4<value_type>& m)
	{
		Vec3d<value_type> r;

		r.x() = v.x() * m.m[0] + v.y() * m.m[4] + v.z() * m.m[8];
		r.y() = v.x() * m.m[1] + v.y() * m.m[5] + v.z() * m.m[9];
		r.z() = v.x() * m.m[2] + v.y() * m.m[6] + v.z() * m.m[10];

		return r;
	};

	friend Vec3d<value_type> operator*(Matrix4x4<value_type>& m, Vec3d<value_type>& v)
	{
		Vec3d<value_type> r;

		r.x() = m.m[0] * v.x() + m.m[1] * v.y() + m.m[2] * v.z();
		r.y() = m.m[4] * v.x() + m.m[5] * v.y() + m.m[6] * v.z();
		r.z() = m.m[8] * v.x() + m.m[9] * v.y() + m.m[10] * v.z();

		return r;
	};

	friend Matrix4x4<value_type> operator*(const Matrix4x4<value_type>& x, const Matrix4x4<value_type>& y)
	{
		Matrix4x4<value_type> r;

		int n = 4;
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				r.m[i * n + j] = 0;
				for (int k = 0; k < n; ++k)
					r.m[i * n + j] += x.m[i * n + k] * y.m[k * n + j];
			}
		}

		return r;
	}

	value_type& m00() { return m[0]; };
	value_type& m01() { return m[1]; };
	value_type& m02() { return m[2]; };
	value_type& m03() { return m[3]; };

	value_type& m10() { return m[4]; };
	value_type& m11() { return m[5]; };
	value_type& m12() { return m[6]; };
	value_type& m13() { return m[7]; };

	value_type& m20() { return m[8]; };
	value_type& m21() { return m[9]; };
	value_type& m22() { return m[10]; };
	value_type& m23() { return m[11]; };

	value_type& m30() { return m[12]; };
	value_type& m31() { return m[13]; };
	value_type& m32() { return m[14]; };
	value_type& m33() { return m[15]; };
};

template<class value_type>
Matrix4x4<value_type> makePerspectiveProjection(const value_type& _aspectRatio, const value_type& _fovy, const value_type& _near, const value_type& _far)
{
	value_type fovy = _fovy * M_PI / 180;
	value_type tinv = 1 / tan(0.5 * fovy);
	value_type m22 = _far / (_far - _near);

	return Matrix4x4<value_type>(
		tinv / _aspectRatio, value_type(0), value_type(0), value_type(0),
		value_type(0), tinv, value_type(0), value_type(0),
		value_type(0), value_type(0), m22, value_type(1),
		value_type(0), value_type(0), -_near * m22, value_type(0)
		);
}

template<class value_type>
Matrix4x4<value_type> makeOrthographicProjection(const value_type& _left, const value_type& _right, const value_type& _bottom, const value_type& _top, const value_type& _near, const value_type& _far)
{
	return Matrix4x4<value_type>(
		2.0 / (_right - _left), 0, 0, 0,
		0, 2.0 / (_top - _bottom), 0, 0,
		0, 0, 1.0 / (_far - _near), 0,
		-(_right + _left) / (_right - _left), -(_top + _bottom) / (_top - _bottom), -_near / (_far - _near), 1
		);
}

template<class value_type>
Matrix4x4<value_type> makeRotate(const Vec3d<value_type>&  axis, value_type angle)
{
	angle = M_PI * angle / 180;
	float c = cos(angle);
	float s = sin(angle);

	Matrix4x4<value_type> rot;

	Vec3d<value_type> b[3] = { Vec3d<value_type>(1,0,0),Vec3d<value_type>(0,1,0),Vec3d<value_type>(0,0,1) };
	float threshold = 0.1f;
	for (int index = 0; index < 3; ++index)
	{
		if (cross<value_type>(axis, b[index]).length() / axis.length() > threshold)
		{
			Vec3d<value_type> i = cross<value_type>(b[index], axis);
			Vec3d<value_type> j = cross<value_type>(axis, i);
			Vec3d<value_type> k = cross<value_type>(i, j);

			i.normalize();
			j.normalize();
			k.normalize();

			Matrix4x4<value_type> m1;
			m1.m00() = i.x(); m1.m01() = j.x(); m1.m02() = k.x(); m1.m03() = 0;
			m1.m10() = i.y(); m1.m11() = j.y(); m1.m12() = k.y(); m1.m13() = 0;
			m1.m20() = i.z(); m1.m21() = j.z(); m1.m22() = k.z(); m1.m23() = 0;
			m1.m30() = 0;     m1.m31() = 0;     m1.m32() = 0;     m1.m33() = 1;

			Matrix4x4<value_type> m2;
			m2.m00() = c;  m2.m01() = s; m2.m02() = 0; m2.m03() = 0;
			m2.m10() = -s; m2.m11() = c; m2.m12() = 0; m2.m13() = 0;
			m2.m20() = 0;  m2.m21() = 0; m2.m22() = 1; m2.m23() = 0;
			m2.m30() = 0;  m2.m31() = 0; m2.m32() = 0; m2.m33() = 1;

			Matrix4x4<value_type> m3;
			m3.m00() = i.x(); m3.m01() = i.y(); m3.m02() = i.z(); m3.m03() = 0;
			m3.m10() = j.x(); m3.m11() = j.y(); m3.m12() = j.z(); m3.m13() = 0;
			m3.m20() = k.x(); m3.m21() = k.y(); m3.m22() = k.z(); m3.m23() = 0;
			m3.m30() = 0;     m3.m31() = 0;     m3.m32() = 0;     m3.m33() = 1;

			rot = m1 * m2 * m3;

			break;
		}
	}

	return rot;
}

typedef Matrix4x4<float> flt4x4;
typedef Matrix4x4<double> dbl4x4;