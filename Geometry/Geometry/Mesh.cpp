#include "pch.h"

#include <algorithm>
#include <stdlib.h>
#include "Mesh.h"
#include "Matrix4x4.h"

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

	// uv-coordinates
	m.flt2_streams.push_back(std::pair<std::string, std::vector<flt2>>(std::string("TEXCOORD"), std::vector<flt2>()));

	auto& uvs = m.flt2_streams.back().second;

	// bottom
	uvs.push_back(flt2(1.0f / 4.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(1.0f / 2.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(1.0f / 2.0f, 2.0f / 3.0f));
	uvs.push_back(flt2(1.0f / 4.0f, 2.0f / 3.0f));

	// top
	uvs.push_back(flt2(1.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(3.0f / 4.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(3.0f / 4.0f, 2.0f / 3.0f));
	uvs.push_back(flt2(1.0f, 2.0f / 3.0f));

	// left
	uvs.push_back(flt2(1.0f / 4.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(0.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(0.0f, 2.0f / 3.0f));
	uvs.push_back(flt2(1.0f / 4.0f, 2.0f / 3.0f));

	// right
	uvs.push_back(flt2(1.0f / 2.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(3.0f / 4.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(3.0f / 4.0f, 2.0f / 3.0f));
	uvs.push_back(flt2(1.0f / 2.0f, 2.0f / 3.0f));

	// back
	uvs.push_back(flt2(1.0f / 4.0f, 1.0f / 3.0f));
	uvs.push_back(flt2(1.0f / 4.0f, 0.0f));
	uvs.push_back(flt2(1.0f / 2.0f, 0.0f));
	uvs.push_back(flt2(1.0f / 2.0f, 1.0f / 3.0f));

	// front
	uvs.push_back(flt2(1.0f / 4.0f, 2.0f / 3.0f));
	uvs.push_back(flt2(1.0f / 4.0f, 1.0f));
	uvs.push_back(flt2(1.0f / 2.0f, 1.0f));
	uvs.push_back(flt2(1.0f / 2.0f, 2.0f / 3.0f));

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

	m.params["technique"] = "Light0Tex";
	m.params["pass"] = "P0";
	m.params["gDiffuseMap"] = "test.dds";

	return m;
}

Mesh createTrees()
{
	srand(0);
	Mesh m;

	// vertices positions
	m.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("POSITION"), std::vector<flt3>()));
	auto& pts = m.flt3_streams.back().second;
	
	// billboard sizes
	m.flt2_streams.push_back(std::pair<std::string, std::vector<flt2>>(std::string("SIZE"), std::vector<flt2>()));
	auto& szs = m.flt2_streams.back().second;

	int n = 2;
	float side = 20;
	float width = 7;
	float height = 7;
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			float x = -0.5f * side + i * side / n;
			float y = 0.0f;
			float z = -0.5f * side + j * side / n;

			pts.push_back(flt3(x, y, z));

			float w = width * (0.5f + (float)rand() / RAND_MAX / 2);
			float h = height * (0.5f + (float)rand() / RAND_MAX / 2);

			szs.push_back(flt2(w, h));
		}
	}

	m.params["technique"] = "Light3gTexAlphaClipFog";
	m.params["pass"] = "P0";
	m.params["gTreeMapArray"] = "tree0.dds;tree1.dds;tree2.dds;tree3.dds;";
	m.params["gFogStart"] = "{5}";
	m.params["gFogRange"] = "{20}";
	m.params["gFogColor"] = "{1,0,0,1}";

	return m;
}

Mesh createSelectionBoxes()
{
	srand(0);
	Mesh m;

	m.flt3_streams.reserve(5);

	// vertices positions
	m.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("POSITION"), std::vector<flt3>()));
	auto& pts = m.flt3_streams.back().second;

	// axis0
	m.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("AXIS"), std::vector<flt3>()));
	auto& axis0 = m.flt3_streams.back().second;

	// axis1
	m.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("AXIS1"), std::vector<flt3>()));
	auto& axis1 = m.flt3_streams.back().second;

	// axis2
	m.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("AXIS2"), std::vector<flt3>()));
	auto& axis2 = m.flt3_streams.back().second;

	// size
	m.flt1_streams.push_back(std::pair<std::string, std::vector<flt1>>(std::string("SIZE"), std::vector<flt1>()));
	auto& size = m.flt1_streams.back().second;

	// color
	m.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("COLOR"), std::vector<flt3>()));
	auto& color = m.flt3_streams.back().second;

	int n = 2;
	float sz = 10;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			flt3 pos(3 * sz * i, 0, 3 * sz * j);

			// fill positions
			pts.push_back(pos);

			float angleX = 180.0 * std::rand() / RAND_MAX - 90.0;
			flt4x4 rotX = makeRotate(flt3(1, 0, 0), angleX);

			float angleY = 360.0 * std::rand() / RAND_MAX;
			flt4x4 rotY = makeRotate(flt3(0, 1, 0), angleY);

			flt4x4 rot = rotX * rotY;

			flt3 a0 = (0.5f + 0.5f * rand() / RAND_MAX) * sz * flt3(1, 0, 0);
			flt3 a1 = (0.5f + 0.5f * rand() / RAND_MAX) * sz * flt3(0, 1, 0);
			flt3 a2 = (0.5f + 0.5f * rand() / RAND_MAX) * sz * flt3(0, 0, 1);

			a0 = a0 * rot;
			a1 = a1 * rot;
			a2 = a2 * rot;

			// fill axises
			axis0.push_back(a0);
			axis1.push_back(a1);
			axis2.push_back(a2);

			// fill sizes
			size.push_back(0.5);

			// fill colors
			color.push_back(flt3(1, 0, 0));
		}
	}

	m.params["technique"] = "SelectedObjectsBox";
	m.params["pass"] = "P0";

	return m;
}

