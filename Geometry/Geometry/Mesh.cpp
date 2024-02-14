#include "pch.h"

#include <algorithm>
#include <stdlib.h>
#include "Mesh.h"
#include "Matrix4x4.h"
#include "ResourceManager.h"

uint32_t Mesh::instanceNumber = 0;

Mesh createCube()
{
	Mesh m;

	// vertices positions
	m.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("POSITION"),
			std::vector<flt3>())
	);

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
	m.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("NORMAL"), 
			std::vector<flt3>())
	);

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
	m.flt4_streams.push_back(
		std::pair<string_id, std::vector<flt4>>(
			StringManager::toStringId("COLOR"),
			std::vector<flt4>())
	);

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
	m.flt2_streams.push_back(
		std::pair<string_id, std::vector<flt2>>(
			StringManager::toStringId("TEXCOORD"), 
			std::vector<flt2>())
	);

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

	m.setTechnique(StringManager::toStringId("Light0Tex"));
	m.setPass(StringManager::toStringId("P0"));

	ParamKey diffuse_map_key{ StringManager::toStringId("gDiffuseMap"),-1,string_id(-1) };
	m.setParam(diffuse_map_key, StringManager::toStringId("test.dds"));

	ParamKey tex_transform_key{ StringManager::toStringId("gTexTransform"),-1,string_id(-1) };
	m.setParam(tex_transform_key, flt4x4());

	return m;
}

Mesh createTrees()
{
	srand(0);
	Mesh m;

	// vertices positions
	m.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("POSITION"),
			std::vector<flt3>())
	);
	auto& pts = m.flt3_streams.back().second;
	
	// billboard sizes
	m.flt2_streams.push_back(
		std::pair<string_id, std::vector<flt2>>(
			StringManager::toStringId("SIZE"),
			std::vector<flt2>())
	);
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

	m.setTechnique(StringManager::toStringId("Light3gTexAlphaClipFog"));
	m.setPass(StringManager::toStringId("P0"));

	ParamKey tree_map_array_key{ StringManager::toStringId("gTreeMapArray"), -1, string_id(-1) };
	m.setParam(tree_map_array_key, StringManager::toStringId("tree0.dds;tree1.dds;tree2.dds;tree3.dds;"));

	ParamKey fog_start_key{ StringManager::toStringId("gFogStart"), -1, string_id(-1) };
	m.setParam(fog_start_key, 5.0f);

	ParamKey fog_range_key{ StringManager::toStringId("gFogRange"), -1, string_id(-1) };
	m.setParam(fog_range_key, 20.0f);

	ParamKey fog_color_key{ StringManager::toStringId("gFogColor"), -1, string_id(-1) };
	m.setParam(fog_color_key, flt4(1, 0, 0, 1));

	return m;
}

