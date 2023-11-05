#include "GraphicsCore.h"
#include "StringManager.h"
#include "Mesh.h"
#include <algorithm>

#define CALCULATE_INTEGRALS_ON_CPU

Mesh testMesh;

float uA;
float vA;

float uB;
float vB;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP:
#ifndef CALCULATE_INTEGRALS_ON_CPU
	{
		float mousePosX = LOWORD(lParam) + 0.5f;
		float mousePosY = HIWORD(lParam) + 0.5f;

		RECT rect;
		GetClientRect(hwnd, &rect);

		float width = rect.right - rect.left;
		float height = std::floor(0.5f * (rect.bottom - rect.top));

		mousePosX = std::min<float>(std::max<float>(0.0f, mousePosX), width - 1);
		mousePosY = std::min<float>(std::max<float>(0.0f, mousePosY), height - 1);

		flt2 uvA((mousePosX + 0.5f) / width, (mousePosY + 0.5f) / height);
		flt2 uvB = GraphicsCore::instance()->mapAtoB(uvA);

		ParamKey uvA_key{ StringManager::toStringId("posInA"), -1, string_id(-1) };
		testMesh.setParam(uvA_key, uvA);

		ParamKey mappedPos_key{ StringManager::toStringId("mappedPos"), -1, string_id(-1) };
		testMesh.setParam(mappedPos_key, uvB); 
	}
#else
	{
		float mousePosX = LOWORD(lParam) + 0.5f;
		float mousePosY = HIWORD(lParam) + 0.5f;

		RECT rect;
		GetClientRect(hwnd, &rect);

		float width = rect.right - rect.left;
		float height = std::floor(0.5f * (rect.bottom - rect.top));

		if (mousePosY < height)
		{
			uA = (mousePosX + 0.5f) / width;
			vA = (mousePosY + 0.5f) / height;
		}
		else
		{
			mousePosY -= height;
			uB = (mousePosX + 0.5f) / width;
			vB = (mousePosY + 0.5f) / height;
		}

		flt2 uvA(uA, vA);
		ParamKey uvA_key{ StringManager::toStringId("posInA"), -1, string_id(-1) };
		testMesh.setParam(uvA_key, uvA);

		flt2 uvB(uB, vB);
		ParamKey mappedPos_key{ StringManager::toStringId("mappedPos"), -1, string_id(-1) };
		testMesh.setParam(mappedPos_key, uvB); 
	}
#endif
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_SPACE:
			OutputDebugStringA("integrals calculation starts\n");
			GraphicsCore::instance()->calculateIntegralsOnCpu(
				uA, vA, uB, vB
			);
			OutputDebugStringA("integrals calculation finishes\n");
			break;
		}
	}
	}
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
	GraphicsCore::instance()->openTextureB("imageB.jpg");

//	GraphicsCore::instance()->defineTheSamePoints();

	testMesh = createMeshForTestingDefinitionOfTheSamePoints();


	return GraphicsCore::instance()->run();
}