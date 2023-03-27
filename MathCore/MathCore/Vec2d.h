#pragma once

template<class value_type>
class Vec2d
{
	value_type v[2];

public:
	Vec2d() { v[0] = value_type(0); v[1] = value_type(0); };
	Vec2d(const value_type& x, const value_type& y) { v[0] = x; v[1] = y; };
	Vec2d(const std::string& str) { 
		stringToNumbers(str, dynamic_cast<float*>(&v), sizeof v / sizeof * v); 
	};

	Vec2d<value_type> operator+(const Vec2d<value_type>& v) const;
	Vec2d<value_type> operator-(const Vec2d<value_type>& v) const;

	friend value_type dot(const Vec2d<value_type>& v1, const Vec2d<value_type>& v2);
	friend value_type cross(const Vec2d<value_type>& v1, const Vec2d<value_type>& v2);
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

template<class value_type>
value_type dot(const Vec2d<value_type>& v1, const Vec2d<value_type>& v2)
{
	return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1];
}

template<class value_type>
value_type cross(const Vec2d<value_type>& v1, const Vec2d<value_type>& v2)
{
	return v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0];
}

typedef Vec2d<float> flt2;
typedef Vec2d<double> dbl2;