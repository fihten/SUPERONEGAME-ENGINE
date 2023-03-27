#pragma once
#include <string>
#include "StringToNumbers.h"

template<class value_type>
class Vec1d
{
	value_type v;
public:
	Vec1d() { v = value_type(0); };
	Vec1d(value_type v) { this->v = v; };
	Vec1d(const std::string& str) { stringToNumbers(str, dynamic_cast<float*>(&v), 1); };

	Vec1d<value_type> operator+(const Vec1d<value_type>&) const;
	Vec1d<value_type> operator-(const Vec1d<value_type>&) const;

	friend value_type dot(const Vec1d<value_type>& v1, const Vec1d<value_type>& v2);
	friend value_type cross(const Vec1d<value_type>& v1, const Vec1d<value_type>& v2);
};

template<class value_type>
inline Vec1d<value_type> Vec1d<value_type>::operator+(const Vec1d<value_type>& v) const
{
	Vec1d<value_type> r;
	r.v = this->v + v.v;

	return r;
}

template<class value_type>
inline Vec1d<value_type> Vec1d<value_type>::operator-(const Vec1d<value_type>& v) const
{
	Vec1d<value_type> r;
	r.v = this->v - v.v;

	return r;
}

template<class value_type>
value_type dot(const Vec1d<value_type>& v1, const Vec1d<value_type>& v2)
{
	return Vec1d<value_type>(v1.v * v2.v);
}

template<class value_type>
value_type cross(const Vec1d<value_type>& v1, const Vec1d<value_type>& v2)
{
	return value_type(0);
}

typedef Vec1d<float> flt1;
typedef Vec1d<double> dbl1;