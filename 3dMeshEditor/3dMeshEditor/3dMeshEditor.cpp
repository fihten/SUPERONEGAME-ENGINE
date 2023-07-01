#include "GraphicsCore.h"
#include "Cameras.h"
#include "Timer.h"
#include "Mesh.h"
#include "Scene.h"
#include "DrawVisitor.h"
#include <Windows.h>

GraphicsCore graphicsCore;
Mesh cube;
Mesh trees;
Mesh selectionBoxes;
Scene scene;
DrawVisitor drawVisitor;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lparam);
		UINT height = HIWORD(lparam);

		graphicsCore.resize(width, height);

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	float dt = Timer::Instance()->elapsedTime();
	for (int i = 0; i < CAMERAS_NUMBER; ++i)
		cameras()[i].processMessage(msg,wparam,lparam, dt);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void drawFunc(GraphicsCore* graphicsCore)
{
	graphicsCore->startFrame();
	scene.accept(&drawVisitor);
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	selectionBoxes = createSelectionBoxes();

	NodeID mesh = scene.addMeshNode(&selectionBoxes);
	drawVisitor.graphicsCore = &graphicsCore;

	graphicsCore.init(hInstance, iCmdShow, WndProc, drawFunc, 640, 480, true, false);

	return graphicsCore.run();
}