Mesh createSelectionBoxes()
{
	srand(0);
	Mesh m;

	m.flt3_streams.reserve(5);

	// vertices positions
	m.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("POSITION"),
			std::vector<flt3>())
	);
	auto& pts = m.flt3_streams.back().second;

	// axis0
	m.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("AXIS"),
			std::vector<flt3>())
	);
	auto& axis0 = m.flt3_streams.back().second;

	// axis1
	m.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("AXIS1"),
			std::vector<flt3>())
	);
	auto& axis1 = m.flt3_streams.back().second;

	// axis2
	m.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("AXIS2"),
			std::vector<flt3>())
	);
	auto& axis2 = m.flt3_streams.back().second;

	// size
	m.flt1_streams.push_back(
		std::pair<string_id, std::vector<flt1>>(
			StringManager::toStringId("SIZE"), 
			std::vector<flt1>())
	);
	auto& size = m.flt1_streams.back().second;

	// color
	m.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("COLOR"), 
			std::vector<flt3>())
	);
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

	m.setTechnique(StringManager::toStringId("SelectedObjectsBox"));
	m.setPass(StringManager::toStringId("P0"));

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
	sphere.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("POSITION"), 
			std::vector<flt3>())
	);
	auto& pts = sphere.flt3_streams.back().second;
	pts.reserve(vertices);

	// vertices normals
	sphere.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("NORMAL"),
			std::vector<flt3>())
	);
	auto& nms = sphere.flt3_streams.back().second;
	nms.reserve(vertices);

	// vertices colors
	sphere.flt4_streams.push_back(
		std::pair<string_id, std::vector<flt4>>(
			StringManager::toStringId("COLOR"),
			std::vector<flt4>())
	);
	auto& clrs = sphere.flt4_streams.back().second;
	clrs.reserve(vertices);

	// uv-coordinates
	sphere.flt2_streams.push_back(
		std::pair<string_id, std::vector<flt2>>(
			StringManager::toStringId("TEXCOORD"),
			std::vector<flt2>())
	);
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

	sphere.setTechnique(StringManager::toStringId("Light0Tex"));
	sphere.setPass(StringManager::toStringId("P0"));

	ParamKey diffuse_map_key{ StringManager::toStringId("gDiffuseMap"),-1,string_id(-1) };
	sphere.setParam(diffuse_map_key, StringManager::toStringId("earth.jpg"));

	ParamKey tex_transform_key{ StringManager::toStringId("gTexTransform"),-1,string_id(-1) };
	sphere.setParam(tex_transform_key, flt4x4());

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
	cone.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("POSITION"),
			std::vector<flt3>())
	);
	auto& pts = cone.flt3_streams.back().second;
	pts.reserve(vertices);

	// vertices normals
	cone.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("NORMAL"),
			std::vector<flt3>())
	);
	auto& nms = cone.flt3_streams.back().second;
	nms.reserve(vertices);

	// vertices colors
	cone.flt4_streams.push_back(
		std::pair<string_id, std::vector<flt4>>(
			StringManager::toStringId("COLOR"),
			std::vector<flt4>())
	);
	auto& clrs = cone.flt4_streams.back().second;
	clrs.reserve(vertices);

	// uv-coordinates
	cone.flt2_streams.push_back(
		std::pair<string_id, std::vector<flt2>>(
			StringManager::toStringId("TEXCOORD"),
			std::vector<flt2>())
	);
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

	cone.setTechnique(StringManager::toStringId("Light0Tex"));
	cone.setPass(StringManager::toStringId("P0"));

	ParamKey diffuse_map_key{ StringManager::toStringId("gDiffuseMap"),-1,string_id(-1) };
	cone.setParam(diffuse_map_key, StringManager::toStringId("vase.jpg"));

	ParamKey tex_transform_key{ StringManager::toStringId("gTexTransform"),-1,string_id(-1) };
	cone.setParam(tex_transform_key, flt4x4());

	return cone;
}

Mesh createPlane(float width, float height, float m, float n)
{
	int vertices = (m + 1) * (n + 1);
	int triangles = 2 * m * n;
	int indices = 3 * triangles;

	Mesh plane;

	plane.flt3_streams.reserve(2);

	// vertices positions
	plane.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("POSITION"),
			std::vector<flt3>())
	);
	auto& pts = plane.flt3_streams.back().second;
	pts.reserve(vertices);

	// vertices normals
	plane.flt3_streams.push_back(
		std::pair<string_id, std::vector<flt3>>(
			StringManager::toStringId("NORMAL"),
			std::vector<flt3>())
	);
	auto& nms = plane.flt3_streams.back().second;
	nms.reserve(vertices);

	// vertices colors
	plane.flt4_streams.push_back(
		std::pair<string_id, std::vector<flt4>>(
			StringManager::toStringId("COLOR"),
			std::vector<flt4>())
	);
	auto& clrs = plane.flt4_streams.back().second;
	clrs.reserve(vertices);

	// uv-coordinates
	plane.flt2_streams.push_back(
		std::pair<string_id, std::vector<flt2>>(
			StringManager::toStringId("TEXCOORD"),
			std::vector<flt2>())
	);
	auto& uvs = plane.flt2_streams.back().second;
	uvs.reserve(vertices);

	auto& inds = plane.indicies;
	inds.reserve(indices);

	float du = 1.0f / m;
	float dv = 1.0f / n;

	float x0 = -width / 2;
	float z0 = -height / 2;

	float dx = width / m;
	float dz = height / n;

	for (int i = 0; i < m + 1; ++i)
	{
		for (int j = 0; j < n + 1; ++j)
		{
			pts.push_back(flt3(x0 + i * dx, 0, z0 + j * dz));
			nms.push_back(flt3(0, 1, 0));
			clrs.push_back(flt4(0, 0, 1, 0));
			uvs.push_back(flt2(i * du, j * dv));

			if (i < m && j < n)
			{
				inds.push_back(j * (m + 1) + i);
				inds.push_back(j * (m + 1) + i + 1);
				inds.push_back((j + 1) * (m + 1) + i + 1);

				inds.push_back(j * (m + 1) + i);
				inds.push_back((j + 1) * (m + 1) + i + 1);
				inds.push_back((j + 1) * (m + 1) + i);
			}
		}
	}

	plane.setTechnique(StringManager::toStringId("Light0Tex"));
	plane.setPass(StringManager::toStringId("P0"));

	ParamKey diffuse_map_key{ StringManager::toStringId("gDiffuseMap"),-1,string_id(-1) };
	plane.setParam(diffuse_map_key, StringManager::toStringId("bosh.jpg"));

	ParamKey tex_transform_key{ StringManager::toStringId("gTexTransform"),-1,string_id(-1) };
	plane.setParam(tex_transform_key, flt4x4());

	return plane;
}

