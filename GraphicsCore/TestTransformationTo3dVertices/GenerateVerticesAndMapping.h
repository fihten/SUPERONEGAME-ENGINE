#pragma once

#include <vector>
#include "Vec2d.h"
#include "Vec3d.h"

void generateVerticesAndMapping(
	std::vector<flt2>& points,
	std::vector<Vec2d<uint32_t>>& mapping
);