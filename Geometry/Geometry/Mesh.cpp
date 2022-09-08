#include "pch.h"
#include "Mesh.h"

Mesh&& createCube()
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

	return std::move(m);
}