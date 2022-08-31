#pragma once

template<class value_type>
class Vec3d
{
	value_type v[3];

public:
	Vec3d() { v[0] = value_type(0); v[1] = value_type(0); v[2] = value_type(0); };
	Vec3d(const value_type& x, const value_type& y, const value_type& z) { v[0] = x; v[1] = y; v[2] = z; };
};