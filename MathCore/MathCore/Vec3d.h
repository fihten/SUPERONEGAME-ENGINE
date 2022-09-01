#pragma once

template<class value_type>
class Vec3d
{
	value_type v[3];

public:
	Vec3d() { v[0] = value_type(0); v[1] = value_type(0); v[2] = value_type(0); };
	Vec3d(const value_type& x, const value_type& y, const value_type& z) { v[0] = x; v[1] = y; v[2] = z; };

	Vec3d<value_type> operator+(const Vec3d<value_type>& v) const;
	Vec3d<value_type> operator-(const Vec3d<value_type>& v) const;

	friend value_type dot(const Vec3d<value_type>& v1, const Vec3d<value_type>& v2);
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
value_type dot(const Vec3d<value_type>& v1, const Vec3d<value_type>& v2)
{
	return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}