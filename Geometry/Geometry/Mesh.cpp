#include "pch.h"

#include <algorithm>
#include "Mesh.h"

uint32_t Mesh::instanceNumber = 0;

Mesh createCube()
{
	Mesh m;

	// vertices positions
	m.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("POSITION"), std::vector<flt3>()));

	auto& pts = m.flt3_streams.back().second;
	
	// bottom
	pts.push_back(flt3(-0.5, -0.5, -0.5));
	pts.push_back(flt3(-0.5, 0.5, -0.5));
	pts.push_back(flt3(0.5, 0.5, -0.5));
	pts.push_back(flt3(0.5, -0.5, -0.5));

	// top
	pts.push_back(flt3(-0.5, -0.5, 0.5));
	pts.push_back(flt3(-0.5, 0.5, 0.5));
	pts.push_back(flt3(0.5, 0.5, 0.5));
	pts.push_back(flt3(0.5, -0.5, 0.5));

	// left
	pts.push_back(flt3(-0.5, -0.5, -0.5));
	pts.push_back(flt3(-0.5, -0.5, 0.5));
	pts.push_back(flt3(0.5, -0.5, 0.5));
	pts.push_back(flt3(0.5, -0.5, -0.5));

	// right
	pts.push_back(flt3(-0.5, 0.5, -0.5));
	pts.push_back(flt3(-0.5, 0.5, 0.5));
	pts.push_back(flt3(0.5, 0.5, 0.5));
	pts.push_back(flt3(0.5, 0.5, -0.5));

	// back
	pts.push_back(flt3(-0.5, -0.5, -0.5));
	pts.push_back(flt3(-0.5, -0.5, 0.5));
	pts.push_back(flt3(-0.5, 0.5, 0.5));
	pts.push_back(flt3(-0.5, 0.5, -0.5));

	// front
	pts.push_back(flt3(0.5, -0.5, -0.5));
	pts.push_back(flt3(0.5, -0.5, 0.5));
	pts.push_back(flt3(0.5, 0.5, 0.5));
	pts.push_back(flt3(0.5, 0.5, -0.5));

	// vertices normals
	m.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("NORMAL"), std::vector<flt3>()));

	auto& nms = m.flt3_streams.back().second;

	// bottom
	nms.push_back(flt3(0.0, 0.0, -1.0));
	nms.push_back(flt3(0.0, 0.0, -1.0));
	nms.push_back(flt3(0.0, 0.0, -1.0));
	nms.push_back(flt3(0.0, 0.0, -1.0));

	// top
	nms.push_back(flt3(0.0, 0.0, 1.0));
	nms.push_back(flt3(0.0, 0.0, 1.0));
	nms.push_back(flt3(0.0, 0.0, 1.0));
	nms.push_back(flt3(0.0, 0.0, 1.0));

	// left
	nms.push_back(flt3(0.0, -1.0, 0.0));
	nms.push_back(flt3(0.0, -1.0, 0.0));
	nms.push_back(flt3(0.0, -1.0, 0.0));
	nms.push_back(flt3(0.0, -1.0, 0.0));

	// right
	nms.push_back(flt3(0.0, 1.0, 0.0));
	nms.push_back(flt3(0.0, 1.0, 0.0));
	nms.push_back(flt3(0.0, 1.0, 0.0));
	nms.push_back(flt3(0.0, 1.0, 0.0));

	// back
	nms.push_back(flt3(-1.0, 0.0, 0.0));
	nms.push_back(flt3(-1.0, 0.0, 0.0));
	nms.push_back(flt3(-1.0, 0.0, 0.0));
	nms.push_back(flt3(-1.0, 0.0, 0.0));

	// front
	nms.push_back(flt3(1.0, 0.0, 0.0));
	nms.push_back(flt3(1.0, 0.0, 0.0));
	nms.push_back(flt3(1.0, 0.0, 0.0));
	nms.push_back(flt3(1.0, 0.0, 0.0));

	// vertices colors
	m.flt4_streams.push_back(std::pair<std::string, std::vector<flt4>>(std::string("COLOR"), std::vector<flt4>()));

	auto& clrs = m.flt4_streams.back().second;
	
	// bottom
	clrs.push_back(flt4(0, 0, 1, 0));
	clrs.push_back(flt4(0, 0, 1, 0));
	clrs.push_back(flt4(0, 0, 1, 0));
	clrs.push_back(flt4(0, 0, 1, 0));

	// top
	clrs.push_back(flt4(0, 1, 0, 0));
	clrs.push_back(flt4(0, 1, 0, 0));
	clrs.push_back(flt4(0, 1, 0, 0));
	clrs.push_back(flt4(0, 1, 0, 0));

	// left
	clrs.push_back(flt4(0, 1, 1, 0));
	clrs.push_back(flt4(0, 1, 1, 0));
	clrs.push_back(flt4(0, 1, 1, 0));
	clrs.push_back(flt4(0, 1, 1, 0));

	// right
	clrs.push_back(flt4(1, 0, 0, 0));
	clrs.push_back(flt4(1, 0, 0, 0));
	clrs.push_back(flt4(1, 0, 0, 0));
	clrs.push_back(flt4(1, 0, 0, 0));

	// back
	clrs.push_back(flt4(1, 0, 1, 0));
	clrs.push_back(flt4(1, 0, 1, 0));
	clrs.push_back(flt4(1, 0, 1, 0));
	clrs.push_back(flt4(1, 0, 1, 0));

	// front
	clrs.push_back(flt4(1, 1, 0, 0));
	clrs.push_back(flt4(1, 1, 0, 0));
	clrs.push_back(flt4(1, 1, 0, 0));
	clrs.push_back(flt4(1, 1, 0, 0));

	// indices
	auto& inds = m.indicies;
	
	// bottom
	inds.push_back(2);
	inds.push_back(1);
	inds.push_back(0);
	
	inds.push_back(3);
	inds.push_back(2);
	inds.push_back(0);

	// top
	inds.push_back(4);
	inds.push_back(5);
	inds.push_back(6);

	inds.push_back(4);
	inds.push_back(6);
	inds.push_back(7);

	// left
	inds.push_back(8);
	inds.push_back(9);
	inds.push_back(10);

	inds.push_back(8);
	inds.push_back(10);
	inds.push_back(11);

	// right
	inds.push_back(14);
	inds.push_back(13);
	inds.push_back(12);

	inds.push_back(15);
	inds.push_back(14);
	inds.push_back(12);

	// back
	inds.push_back(18);
	inds.push_back(17);
	inds.push_back(16);

	inds.push_back(19);
	inds.push_back(18);
	inds.push_back(16);

	// front
	inds.push_back(20);
	inds.push_back(21);
	inds.push_back(22);

	inds.push_back(20);
	inds.push_back(22);
	inds.push_back(23);

	m.params["technique"] = "LightTech";
	m.params["pass"] = "P0";

	return m;
}

