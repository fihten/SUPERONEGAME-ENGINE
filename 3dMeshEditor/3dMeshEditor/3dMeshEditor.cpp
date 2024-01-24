#include "GraphicsCore.h"
#include "Cameras.h"
#include "Timer.h"
#include "Mesh.h"
#include "Scene.h"
#include "DrawVisitor.h"
#include "Selector.h"
#include "MainScene.h"
#include "SelectionObjectsTesting.h"
#include "FrameOfReferenceState.h"
#include "Transition.h"
#include <Windows.h>
#include <windef.h>

Transition transitionModifier;
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
		if (!Selector::instance()->turnOnMultipleSelection())
			break;
		bMarginSelection = true;

		mousePosX0 = LOWORD(lparam) + 0.5f;
		mousePosY0 = HIWORD(lparam) + 0.5f;

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

		break;
	}
	case WM_MOUSEMOVE:
	{
		float mousePosX = LOWORD(lparam) + 0.5f;
		float mousePosY = HIWORD(lparam) + 0.5f;

		RECT rect;
		GetClientRect(hwnd, &rect);

		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		mousePosX /= width;
		mousePosY /= height;

		mousePosX = 2 * (mousePosX - 0.5);
		mousePosY = 2 * (0.5 - mousePosY);

		Selector::instance()->selectObject(mousePosX, mousePosY);
	}
	case WM_LBUTTONUP:
	{
		if (!bMarginSelection)
			break;

		mousePosX1 = LOWORD(lparam) + 0.5f;
		mousePosY1 = HIWORD(lparam) + 0.5f;

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
		
		if (msg == WM_LBUTTONUP)
		{
			bMarginSelection = false;
			Selector::instance()->turnOffMultipleSelection();
		}

		break;
	}
	case WM_SIZE:
	{
		UINT width = LOWORD(lparam);
		UINT height = HIWORD(lparam);

		GraphicsCore::instance()->resize(width, height);

		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	}

	float dt = Timer::Instance()->elapsedTime();
	for (int i = 0; i < CAMERAS_NUMBER; ++i)
		cameras()[i].processMessage(msg,wparam,lparam, dt);

	transitionModifier.processWindowMessage(msg, wparam, lparam);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void drawFunc(GraphicsCore* graphicsCore)
{
	graphicsCore->startFrame();
	MainScene::instance()->accept(&drawVisitor);
	Selector::instance()->draw();
	FrameOfReferenceState::instance()->draw();
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	StringManager::init();
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, drawFunc, 640, 480, true, false);

	fillSceneForObjectsSelectionTesting();
	transitionModifier.setWindow(GraphicsCore::instance()->getWindow());

	return GraphicsCore::instance()->run();
}