#pragma once

template<class value_type> 
class Vec4d
{
	value_type v[4];

public:
	Vec4d() { v[0] = value_type(0); v[1] = value_type(0); v[2] = value_type(0); v[3] = value_type(0); };
	Vec4d(const value_type& x, const value_type& y, const value_type& z, const value_type& w) { v[0] = x; v[1] = y; v[2] = z; v[3] = w; };
};