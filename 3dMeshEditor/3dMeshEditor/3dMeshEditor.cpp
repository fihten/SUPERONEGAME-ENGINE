#include "GraphicsCore.h"
#include "Cameras.h"
#include <Windows.h>

GraphicsCore graphicsCore;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void drawFunc(GraphicsCore* graphicsCore)
{

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	graphicsCore.init(hInstance, iCmdShow, WndProc, drawFunc, 100, 200, true, false);

	return graphicsCore.run();
}