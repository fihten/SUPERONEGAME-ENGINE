#pragma once

#include <map>
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

	std::map<std::string, std::string> params;

public:
	std::string getTechnique() const;
	std::string getPass() const;

	enum StreamType
	{
		FLT1,
		FLT2,
		FLT3,
		FLT4
	};
	const void* getStream(const std::string& name, StreamType type) const;
	const std::vector<uint32_t>* getIndicies() const;

	friend Mesh&& createCube();
};