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
#include "Rotation.h"
#include "Scaling.h"
#include "Resource.h"
#include <Windows.h>
#include <windef.h>

Transition transitionModifier;
Rotation rotationModifier;
Scaling scalingModifier;
DrawVisitor drawVisitor;

HWND hwnd;

HWND windowCreator(HINSTANCE instanceHandle, int width, int height, int show, WNDPROC WndProc)
{
	// The first task to creating a window is to describe some of its
	// characteristics by filling out a WNDCLASS structure.
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = L"MY3DMESHEDITOR";
	wc.lpszClassName = L"CANVAS";

	// Next we register this WNDCLASS instance with Windows so
	// that we can create a window based on it.
	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass FAILED", 0, 0);
		return false;
	}

	// With our WNDCLASS instance registered, we can create a
	// window with the CreateWindow function. This function
	// returns a handle to the window it creates (an HWND).
	// If the creation failed, the handle will have the value
	// of zero. A window handle is a way to refer to the window,
	// which is internally managed by Windows. Many of the Win32 API
	// functions that operate on windows require an HWND so that
	// they know what window to act on.

	hwnd = CreateWindow(
		L"CANVAS", // Registered WNDCLASS instance to use
		L"3d mesh editor", // window title
		WS_OVERLAPPEDWINDOW, // style flags
		CW_USEDEFAULT, // x-coordinate
		CW_USEDEFAULT, // y-coordinate
		width, // width
		height, // height
		0, // parent window
		0, // menu handle
		instanceHandle, // app instance
		0 // extra creation parameters
	);
	if (hwnd == 0)
	{
		MessageBox(0, L"CreateWindow FAILED", 0, 0);
		return false;
	}

	// Even though we just created a window, it is not initially
	// shown. Therefore, the final step is to show and update the
	// window we just created, which can be done with the following
	// two function calls. Observe that we pass the handle to the
	// window we want to show and update so that these functions know
	// which window to show and update.
	ShowWindow(hwnd, show);
	UpdateWindow(hwnd);

	return hwnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static float mousePosX0 = 0;
	static float mousePosY0 = 0;

	static float mousePosX1 = 0;
	static float mousePosY1 = 0;

	static bool bMarginSelection = false;

	auto behaviour = scalingModifier.processWindowMessage(msg, wparam, lparam);
	if (behaviour == Modifier::Behaviour::FINISH)
		return 0;

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

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void drawFunc(GraphicsCore* graphicsCore)
{
	graphicsCore->startFrame();
	MainScene::instance()->accept(&drawVisitor);
	Selector::instance()->draw();
	FrameOfReferenceState::instance()->drawScale();
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	StringManager::init();
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, windowCreator, drawFunc, 640, 480, true, false);

	fillSceneForObjectsSelectionTesting();
	
	transitionModifier.setWindow(hwnd);
	rotationModifier.setWindow(hwnd);
	scalingModifier.setWindow(hwnd);

	return GraphicsCore::instance()->run();
}