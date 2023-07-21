#include "GraphicsCore.h"
#include "Cameras.h"
#include "Timer.h"
#include "Mesh.h"
#include "Scene.h"
#include "DrawVisitor.h"
#include "Selector.h"
#include "MainScene.h"
#include <Windows.h>
#include <windef.h>

Mesh cube;
Mesh trees;
Mesh selectionBoxes;
Scene scene;
DrawVisitor drawVisitor;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static float mousePosX0 = 0;
	static float mousePosY0 = 0;

	static float mousePosX1 = 0;
	static float mousePosY1 = 0;

	static bool bMarginSelection = false;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		bMarginSelection = true;

		mousePosX0 = LOWORD(lparam);
		mousePosY0 = HIWORD(lparam);

		RECT rect;
		GetClientRect(hwnd, &rect);

		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		mousePosX0 /= width;
		mousePosY0 /= height;

		mousePosX0 = 2 * (mousePosX0 - 0.5);
		mousePosY0 = 2 * (0.5 - mousePosY0);

		mousePosX1 = mousePosX0;
		mousePosY1 = mousePosY0;

		Selector::instance()->selectObjects(
			mousePosX0, mousePosY0,
			mousePosX1, mousePosY1
		);

		return 0;
	}
/*	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	{
		if (!bMarginSelection)
			return 0;

		mousePosX1 = LOWORD(lparam);
		mousePosY1 = HIWORD(lparam);

		RECT rect;
		GetClientRect(hwnd, &rect);

		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		mousePosX1 /= width;
		mousePosY1 /= height;

		mousePosX1 = 2 * (mousePosX1 - 0.5);
		mousePosY1 = 2 * (0.5 - mousePosY1);

		Selector::instance()->selectObjects(
			mousePosX0, mousePosY0,
			mousePosX1, mousePosY1
		);
		
		bMarginSelection = false;

		return 0;
	}*/
	case WM_SIZE:
	{
		UINT width = LOWORD(lparam);
		UINT height = HIWORD(lparam);

		GraphicsCore::instance()->resize(width, height);

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
	MainScene::instance()->accept(&drawVisitor);
	Selector::instance()->draw();
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, drawFunc, 640, 480, true, false);

	cube = createCube();
	MainScene::instance()->addMeshNode(&cube);

	return GraphicsCore::instance()->run();
}