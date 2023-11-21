#include "GraphicsCore.h"
#include "StringManager.h"
#include "Mesh.h"

Mesh testMesh;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void drawFunc(GraphicsCore* graphicsCore)
{
	graphicsCore->startFrame();
	graphicsCore->draw(testMesh);
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	StringManager::init();
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, drawFunc, 640, 480, true, false);

	GraphicsCore::instance()->openTextureA("imageA.jpg");
	GraphicsCore::instance()->openTextureB("white.jpg");

	testMesh = createMeshForTestingDefinitionOfTheSamePoints();
	ParamKey imageB_key{ StringManager::toStringId("imageB"),-1, string_id(-1) };
	testMesh.setParam(imageB_key, StringManager::toStringId("white.jpg"));

	return GraphicsCore::instance()->run();
}