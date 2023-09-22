#include "GraphicsCore.h"
#include "StringManager.h"
#include "Mesh.h"

Mesh testMesh;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

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
	GraphicsCore::instance()->interpolateTextureA();

	GraphicsCore::instance()->openTextureB("imageB.jpg");
	GraphicsCore::instance()->interpolateTextureB();

	GraphicsCore::instance()->defineTheSamePoints();

	testMesh = createMeshForTestingDefinitionOfTheSamePoints();


	return GraphicsCore::instance()->run();
}