Mesh createAreaOfSelection()
{
	Mesh m;

	m.flt4_streams.push_back(
		std::pair<string_id, std::vector<flt4>>(
			StringManager::toStringId("NDC"),
			std::vector<flt4>())
	);
	auto& ndcs = m.flt4_streams.back().second;

	ndcs.push_back(flt4(0, 0, 0, 1));
	ndcs.push_back(flt4(1, 0, 0, 1));
	ndcs.push_back(flt4(1, 1, 0, 1));
	ndcs.push_back(flt4(0, 1, 0, 1));

	auto& inds = m.indicies;

	inds.push_back(0);
	inds.push_back(2);
	inds.push_back(1);

	inds.push_back(0);
	inds.push_back(3);
	inds.push_back(2);

	m.setTechnique(StringManager::toStringId("AreaOfSelection"));
	m.setPass(StringManager::toStringId("P0"));

	ParamKey color_key{ StringManager::toStringId("color"),-1,string_id(-1) };
	m.setParam(color_key, flt4(0, 0, 1, 0.5));

	m.setBlendState(StringManager::toStringId("RenderTarget[0].BlendEnable=TRUE;RenderTarget[0].SrcBlend=D3D11_BLEND_INV_SRC_ALPHA;RenderTarget[0].DestBlend=D3D11_BLEND_SRC_ALPHA;RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;RenderTarget[0].RenderTargetWriteMask=15"));

	return m;
}

Mesh createMeshForTestingDefinitionOfTheSamePoints()
{
	Mesh m;

	m.flt2_streams.push_back(
		std::pair<string_id, std::vector<flt2>>(
			StringManager::toStringId("Position"),
			std::vector<flt2>())
	);
	auto& positions = m.flt2_streams.back().second;

	positions.push_back(flt2(-1, -1));
	positions.push_back(flt2(1, -1));
	positions.push_back(flt2(1, 1));
	positions.push_back(flt2(-1, 1));

	auto& inds = m.indicies;

	inds.push_back(0);
	inds.push_back(2);
	inds.push_back(1);

	inds.push_back(0);
	inds.push_back(3);
	inds.push_back(2);

	m.setTechnique(StringManager::toStringId("TestOfTheSamePointsDefinition"));
	m.setPass(StringManager::toStringId("P0"));

	ParamKey imageA_key{ StringManager::toStringId("imageA"),-1, string_id(-1) };
	m.setParam(imageA_key, StringManager::toStringId("imageA.jpg"));

	ParamKey imageB_key{ StringManager::toStringId("imageB"),-1, string_id(-1) };
	m.setParam(imageB_key, StringManager::toStringId("imageB.jpg"));

	return m;
}

Mesh createMeshVisualizingAreaOfIntegration()
{
	Mesh m;

	m.flt2_streams.push_back(
		std::pair<string_id, std::vector<flt2>>(
			StringManager::toStringId("Position"),
			std::vector<flt2>())
	);
	auto& positions = m.flt2_streams.back().second;

	int subdivDegree = 12;
	float dAngle = 2 * M_PI / subdivDegree;
	
	positions.resize(subdivDegree);
	for (int vi = 0; vi < subdivDegree; vi++)
	{
		float angle = vi * dAngle;
		positions[vi] = flt2(cos(angle), sin(angle));
	}

	auto& inds = m.indicies;
	inds.reserve(3 * (subdivDegree - 2));
	for (int vi = 2; vi < subdivDegree; vi++)
	{
		inds.push_back(0);
		inds.push_back(vi - 1);
		inds.push_back(vi);
	}

	m.setTechnique(StringManager::toStringId("VisualizeAreaOfIntegration"));
	m.setPass(StringManager::toStringId("P0"));

	ParamKey arrow_key{ StringManager::toStringId("arrow"),-1,string_id(-1) };
	m.setParam(arrow_key, StringManager::toStringId("Arrow.jpg"));

	m.setBlendState(StringManager::toStringId("RenderTarget[0].BlendEnable=TRUE;RenderTarget[0].SrcBlend=D3D11_BLEND_INV_SRC_ALPHA;RenderTarget[0].DestBlend=D3D11_BLEND_SRC_ALPHA;RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;RenderTarget[0].RenderTargetWriteMask=15"));

	return m;
}

