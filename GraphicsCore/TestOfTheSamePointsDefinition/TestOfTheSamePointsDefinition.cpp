#include "GraphicsCore.h"
#include "StringManager.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

void drawFunc(GraphicsCore* graphicsCore)
{

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	StringManager::init();
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, drawFunc, 640, 480, true, false);


	return GraphicsCore::instance()->run();
}