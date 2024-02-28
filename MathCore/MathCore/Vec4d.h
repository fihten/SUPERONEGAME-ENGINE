#pragma once
#include "StringToNumbers.h"
#include "NumbersToString.h"
#include "Vec3d.h"

template<class value_type>
class Vec4d;

template<class value_type>
value_type dot(const Vec4d<value_type>& v1, const Vec4d<value_type>& v2);

template<class value_type> 
class Vec4d
{
	value_type v[4];

public:
	Vec4d() { v[0] = value_type(0); v[1] = value_type(0); v[2] = value_type(0); v[3] = value_type(0); };
	Vec4d(const value_type& x, const value_type& y, const value_type& z, const value_type& w) { v[0] = x; v[1] = y; v[2] = z; v[3] = w; };
	Vec4d(const Vec3d<value_type>& xyz, const value_type& w)
	{
		v[0] = (const_cast<Vec3d<value_type>&>(xyz)).x();
		v[1] = (const_cast<Vec3d<value_type>&>(xyz)).y();
		v[2] = (const_cast<Vec3d<value_type>&>(xyz)).z();
		v[3] = w;
	};
	Vec4d(const std::string& str) {
		stringToNumbers(str, (float*)(v), sizeof v / sizeof * v);
	};

	Vec4d<value_type> operator+(const Vec4d<value_type>& v) const;
	Vec4d<value_type> operator-(const Vec4d<value_type>& v) const;
	Vec4d<value_type> operator*(const value_type& x) const;
	Vec4d<value_type> operator/(const value_type& x) const;
	operator std::string()const {
		return numbersToString((float*)(v), sizeof v / sizeof * v);
	};

	const value_type* getBuf() const { return v; };

	value_type& x() { return v[0]; };
	value_type& y() { return v[1]; };
	value_type& z() { return v[2]; };
	value_type& w() { return v[3]; };

	const value_type& x() const { return v[0]; };
	const value_type& y() const { return v[1]; };
	const value_type& z() const { return v[2]; };
	const value_type& w() const { return v[3]; };

	Vec3d<value_type>& xyz() { return *((Vec3d<value_type>*)this); };
	Vec3d<value_type> const& xyz() const { return *((Vec3d<value_type>*)this); };

	friend value_type dot<>(const Vec4d<value_type>& v1, const Vec4d<value_type>& v2);
};

template<class value_type>
Vec4d<value_type> Vec4d<value_type>::operator+(const Vec4d<value_type>& v) const
{
	return Vec4d<value_type>(this->v[0] + v.v[0], this->v[1] + v.v[1], this->v[2] + v.v[2], this->v[3] + v.v[3]);
}

template<class value_type>
Vec4d<value_type> Vec4d<value_type>::operator-(const Vec4d<value_type>& v) const
{
	return Vec4d<value_type>(this->v[0] - v.v[0], this->v[1] - v.v[1], this->v[2] - v.v[2], this->v[3] - v.v[3]);
}

template<class value_type>
Vec4d<value_type> Vec4d<value_type>::operator/(const value_type& x) const
{
	return Vec4d<value_type>(this->v[0] / x, this->v[1] / x, this->v[2] / x, this->v[3] / x);
}

template<class value_type>
Vec4d<value_type> Vec4d<value_type>::operator*(const value_type& x) const
{
	return Vec4d<value_type>(this->v[0] * x, this->v[1] * x, this->v[2] * x, this->v[3] * x);
}

template<class value_type>
value_type dot(const Vec4d<value_type>& v1, const Vec4d<value_type>& v2)
{
	return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2] + v1.v[3] * v2.v[3];
}

typedef Vec4d<float> flt4;
typedef Vec4d<double> dbl4;