std::string Mesh::getTechnique() const
{
	std::string technique = "";
	
	if (params.count("technique"))
		technique = params.at("technique");
	
	return technique;
}

std::string Mesh::getPass() const
{
	std::string pass = "";
	
	if (params.count("pass"))
		pass = params.at("pass");

	return pass;
}

const void* Mesh::getStream(const std::string& name, StreamType type) const
{
	const void* rPtr = nullptr;
	switch (type)
	{
	case FLT1:
	{
		const auto& it = std::find_if(flt1_streams.begin(), flt1_streams.end(), [&](const std::pair<std::string, std::vector<flt1>>& el) -> bool
		{
			return el.first == name;
		});

		if (it != flt1_streams.end())
			rPtr = &it->second;

		break;
	}
	case FLT2:
	{
		const auto& it = std::find_if(flt2_streams.begin(), flt2_streams.end(), [&](const std::pair<std::string, std::vector<flt2>>& el) -> bool
		{
			return el.first == name;
		});

		if (it != flt2_streams.end())
			rPtr = &it->second;

		break;
	}
	case FLT3:
	{
		const auto& it = std::find_if(flt3_streams.begin(), flt3_streams.end(), [&](const std::pair<std::string, std::vector<flt3>>& el) -> bool
		{
			return el.first == name;
		});

		if (it != flt3_streams.end())
			rPtr = &it->second;

		break;
	}
	case FLT4:
	{
		const auto& it = std::find_if(flt4_streams.begin(), flt4_streams.end(), [&](const std::pair<std::string, std::vector<flt4>>& el) -> bool
		{
			return el.first == name;
		});

		if (it != flt4_streams.end())
			rPtr = &it->second;

		break;
	}
	}

	return rPtr;
}

const std::vector<uint32_t>* Mesh::getIndicies() const
{
	return &indicies;
}

std::string Mesh::getName() const
{
	return name;
}

void Mesh::setName(const std::string& name)
{
	this->name = name;
}

uint32_t Mesh::getVerticesCount() const
{
	uint32_t count = 0;
	for (const auto& flt1_stream : flt1_streams)
		count = std::max<uint32_t>(count, flt1_stream.second.size());
	for (const auto& flt2_stream : flt2_streams)
		count = std::max<uint32_t>(count, flt2_stream.second.size());
	for (const auto& flt3_stream : flt3_streams)
		count = std::max<uint32_t>(count, flt3_stream.second.size());
	for (const auto& flt4_stream : flt4_streams)
		count = std::max<uint32_t>(count, flt4_stream.second.size());
	return count;
}

uint32_t Mesh::getIndicesCount() const
{
	return indicies.size();
}

std::string Mesh::getParam(const std::string& param) const
{
	if (params.count(param) == 0)
		return "";
	return params.at(param);
}