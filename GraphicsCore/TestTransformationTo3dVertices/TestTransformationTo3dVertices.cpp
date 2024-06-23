#include "GraphicsCore.h"
#include "StringManager.h"
#include "Mesh.h"
#include "ModelMaker.h"
#include "GenerateVerticesAndMapping.h"
#include <algorithm>

Mesh meshOfVertices;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

void drawFunc(GraphicsCore* graphicsCore)
{
	graphicsCore->startFrame();
	graphicsCore->draw(meshOfVertices);
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	StringManager::init();
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, nullptr, drawFunc, 640, 480, true, false);

	TransformTo3dVertices transformator;
	transformator.init();

	std::vector<flt2> points;
	std::vector<Vec2d<uint32_t>> mapping;
	generateVerticesAndMapping(points, mapping);

	int amountOfVertices = 0;
	for (int i = 0; i < points.size(); i++)
	{
		amountOfVertices = std::max<int>(amountOfVertices, mapping[i].x());
	}

	std::vector<flt3> vertices(amountOfVertices);
	transformator.transform(points.data(), mapping.data(), points.size(), vertices.data());

	return GraphicsCore::instance()->run();
}