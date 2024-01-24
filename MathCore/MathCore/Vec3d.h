#pragma once
#include "StringToNumbers.h"
#include "NumbersToString.h"
#include <cmath>

template<class value_type>
class Matrix4x4;

template<class value_type>
class Vec3d;

template<class value_type>
value_type dot(const Vec3d<value_type>& v1, const Vec3d<value_type>& v2);

template<class value_type>
Vec3d<value_type> cross(const Vec3d<value_type>& v1, const Vec3d<value_type>& v2);

template<class value_type>
Vec3d<value_type> operator*(const Vec3d<value_type>& vec, const value_type& x);

template<class value_type>
Vec3d<value_type> operator*(const value_type& x, const Vec3d<value_type>& vec);

template<class value_type>
Vec3d<value_type> operator*(const Vec3d<value_type>& v, const Matrix4x4<value_type>& m);

template<class value_type>
class Vec3d
{
	value_type v[3];

public:
	Vec3d() { v[0] = value_type(0); v[1] = value_type(0); v[2] = value_type(0); };
	Vec3d(const value_type& x, const value_type& y, const value_type& z) { v[0] = x; v[1] = y; v[2] = z; };
	Vec3d(const std::string& str) {
		stringToNumbers(str, (float*)(v), sizeof v / sizeof * v);
	};

	Vec3d<value_type> operator+(const Vec3d<value_type>& v) const;
	Vec3d<value_type> operator-(const Vec3d<value_type>& v) const;
	Vec3d<value_type> operator/(const value_type& x) const;
	Vec3d<value_type>& operator+=(const Vec3d<value_type>& v);
	Vec3d<value_type>& operator/=(const value_type& v);
	Vec3d<value_type>& operator*=(const value_type& v);
	operator std::string()const {
		return numbersToString((float*)(v), sizeof v / sizeof * v);
	};

	value_type& x() { return v[0]; };
	value_type& y() { return v[1]; };
	value_type& z() { return v[2]; };

	value_type length() const;
	value_type normalize();

	friend value_type dot<>(const Vec3d<value_type>& v1, const Vec3d<value_type>& v2);
	friend Vec3d<value_type> cross<>(const Vec3d<value_type>& v1, const Vec3d<value_type>& v2);
	friend Vec3d<value_type> operator*<>(const Vec3d<value_type>& vec, const value_type& x);
	friend Vec3d<value_type> operator*<>(const value_type& x, const Vec3d<value_type>& vec);
	friend Vec3d<value_type> operator*<>(const Vec3d<value_type>& v, const Matrix4x4<value_type>& m);
};

template<class value_type>
Vec3d<value_type> Vec3d<value_type>::operator+(const Vec3d<value_type>& v) const
{
	return Vec3d<value_type>(this->v[0] + v.v[0], this->v[1] + v.v[1], this->v[2] + v.v[2]);
}

template<class value_type>
Vec3d<value_type> Vec3d<value_type>::operator-(const Vec3d<value_type>& v) const
{
	return Vec3d<value_type>(this->v[0] - v.v[0], this->v[1] - v.v[1], this->v[2] - v.v[2]);
}

template<class value_type>
Vec3d<value_type> Vec3d<value_type>::operator/(const value_type& x) const
{
	return Vec3d<value_type>(this->v[0] / x, this->v[1] / x, this->v[2] / x);
}

template<class value_type>
Vec3d<value_type>& Vec3d<value_type>::operator+=(const Vec3d<value_type>& v)
{
	this->x() += const_cast<Vec3d<value_type>&>(v).x();
	this->y() += const_cast<Vec3d<value_type>&>(v).y();
	this->z() += const_cast<Vec3d<value_type>&>(v).z();

	return *this;
}

template<class value_type>
Vec3d<value_type>& Vec3d<value_type>::operator/=(const value_type& v)
{
	this->x() /= v;
	this->y() /= v;
	this->z() /= v;

	return *this;
}

template<class value_type>
Vec3d<value_type>& Vec3d<value_type>::operator*=(const value_type& v)
{
	this->x() *= v;
	this->y() *= v;
	this->z() *= v;

	return *this;
}

template<class value_type>
value_type dot(const Vec3d<value_type>& v1, const Vec3d<value_type>& v2)
{
	return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

template<class value_type>
Vec3d<value_type> cross(const Vec3d<value_type>& v1, const Vec3d<value_type>& v2)
{
	return Vec3d<value_type>(v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1], v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2], v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]);
}

template<class value_type>
Vec3d<value_type> operator*(const Vec3d<value_type>& vec, const value_type& x)
{
	return Vec3d<value_type>(x * vec.v[0], x * vec.v[1], x * vec.v[2]);
}

template<class value_type>
Vec3d<value_type> operator*(const value_type& x, const Vec3d<value_type>& vec)
{
	return Vec3d<value_type>(x * vec.v[0], x * vec.v[1], x * vec.v[2]);
}

template<class value_type>
value_type Vec3d<value_type>::length() const
{
	return std::sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

template<class value_type>
value_type Vec3d<value_type>::normalize()
{
	value_type len = this->length();
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
	return len;
}

typedef Vec3d<float> flt3;
typedef Vec3d<double> dbl3;