Mesh createHistogram(float y[], int N)
{
	Mesh m;

	m.setTechnique(StringManager::toStringId("histogram"));
	m.setPass(StringManager::toStringId("P0"));

	m.flt2_streams.push_back(
		std::pair<string_id, std::vector<flt2>>(
			StringManager::toStringId("Position"),
			std::vector<flt2>())
	);
	auto& positions = m.flt2_streams.back().second;

	auto& inds = m.indicies;
	inds.reserve(6 * N);

	float h = 2.0f / N;
	for (int x = 0; x < N; x++)
	{
		positions.push_back(flt2(x * h - 1.0f, -1.0f));
		positions.push_back(flt2((x + 1) * h - 1.0f, -1.0f));

		positions.push_back(flt2(x * h - 1.0f, y[x] - 1.0f));
		positions.push_back(flt2((x + 1) * h - 1.0f, y[x] - 1.0f));

		int offset = 4 * x;

		inds.push_back(offset);
		inds.push_back(offset + 2);
		inds.push_back(offset + 1);

		inds.push_back(offset + 2);
		inds.push_back(offset + 3);
		inds.push_back(offset + 1);
	}

	return m;
}

void Mesh::setTechnique(string_id technique_name_id)
{
	this->technique_name_id = technique_name_id;

	params.clear();

	std::map<string_id, Float4Resource>& flt4s =
		ResourceManager::instance()->getFloat4s(technique_name_id);
	for (auto& f4 : flt4s)
	{
		ParamKey pk{ f4.first,-1,string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}

	std::map<string_id, Float3Resource>& flt3s =
		ResourceManager::instance()->getFloat3s(technique_name_id);
	for (auto& f3 : flt3s)
	{
		ParamKey pk{ f3.first,-1,string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}

	std::map<string_id, Float2Resource>& flt2s =
		ResourceManager::instance()->getFloat2s(technique_name_id);
	for (auto& f2 : flt2s)
	{
		ParamKey pk{ f2.first,-1,string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}

	std::map<string_id, Float1Resource>& flt1s =
		ResourceManager::instance()->getFloat1s(technique_name_id);
	for (auto& f1 : flt1s)
	{
		ParamKey pk{ f1.first,-1,string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}

	std::map<string_id, Float4x4Resource>& flt4x4s =
		ResourceManager::instance()->getFloat4x4s(technique_name_id);
	for (auto& f4x4 : flt4x4s)
	{
		ParamKey pk{ f4x4.first,-1,string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}

	std::map<string_id, Int2Resource>& i2s =
		ResourceManager::instance()->getInt2s(technique_name_id);
	for (auto& i2 : i2s)
	{
		ParamKey pk{ i2.first, -1, string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}

	std::map<string_id, Int1Resource>& i1s =
		ResourceManager::instance()->getInt1s(technique_name_id);
	for (auto& i1 : i1s)
	{
		ParamKey pk{ i1.first, -1, string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}

	std::map<string_id, StructResource>& structs =
		ResourceManager::instance()->getStructures(technique_name_id);
	for (auto& s : structs)
	{
		string_id var = s.first;
		const StructResource& sr = s.second;
		for (int e = 0; e < sr.elementsCount; ++e)
		{
			for (int f = 0; f < sr.fieldsCount; ++f)
			{
				auto& fr = sr.fields[f];

				ParamKey pk{ var,e,fr.name };
				ParamValue pv;
				params.push_back({ pk,pv });
			}
		}
	}

	std::map<string_id, Texture2dResource>& textures =
		ResourceManager::instance()->getTextures(technique_name_id);
	for (auto& t : textures)
	{
		ParamKey pk{ t.first,-1,string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}

	std::map<string_id, Texture2dArrayResource>& texArrs =
		ResourceManager::instance()->getTexturesArrays(technique_name_id);
	for (auto& ta : texArrs)
	{
		ParamKey pk{ ta.first,-1,string_id(-1) };
		ParamValue pv;
		params.push_back({ pk,pv });
	}
}

void Mesh::setPass(string_id pass_name_id)
{
	this->pass_name_id = pass_name_id;
}

void Mesh::setBlendState(string_id blend_state_id)
{
	this->blend_state_id = blend_state_id;
}

void Mesh::setDepthStencilState(string_id depth_stencil_state_id)
{
	this->depth_stencil_state_id = depth_stencil_state_id;
}

string_id Mesh::getTechnique() const
{
	return technique_name_id;
}

string_id Mesh::getPass() const
{
	return pass_name_id;
}

string_id Mesh::getBlendState() const
{
	return blend_state_id;
}

string_id Mesh::getDepthStencilState() const
{
	return depth_stencil_state_id;
}

const void* Mesh::getStream(string_id name, StreamType type) const
{
	const void* rPtr = nullptr;
	switch (type)
	{
	case FLT1:
	{
		const auto& it = std::find_if(flt1_streams.begin(), flt1_streams.end(), [&](const std::pair<string_id, std::vector<flt1>>& el) -> bool
		{
			return el.first == name;
		});

		if (it != flt1_streams.end())
			rPtr = &it->second;

		break;
	}
	case FLT2:
	{
		const auto& it = std::find_if(flt2_streams.begin(), flt2_streams.end(), [&](const std::pair<string_id, std::vector<flt2>>& el) -> bool
		{
			return el.first == name;
		});

		if (it != flt2_streams.end())
			rPtr = &it->second;

		break;
	}
	case FLT3:
	{
		const auto& it = std::find_if(flt3_streams.begin(), flt3_streams.end(), [&](const std::pair<string_id, std::vector<flt3>>& el) -> bool
		{
			return el.first == name;
		});

		if (it != flt3_streams.end())
			rPtr = &it->second;

		break;
	}
	case FLT4:
	{
		const auto& it = std::find_if(flt4_streams.begin(), flt4_streams.end(), [&](const std::pair<string_id, std::vector<flt4>>& el) -> bool
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

string_id* Mesh::getStringID(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.s;
}

float* Mesh::getFloat(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f;
}

flt2* Mesh::getFlt2(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f2;
}

flt3* Mesh::getFlt3(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f3;
}

flt4* Mesh::getFlt4(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f4;
}

flt4x4* Mesh::getFlt4x4(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.f4x4;
}

int* Mesh::getInt(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.i;
}

Vec2d<int>* Mesh::getInt2(int param_index)
{
	if (!params[param_index].second.valid)
		return nullptr;
	return &params[param_index].second.i2;
}

bool Mesh::getParam(int param_index, string_id& s) const
{
	if (!params[param_index].second.valid)
		return false;
	s = params[param_index].second.s;
	return true;
}

bool Mesh::getParam(int param_index, float& f) const
{
	if (!params[param_index].second.valid)
		return false;
	f = params[param_index].second.f;
	return true;
}

bool Mesh::getParam(int param_index, flt2& f2) const
{
	if (!params[param_index].second.valid)
		return false;
	f2 = params[param_index].second.f2;
	return true;
}

bool Mesh::getParam(int param_index, flt3& f3) const
{
	if (!params[param_index].second.valid)
		return false;
	f3 = params[param_index].second.f3;
	return true;
}

bool Mesh::getParam(int param_index, flt4& f4) const
{
	if (!params[param_index].second.valid)
		return false;
	f4 = params[param_index].second.f4;
	return true;
}

bool Mesh::getParam(int param_index, flt4x4& f4x4) const
{
	if (!params[param_index].second.valid)
		return false;
	f4x4 = params[param_index].second.f4x4;
	return true;
}

bool Mesh::getParam(int param_index, int& i) const
{
	if (!params[param_index].second.valid)
		return false;
	i = params[param_index].second.i;
	return true;
}

bool Mesh::getParam(int param_index, Vec2d<int>& i2) const
{
	if (!params[param_index].second.valid)
		return false;
	i2 = params[param_index].second.i2;
	return true;
}

bool Mesh::getParam(const ParamKey& param, string_id& s) const
{
	s = string_id(-1);

	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
		return false;

	s = it->second.s;
	
	return it->second.valid;
}

bool Mesh::getParam(const ParamKey& param, float& f) const
{
	f = 0;

	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
		return false;

	f = it->second.f;

	return it->second.valid;
}

bool Mesh::getParam(const ParamKey& param, flt2& f2) const
{
	f2 = flt2();

	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
		return false;

	f2 = it->second.f2;

	return it->second.valid;
}

bool Mesh::getParam(const ParamKey& param, flt3& f3) const
{
	f3 = flt3();

	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
		return false;

	f3 = it->second.f3;

	return it->second.valid;
}

bool Mesh::getParam(const ParamKey& param, flt4& f4) const
{
	f4 = flt4();

	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
		return false;

	f4 = it->second.f4;

	return it->second.valid;
}

bool Mesh::getParam(const ParamKey& param, flt4x4& f4x4) const
{
	f4x4 = flt4x4();

	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
		return false;

	f4x4 = it->second.f4x4;

	return it->second.valid;
}

bool Mesh::getParam(const ParamKey& param, int& i) const
{
	i = 0;

	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
		return false;

	i = it->second.i;

	return it->second.valid;
}

bool Mesh::getParam(const ParamKey& param, Vec2d<int>& i2) const
{
	i2 = Vec2d<int>();

	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
		return false;

	i2 = it->second.i2;

	return it->second.valid;
}

void Mesh::setParam(const ParamKey& param, const string_id& s)
{
	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
	{
		params.push_back({ param,ParamValue(s) });
		return;
	}
	it->second.s = s;
	it->second.valid = true;
}

void Mesh::setParam(const ParamKey& param, const float& f)
{
	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
	{
		params.push_back({ param,ParamValue(f) });
		return;
	}
	it->second.f = f;
	it->second.valid = true;
}

void Mesh::setParam(const ParamKey& param, const flt2& f2)
{
	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
	{
		params.push_back({ param,ParamValue(f2) });
		return;
	}
	it->second.f2 = f2;
	it->second.valid = true;
}

void Mesh::setParam(const ParamKey& param, const flt3& f3)
{
	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
	{
		params.push_back({ param,ParamValue(f3) });
		return;
	}
	it->second.f3 = f3;
	it->second.valid = true;
}

void Mesh::setParam(const ParamKey& param, const flt4& f4)
{
	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
	{
		params.push_back({ param,ParamValue(f4) });
		return;
	}
	it->second.f4 = f4;
	it->second.valid = true;
}

void Mesh::setParam(const ParamKey& param, const flt4x4& f4x4)
{
	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
	{
		params.push_back({ param,ParamValue(f4x4) });
		return;
	}
	it->second.f4x4 = f4x4;
	it->second.valid = true;
}

void Mesh::setParam(const ParamKey& param, const int& i)
{
	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
	{
		params.push_back({ param,ParamValue(i) });
		return;
	}
	it->second.i = i;
	it->second.valid = true;
}

void Mesh::setParam(const ParamKey& param, const Vec2d<int>& i2)
{
	auto it = std::find_if(params.begin(), params.end(), [&param](auto& p)
	{
		return p.first == param;
	});
	if (it == params.end())
	{
		params.push_back({ param,ParamValue(i2) });
		return;
	}
	it->second.i2 = i2;
	it->second.valid = true;
}

flt3 Mesh::getBottomBorder() const
{
	flt3 min_corner(FLT_MAX, FLT_MAX, FLT_MAX);

	const auto& it = std::find_if(
		flt3_streams.begin(),
		flt3_streams.end(),
		[&](auto& el)->bool
	{
		return el.first == position_id;
	}
	);
	if (it == flt3_streams.end())
		return min_corner;
	
	auto& flt3s = it->second;
	for (auto& pos : flt3s)
	{
		min_corner.x() = std::min<float>(min_corner.x(), const_cast<flt3&>(pos).x());
		min_corner.y() = std::min<float>(min_corner.y(), const_cast<flt3&>(pos).y());
		min_corner.z() = std::min<float>(min_corner.z(), const_cast<flt3&>(pos).z());
	}

	return min_corner;
}

flt3 Mesh::getTopBorder() const
{
	flt3 max_corner(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	const auto& it = std::find_if(
		flt3_streams.begin(),
		flt3_streams.end(),
		[&](auto& el)->bool
	{
		return el.first == position_id;
	}
	);
	if (it == flt3_streams.end())
		return max_corner;

	auto& flt3s = it->second;
	for (auto& pos : flt3s)
	{
		max_corner.x() = std::max<float>(max_corner.x(), const_cast<flt3&>(pos).x());
		max_corner.y() = std::max<float>(max_corner.y(), const_cast<flt3&>(pos).y());
		max_corner.z() = std::max<float>(max_corner.z(), const_cast<flt3&>(pos).z());
	}

	return max_corner;
}

void Mesh::getBoundingSphere(flt4& sphere, const flt4x4& transform) const
{
	sphere.x() = 0;
	sphere.y() = 0;
	sphere.z() = 0;
	sphere.w() = 0;

	const auto& it = std::find_if(
		flt3_streams.begin(),
		flt3_streams.end(),
		[&](auto& el)->bool
	{
		return el.first == position_id;
	}
	);
	if (it == flt3_streams.end())
		return;

	auto& flt3s = it->second;
	for (auto& v : flt3s)
	{
		flt4 v4(v, 1);
		sphere.xyz() += (v4 * const_cast<flt4x4&>(transform)).xyz();
	}
	sphere.xyz() /= flt3s.size();

	for (auto& v : flt3s)
	{
		flt4 v4(v, 1);
		float r = ((v4 * const_cast<flt4x4&>(transform)).xyz() - sphere.xyz()).length();
		if (r > sphere.w())
			sphere.w() = r;
	}
}

void Mesh::save(std::ofstream& s) const
{
	s << "mesh name: ";

	if (name.length() == 0)
		s << "0" << std::endl;
	else
		s << "1 " << name << std::endl;
	
	s << "technique: "; 
	StringManager::save(s, technique_name_id);
	s << std::endl;
	
	s << "pass: ";
	StringManager::save(s, pass_name_id);
	s << std::endl;

	s << "blend state: ";
	StringManager::save(s, blend_state_id);
	s << std::endl;

	s << "depth/stencil state: ";
	StringManager::save(s, depth_stencil_state_id);
	s << std::endl;

	s << "float streams:" << std::endl;
	s << "streams count: " << flt1_streams.size() << std::endl;
	for (auto& stream : flt1_streams)
	{
		s << "stream name: ";
		StringManager::save(s, stream.first);
		s << std::endl;

		s << "elements count: " << stream.second.size() << std::endl;
		s << "elements: " << std::endl;
		for (auto& f : stream.second)
		{
			s << std::string(f) << std::endl;
		}
	}
	s << "float2 streams:" << std::endl;
	s << "streams count: " << flt2_streams.size() << std::endl;
	for (auto& stream : flt2_streams)
	{
		s << "stream name: ";
		StringManager::save(s, stream.first);
		s << std::endl;

		s << "elements count: " << stream.second.size() << std::endl;
		s << "elements: " << std::endl;
		for (auto& f2 : stream.second)
		{
			s << std::string(f2) << std::endl;
		}
	}
	s << "float3 streams:" << std::endl;
	s << "streams count: " << flt3_streams.size() << std::endl;
	for (auto& stream : flt3_streams)
	{
		s << "stream name: ";
		StringManager::save(s, stream.first);
		s << std::endl;

		s << "elements count: " << stream.second.size() << std::endl;
		s << "elements: " << std::endl;
		for (auto& f3 : stream.second)
		{
			s << std::string(f3) << std::endl;
		}
	}
	s << "float4 streams:" << std::endl;
	s << "streams count: " << flt4_streams.size() << std::endl;
	for (auto& stream : flt4_streams)
	{
		s << "stream name: ";
		StringManager::save(s, stream.first);
		s << std::endl;

		s << "elements count: " << stream.second.size() << std::endl;
		s << "elements: " << std::endl;
		for (auto& f4 : stream.second)
		{
			s << std::string(f4) << std::endl;
		}
	}
	s << "indicies: " << std::endl;
	s << "indicies count: " << indicies.size() << std::endl;
	for (auto& index : indicies)
	{
		s << index << std::endl;
	}
	s << "params: " << std::endl;
	s << "params count: " << params.size() << std::endl;
	for (const auto& param : params)
	{
		s << "param key: " << std::endl;
		
		s << "name: ";
		StringManager::save(s, param.first.name);
		s << std::endl;

		s << "index: " << param.first.index << std::endl;
		
		s << "field: ";
		StringManager::save(s, param.first.field);
		s << std::endl;

		s << "param value: " << std::endl;
		
		s << "string: ";
		StringManager::save(s, param.second.s);
		s << std::endl;

		s << "float: " << param.second.f << std::endl;
		s << "float2: " << std::string(param.second.f2) << std::endl;
		s << "float3: " << std::string(param.second.f3) << std::endl;
		s << "float4: " << std::string(param.second.f4) << std::endl;
		s << "float4x4: " << std::string(param.second.f4x4) << std::endl;
		s << "int: " << param.second.i << std::endl;
		s << "int2: " << std::string(param.second.i2) << std::endl;
		s << "validity: " << param.second.valid << std::endl;
	}
}

void Mesh::load(std::ifstream& s) 
{
	std::string tmp;

	int validity = 0;
	s >> tmp >> tmp >> validity;
	if (validity == 1)
		s >> name;
	
	s >> tmp;
	technique_name_id = StringManager::load(s);
	
	s >> tmp;
	pass_name_id = StringManager::load(s);

	s >> tmp >> tmp;
	blend_state_id = StringManager::load(s);

	s >> tmp >> tmp;
	depth_stencil_state_id = StringManager::load(s);

	// float streams

	s >> tmp >> tmp;

	int streamsCount = 0;
	s >> tmp >> tmp >> streamsCount;

	flt1_streams.clear();
	flt1_streams.resize(streamsCount);

	for (int si = 0; si < streamsCount; si++)
	{
		std::string streamName;
		s >> tmp >> tmp;

		flt1_streams[si].first = StringManager::load(s);

		int elementsCount;
		s >> tmp >> tmp >> elementsCount;

		auto& stream = flt1_streams[si].second;
		stream.reserve(elementsCount);

		s >> tmp;

		for (int ei = 0; ei < elementsCount; ei++)
		{
			s >> tmp;
			stream.push_back(flt1(tmp));
		}
	}

	// float2 streams

	s >> tmp >> tmp;

	streamsCount = 0;
	s >> tmp >> tmp >> streamsCount;

	flt2_streams.clear();
	flt2_streams.resize(streamsCount);

	for (int si = 0; si < streamsCount; si++)
	{
		std::string streamName;
		s >> tmp >> tmp;

		flt2_streams[si].first = StringManager::load(s);

		int elementsCount;
		s >> tmp >> tmp >> elementsCount;

		auto& stream = flt2_streams[si].second;
		stream.reserve(elementsCount);

		s >> tmp;

		for (int ei = 0; ei < elementsCount; ei++)
		{
			s >> tmp;
			stream.push_back(flt2(tmp));
		}
	}

	// float3 streams

	s >> tmp >> tmp;

	streamsCount = 0;
	s >> tmp >> tmp >> streamsCount;

	flt3_streams.clear();
	flt3_streams.resize(streamsCount);

	for (int si = 0; si < streamsCount; si++)
	{
		std::string streamName;
		s >> tmp >> tmp;

		flt3_streams[si].first = StringManager::load(s);

		int elementsCount;
		s >> tmp >> tmp >> elementsCount;

		auto& stream = flt3_streams[si].second;
		stream.reserve(elementsCount);

		s >> tmp;

		for (int ei = 0; ei < elementsCount; ei++)
		{
			s >> tmp;
			stream.push_back(flt3(tmp));
		}
	}

	// float4 streams

	s >> tmp >> tmp;

	streamsCount = 0;
	s >> tmp >> tmp >> streamsCount;

	flt4_streams.clear();
	flt4_streams.resize(streamsCount);

	for (int si = 0; si < streamsCount; si++)
	{
		std::string streamName;
		s >> tmp >> tmp;

		flt4_streams[si].first = StringManager::load(s);

		int elementsCount;
		s >> tmp >> tmp >> elementsCount;

		auto& stream = flt4_streams[si].second;
		stream.reserve(elementsCount);

		s >> tmp;

		for (int ei = 0; ei < elementsCount; ei++)
		{
			s >> tmp;
			stream.push_back(flt4(tmp));
		}
	}

	// indices
	s >> tmp;

	int indicesCount;
	s >> tmp >> tmp >> indicesCount;

	indicies.resize(indicesCount);
	for (int ii = 0; ii < indicesCount; ii++)
	{
		s >> indicies[ii];
	}

	// params
	s >> tmp;

	int paramsCount;
	s >> tmp >> tmp >> paramsCount;

	params.resize(paramsCount);
	for (int pi = 0; pi < paramsCount; pi++)
	{
		// key
		s >> tmp >> tmp;
		
		s >> tmp;
		params[pi].first.name = StringManager::load(s);
		
		s >> tmp >> params[pi].first.index;

		s >> tmp;
		params[pi].first.field = StringManager::load(s);

		// value
		s >> tmp >> tmp;

		s >> tmp;
		params[pi].second.s = StringManager::load(s);

		s >> tmp >> params[pi].second.f;

		s >> tmp >> tmp;
		params[pi].second.f2 = tmp;

		s >> tmp >> tmp;
		params[pi].second.f3 = tmp;

		s >> tmp >> tmp;
		params[pi].second.f4 = tmp;

		s >> tmp >> tmp;
		params[pi].second.f4x4 = tmp;

		s >> tmp >> params[pi].second.i;

		s >> tmp >> tmp;
		params[pi].second.i2 = tmp;

		s >> tmp >> params[pi].second.valid;
	}
}