Mesh createSphere(int latitudes, int longitudes)
{
	int vertices = latitudes * (longitudes + 1) - 2;
	int triangles = 2 * (latitudes - 2) * longitudes;
	int indices = 3 * triangles;

	Mesh sphere;

	sphere.flt3_streams.reserve(2);

	// vertices positions
	sphere.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("POSITION"), std::vector<flt3>()));
	auto& pts = sphere.flt3_streams.back().second;
	pts.reserve(vertices);

	// vertices normals
	sphere.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("NORMAL"), std::vector<flt3>()));
	auto& nms = sphere.flt3_streams.back().second;
	nms.reserve(vertices);

	// vertices colors
	sphere.flt4_streams.push_back(std::pair<std::string, std::vector<flt4>>(std::string("COLOR"), std::vector<flt4>()));
	auto& clrs = sphere.flt4_streams.back().second;
	clrs.reserve(vertices);

	// uv-coordinates
	sphere.flt2_streams.push_back(std::pair<std::string, std::vector<flt2>>(std::string("TEXCOORD"), std::vector<flt2>()));
	auto& uvs = sphere.flt2_streams.back().second;
	uvs.reserve(vertices);

	auto& inds = sphere.indicies;
	inds.reserve(indices);

	float radius = 1;

	double dh = 2 * M_PI / longitudes;
	double dv = M_PI / (latitudes - 1);

	for (int hi = 0; hi < longitudes; ++hi)
	{
		flt3 n(0, 1, 0);
		pts.push_back(radius * n);
		nms.push_back(n);
		flt4 c(0, 1, 0, 0);
		clrs.push_back(c);
		uvs.push_back(flt2(hi * dh / 2 / M_PI, 0));
	}
	for (int hi = 0; hi < longitudes + 1; ++hi)
	{
		flt3 n(sin(dv) * cos(hi * dh), cos(dv), sin(dv) * sin(hi * dh));
		pts.push_back(radius * n);
		nms.push_back(n);
		flt4 c(0, 1, 0, 0);
		clrs.push_back(c);
		uvs.push_back(flt2(hi * dh / 2 / M_PI, dv / M_PI));

		if (hi < longitudes)
		{
			inds.push_back(hi);
			inds.push_back(longitudes + hi);
			inds.push_back(longitudes + hi + 1);
		}
	}
	for (int vi = 2; vi < latitudes - 1; ++vi)
	{
		for (int hi = 0; hi < longitudes + 1; ++hi)
		{
			flt3 n(sin(vi * dv) * cos(hi * dh), cos(vi * dv), sin(vi * dv) * sin(hi * dh));
			pts.push_back(radius * n);
			nms.push_back(n);
			flt4 c(0, 1, 0, 0);
			clrs.push_back(c);
			uvs.push_back(flt2(hi * dh / 2 / M_PI, vi * dv / M_PI));

			if (hi < longitudes)
			{
				inds.push_back(longitudes + (longitudes + 1) * (vi - 2) + hi);
				inds.push_back(longitudes + (longitudes + 1) * (vi - 1) + hi);
				inds.push_back(longitudes + (longitudes + 1) * (vi - 1) + hi + 1);

				inds.push_back(longitudes + (longitudes + 1) * (vi - 2) + hi);
				inds.push_back(longitudes + (longitudes + 1) * (vi - 1) + hi + 1);
				inds.push_back(longitudes + (longitudes + 1) * (vi - 2) + hi + 1);
			}
		}
	}
	for (int hi = 0; hi < longitudes; ++hi)
	{
		flt3 n(0, -1, 0);
		pts.push_back(radius * n);
		nms.push_back(n);
		flt4 c(0, 1, 0, 0);
		clrs.push_back(c);
		uvs.push_back(flt2(hi * dh / 2 / M_PI, 1));

		inds.push_back(longitudes + (longitudes + 1) * (latitudes - 3) + hi);
		inds.push_back(longitudes + (longitudes + 1) * (latitudes - 2) + hi);
		inds.push_back(longitudes + (longitudes + 1) * (latitudes - 3) + hi + 1);
	}

	sphere.params["technique"] = "Light0Tex";
	sphere.params["pass"] = "P0";
	sphere.params["gDiffuseMap"] = "earth.jpg";

	return sphere;
}

