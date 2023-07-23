#pragma once

#include <map>
#include <vector>
#include <string>

#include "Vec1d.h"
#include "Vec2d.h"
#include "Vec3d.h"
#include "Vec4d.h"
#include "Matrix4x4.h"

class Mesh;
Mesh createCube();
Mesh createTrees();
Mesh createSelectionBoxes();
Mesh createSphere(int latitudes, int longitudes);

class Mesh
{
protected:
	static uint32_t instanceNumber;
private:
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

	virtual std::string getTechnique() const;
	virtual std::string getPass() const;

	enum StreamType
	{
		FLT1,
		FLT2,
		FLT3,
		FLT4
	};
	virtual const void* getStream(const std::string& name, StreamType type) const;
	virtual const std::vector<uint32_t>* getIndicies() const;
	
	virtual std::string getName() const;
	virtual void setName(const std::string& name);

	virtual uint32_t getVerticesCount() const;
	virtual uint32_t getIndicesCount() const;

	virtual std::string getParam(const std::string& param) const;
	virtual void setParam(const std::string& param, const std::string& value);

	virtual flt4x4 getPosition() const { return flt4x4(); }

	virtual flt3 getBottomBorder() const;
	virtual flt3 getTopBorder() const;

	friend Mesh createCube();
	friend Mesh createTrees();
	friend Mesh createSelectionBoxes();
	friend Mesh createSphere(int latitudes, int longitudes);
public:
	uint32_t id;
	void* scene = nullptr;
	int nodeID = -1;

	void* gpuReadyData = nullptr;
	uint32_t verticesCount = 0;
	uint32_t elementSize = 0;
};