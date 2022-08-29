#pragma once

template<class value_type>
class Vec1d
{
	value_type v;
public:
	Vec1d() { v = 0; };
	Vec1d(value_type v) { this->v = v; };

	Vec1d<value_type> operator+(const Vec1d<value_type>&) const;
	Vec1d<value_type> operator-(const Vec1d<value_type>&) const;

	friend value_type dot(const Vec1d<value_type>& v1, const Vec1d<value_type>& v2);
};

template<class value_type>
inline Vec1d<value_type> Vec1d<value_type>::operator+(const Vec1d<value_type>& v) const
{
	Vec1d r;
	r.v = this->v + v.v;

	return r;
}

template<class value_type>
inline Vec1d<value_type> Vec1d<value_type>::operator-(const Vec1d<value_type>& v) const
{
	Vec1d r;
	r.v = this->v - v.v;

	return r;
}

template<class value_type>
value_type dot(const Vec1d<value_type>& v1, const Vec1d<value_type>& v2)
{
	return Vec1d<value_type>(v1.v * v2.v);
}