Mesh createCone(float topRadius, float bottomRadius, float height, int edgesNumbers)
{
	int vertices = 2 * (2 * edgesNumbers + 1);
	int triangles = 4 * edgesNumbers;
	int indices = 3 * triangles;

	Mesh cone;

	cone.flt3_streams.reserve(2);

	// vertices positions
	cone.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("POSITION"), std::vector<flt3>()));
	auto& pts = cone.flt3_streams.back().second;
	pts.reserve(vertices);

	// vertices normals
	cone.flt3_streams.push_back(std::pair<std::string, std::vector<flt3>>(std::string("NORMAL"), std::vector<flt3>()));
	auto& nms = cone.flt3_streams.back().second;
	nms.reserve(vertices);

	// vertices colors
	cone.flt4_streams.push_back(std::pair<std::string, std::vector<flt4>>(std::string("COLOR"), std::vector<flt4>()));
	auto& clrs = cone.flt4_streams.back().second;
	clrs.reserve(vertices);

	// uv-coordinates
	cone.flt2_streams.push_back(std::pair<std::string, std::vector<flt2>>(std::string("TEXCOORD"), std::vector<flt2>()));
	auto& uvs = cone.flt2_streams.back().second;
	uvs.reserve(vertices);

	auto& inds = cone.indicies;
	inds.reserve(indices);

	float dv = 2 * M_PI / edgesNumbers;

	for (int vi = 0; vi < edgesNumbers; ++vi)
	{
		pts.push_back(flt3(0, height / 2, 0));
		nms.push_back(flt3(0, 1, 0));
		clrs.push_back(flt4(0, 0, 1, 0));
		uvs.push_back(flt2((float)vi / (float)edgesNumbers, 0));
	}

	float v = 0.5 - std::atan(height / 2 / topRadius) / M_PI;
	
	float side = sqrt(pow(height, 2) + pow(topRadius - bottomRadius, 2));
	
	float nX = height / side;
	float nY = abs(topRadius - bottomRadius) / side + 1;

	side = sqrt(nX * nX + nY * nY);
	nX /= side;
	nY /= side;

	for (int vi = 0; vi < edgesNumbers + 1; ++vi)
	{
		pts.push_back(flt3(topRadius * cos(vi * dv), height / 2, topRadius * sin(vi * dv)));
		nms.push_back(flt3(nX * cos(vi * dv), nY, nX * sin(vi * dv)));
		clrs.push_back(flt4(0, 0, 1, 0));
		uvs.push_back(flt2((float)vi / (float)edgesNumbers, v));

		if (vi < edgesNumbers)
		{
			inds.push_back(vi);
			inds.push_back(edgesNumbers + vi);
			inds.push_back(edgesNumbers + vi + 1);
		}
	}

	v = 0.5 - std::atan(- height / 2 / bottomRadius) / M_PI;

	side = sqrt(pow(height, 2) + pow(topRadius - bottomRadius, 2));

	nX = height / side;
	nY = abs(topRadius - bottomRadius) / side - 1;

	side = sqrt(nX * nX + nY * nY);
	nX /= side;
	nY /= side;

	for (int vi = 0; vi < edgesNumbers + 1; ++vi)
	{
		pts.push_back(flt3(bottomRadius * cos(vi * dv), - height / 2, bottomRadius * sin(vi * dv)));
		nms.push_back(flt3(nX * cos(vi * dv), nY, nX * sin(vi * dv)));
		clrs.push_back(flt4(0, 0, 1, 0));
		uvs.push_back(flt2((float)vi / (float)edgesNumbers, v));

		if (vi < edgesNumbers)
		{
			inds.push_back(2 * edgesNumbers + 1 + vi);
			inds.push_back(edgesNumbers + vi + 1);
			inds.push_back(edgesNumbers + vi);

			inds.push_back(2 * edgesNumbers + 1 + vi);
			inds.push_back(2 * edgesNumbers + 2 + vi);
			inds.push_back(edgesNumbers + vi + 1);
		}
	}

	for (int vi = 0; vi < edgesNumbers; ++vi)
	{
		pts.push_back(flt3(0, -height / 2, 0));
		nms.push_back(flt3(0, -1, 0));
		clrs.push_back(flt4(0, 0, 1, 0));
		uvs.push_back(flt2((float)vi / (float)edgesNumbers, 1));

		inds.push_back(3 * edgesNumbers + 2 + vi);
		inds.push_back(2 * edgesNumbers + 2 + vi);
		inds.push_back(2 * edgesNumbers + 1 + vi);
	}

	cone.params["technique"] = "Light0Tex";
	cone.params["pass"] = "P0";
	cone.params["gDiffuseMap"] = "vase.jpg";

	return cone;
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
	if (gpuReadyData)
		return verticesCount;

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

