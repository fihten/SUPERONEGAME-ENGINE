#pragma once

template<class value_type>
class Vec1d
{
	value_type v;
public:
	Vec1d operator+(const Vec1d&) const;
	Vec1d operator-(const Vec1d&) const;
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