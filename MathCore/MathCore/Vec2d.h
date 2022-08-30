#pragma once

template<class value_type>
class Vec2d
{
	value_type v[2];

public:
	Vec2d() { v[0] = value_type(0); v[1] = value_type(0); };
	Vec2d(const value_type& x, const value_type& y) { v[0] = x; v[1] = y; };

	Vec2d<value_type> operator+(const Vec2d<value_type>& v) const;
	Vec2d<value_type> operator-(const Vec2d<value_type>& v) const;
};

template<class value_type>
Vec2d<value_type> Vec2d<value_type>::operator+(const Vec2d<value_type>& v) const
{
	Vec2d<value_type> r;
	for (int i = 0; i < 2; ++i)
		r.v[i] = this->v[i] + v.v[i];
	return r;
}

template<class value_type>
Vec2d<value_type> Vec2d<value_type>::operator-(const Vec2d<value_type>& v) const
{
	Vec2d<value_type> r;
	for (int i = 0; i < 2; ++i)
		r.v[i] = this->v[i] - v.v[i];
	return r;
}