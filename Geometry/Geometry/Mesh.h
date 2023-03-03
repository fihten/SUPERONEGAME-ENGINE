#pragma once

#include <map>
#include <vector>
#include <string>

#include "Vec1d.h"
#include "Vec2d.h"
#include "Vec3d.h"
#include "Vec4d.h"

class Mesh;
Mesh createCube();

class Mesh
{
	static uint32_t instanceNumber;
	std::string name = "";

	std::vector<std::pair<std::string, std::vector<flt1>>> flt1_streams;
	std::vector<std::pair<std::string, std::vector<flt2>>> flt2_streams;
	std::vector<std::pair<std::string, std::vector<flt3>>> flt3_streams;
	std::vector<std::pair<std::string, std::vector<flt4>>> flt4_streams;

	std::vector<uint32_t> indicies;

	std::map<std::string, std::string> params;

public:
	Mesh():id(instanceNumber)
	{
		++instanceNumber;
	}

	Mesh(const Mesh& m):id(instanceNumber)
	{
		++instanceNumber;

		this->name = m.name;

		this->flt1_streams = m.flt1_streams;
		this->flt2_streams = m.flt2_streams;
		this->flt3_streams = m.flt3_streams;
		this->flt4_streams = m.flt4_streams;

		this->indicies = m.indicies;

		this->params = m.params;
	}

	Mesh(Mesh&& m):id(instanceNumber)
	{
		++instanceNumber;

		this->name = std::move(m.name);

		this->flt1_streams = std::move(m.flt1_streams);
		this->flt2_streams = std::move(m.flt2_streams);
		this->flt3_streams = std::move(m.flt3_streams);
		this->flt4_streams = std::move(m.flt4_streams);

		this->indicies = std::move(m.indicies);

		this->params = std::move(m.params);
	}

	Mesh& operator=(Mesh&& m)
	{
		this->name = std::move(m.name);

		this->flt1_streams = std::move(m.flt1_streams);
		this->flt2_streams = std::move(m.flt2_streams);
		this->flt3_streams = std::move(m.flt3_streams);
		this->flt4_streams = std::move(m.flt4_streams);

		this->indicies = std::move(m.indicies);

		this->params = std::move(m.params);

		return *this;
	}

	Mesh& operator=(const Mesh& m)
	{
		this->name = m.name;

		this->flt1_streams = m.flt1_streams;
		this->flt2_streams = m.flt2_streams;
		this->flt3_streams = m.flt3_streams;
		this->flt4_streams = m.flt4_streams;

		this->indicies = m.indicies;

		this->params = m.params;

		return *this;
	}

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
	
	std::string getName() const;
	void setName(const std::string& name);

	uint32_t getVerticesCount() const;
	uint32_t getIndicesCount() const;

	friend Mesh createCube();

public:
	const uint32_t id;
	void* scene = nullptr;
	int nodeID = -1;
};