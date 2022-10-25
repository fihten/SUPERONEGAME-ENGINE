#include "pch.h"

#include <algorithm>
#include "Mesh.h"

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

	// vertices colors
	m.flt4_streams.push_back(std::pair<std::string, std::vector<flt4>>(std::string("COLOR"), std::vector<flt4>()));

	auto& clrs = m.flt4_streams.back().second;
	clrs.push_back(flt4(0, 0, 0, 1));
	clrs.push_back(flt4(0, 0, 1, 1));
	clrs.push_back(flt4(0, 1, 0, 1));
	clrs.push_back(flt4(0, 1, 1, 1));
	clrs.push_back(flt4(1, 0, 0, 1));
	clrs.push_back(flt4(1, 0, 1, 1));
	clrs.push_back(flt4(1, 1, 0, 1));
	clrs.push_back(flt4(1, 1, 1, 1));


	// indices
	auto& inds = m.indicies;
	
	// bottom
	inds.push_back(0);
	inds.push_back(1);
	inds.push_back(2);
	
	inds.push_back(0);
	inds.push_back(2);
	inds.push_back(3);

	// top
	inds.push_back(6);
	inds.push_back(5);
	inds.push_back(4);

	inds.push_back(6);
	inds.push_back(4);
	inds.push_back(7);

	// front 
	inds.push_back(2);
	inds.push_back(6);
	inds.push_back(7);

	inds.push_back(2);
	inds.push_back(7);
	inds.push_back(3);

	// back
	inds.push_back(4);
	inds.push_back(5);
	inds.push_back(1);

	inds.push_back(4);
	inds.push_back(1);
	inds.push_back(0);

	// right
	inds.push_back(5);
	inds.push_back(6);
	inds.push_back(1);

	inds.push_back(6);
	inds.push_back(2);
	inds.push_back(1);

	// left
	inds.push_back(7);
	inds.push_back(4);
	inds.push_back(0);

	inds.push_back(7);
	inds.push_back(0);
	inds.push_back(3);

	m.params["technique"] = "Demo";
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