void Mesh::setParam(const std::string& param, const std::string& value)
{
	params[param] = value;
}

flt3 Mesh::getBottomBorder() const
{
	flt3 min(FLT_MAX, FLT_MAX, FLT_MAX);

	const auto& it = std::find_if(
		flt3_streams.begin(),
		flt3_streams.end(),
		[&](auto& el)->bool
	{
		return el.first == std::string("POSITION");
	}
	);
	if (it == flt3_streams.end())
		return min;
	
	auto& flt3s = it->second;
	for (auto& pos : flt3s)
	{
		min.x() = std::min(min.x(), const_cast<flt3&>(pos).x());
		min.y() = std::min(min.y(), const_cast<flt3&>(pos).y());
		min.z() = std::min(min.z(), const_cast<flt3&>(pos).z());
	}

	return min;
}

flt3 Mesh::getTopBorder() const
{
	flt3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	const auto& it = std::find_if(
		flt3_streams.begin(),
		flt3_streams.end(),
		[&](auto& el)->bool
	{
		return el.first == std::string("POSITION");
	}
	);
	if (it == flt3_streams.end())
		return max;

	auto& flt3s = it->second;
	for (auto& pos : flt3s)
	{
		max.x() = std::max(max.x(), const_cast<flt3&>(pos).x());
		max.y() = std::max(max.y(), const_cast<flt3&>(pos).y());
		max.z() = std::max(max.z(), const_cast<flt3&>(pos).z());
	}

	return max;
}