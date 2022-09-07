#pragma once

#include <vector>
#include <string>

#include "Vec1d.h"
#include "Vec2d.h"
#include "Vec3d.h"
#include "Vec4d.h"

class Mesh
{
	std::vector<std::pair<std::string, std::vector<flt1>>> flt1_streams;
	std::vector<std::pair<std::string, std::vector<flt2>>> flt2_streams;
	std::vector<std::pair<std::string, std::vector<flt3>>> flt3_streams;
	std::vector<std::pair<std::string, std::vector<flt4>>> flt4_streams;

	std::vector<uint32_t> indicies;

	friend Mesh&& createCube();
};