#include "GraphicsCore.h"
#include "StringManager.h"
#include "Mesh.h"
#include <algorithm>

#define CALCULATE_INTEGRALS_ON_CPU

Mesh testMesh;
Mesh areaOfIntegrationInA;
Mesh areaOfIntegrationInB;

float uA;
float vA;

float uB;
float vB;

bool showAreasOfIntegration = false;

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
			uA = 0.75;// (mousePosX + 0.5f) / width;
			vA = 0.48;// (mousePosY + 0.5f) / height;
		}
		else
		{
			mousePosY -= height;
			uB = 0.75;//(mousePosX + 0.5f) / width;
			vB = 0.48;//(mousePosY + 0.5f) / height;
		}

		flt2 uvA(uA, vA);
		ParamKey uvA_key{ StringManager::toStringId("posInA"), -1, string_id(-1) };
		testMesh.setParam(uvA_key, uvA);

		flt2 uvB(uB, vB);
		ParamKey mappedPos_key{ StringManager::toStringId("mappedPos"), -1, string_id(-1) };
		testMesh.setParam(mappedPos_key, uvB); 
		
		// set params of area of integration within texture A
		int radiusOfIntegrationArea = GraphicsCore::instance()->getRadiusOfBiggestAreaOfIntegration();
		ParamKey radiusOfArrow_key{ StringManager::toStringId("radiusOfArrow"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(radiusOfArrow_key, radiusOfIntegrationArea);

		ParamKey angle0_key{ StringManager::toStringId("angle0"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(angle0_key, 0.0f);

		ParamKey scale0_key{ StringManager::toStringId("scale0"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(scale0_key, 1.0f);

		ParamKey angle1_key{ StringManager::toStringId("angle1"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(angle1_key, 0.0f);

		ParamKey scale1_key{ StringManager::toStringId("scale1"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(scale1_key, 1.0f);

		int wA = GraphicsCore::instance()->getWidthOfTextureA();
		ParamKey width_key{ StringManager::toStringId("width"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(width_key, wA);

		int hA = GraphicsCore::instance()->getHeightOfTextureA();
		ParamKey height_key{ StringManager::toStringId("height"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(height_key, hA);

		ParamKey ndcOffsetY_key{ StringManager::toStringId("ndcOffsetY"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(ndcOffsetY_key, 0.0f);

		Vec2d<int> centerOfArrowInA(uA * wA - 0.5f, vA * hA - 0.5f);
		ParamKey centerOfArrow_key{ StringManager::toStringId("centerOfArrow"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(centerOfArrow_key, centerOfArrowInA);

		// set params of area of integration within texture B
		areaOfIntegrationInB.setParam(radiusOfArrow_key, radiusOfIntegrationArea);

		areaOfIntegrationInB.setParam(angle0_key, 0.0f);
		areaOfIntegrationInB.setParam(scale0_key, 1.0f);

		areaOfIntegrationInB.setParam(angle1_key, 0.0f);
		areaOfIntegrationInB.setParam(scale1_key, 1.0f);

		int wB = GraphicsCore::instance()->getWidthOfTextureB();
		areaOfIntegrationInB.setParam(width_key, wB);

		int hB = GraphicsCore::instance()->getHeightOfTextureB();
		areaOfIntegrationInB.setParam(height_key, hB);

		areaOfIntegrationInB.setParam(ndcOffsetY_key, -1.0f);

		Vec2d<int> centerOfArrowInB(uB * wB - 0.5f, vB * hB - 0.5f);
		areaOfIntegrationInB.setParam(centerOfArrow_key, centerOfArrowInB);
	}
#endif
		break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_SPACE:
			OutputDebugStringA("integrals calculation starts\n");

			float angle0 = 0;
			float scale0 = 1;

			float angle1 = 0;
			float scale1 = 1;

			GraphicsCore::instance()->calculateIntegralsAtTwoPointsOfAandB(
				uA, vA, uB, vB,
				angle0, scale0,
				angle1, scale1
			);

			ParamKey angle0_key{ StringManager::toStringId("angle0"), -1, string_id(-1) };
			areaOfIntegrationInB.setParam(angle0_key, angle0);

			ParamKey scale0_key{ StringManager::toStringId("scale0"), -1, string_id(-1) };
			areaOfIntegrationInB.setParam(scale0_key, scale0);

			ParamKey angle1_key{ StringManager::toStringId("angle1"), -1, string_id(-1) };
			areaOfIntegrationInB.setParam(angle1_key, angle1);

			ParamKey scale1_key{ StringManager::toStringId("scale1"), -1, string_id(-1) };
			areaOfIntegrationInB.setParam(scale1_key, scale1);

			OutputDebugStringA("integrals calculation finishes\n");
			break;
		}
	}
	case WM_CHAR:
	{
		switch (wParam)
		{
		case 's':
			showAreasOfIntegration = !showAreasOfIntegration;
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
	if (showAreasOfIntegration)
	{
		graphicsCore->draw(areaOfIntegrationInA);
		graphicsCore->draw(areaOfIntegrationInB);
	}
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	StringManager::init();
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, nullptr, drawFunc, 640, 480, true, false);

	GraphicsCore::instance()->openTextureA("imageA.jpg");
	GraphicsCore::instance()->openTextureB("imageB.jpg");

//	GraphicsCore::instance()->defineTheSamePoints();

	testMesh = createMeshForTestingDefinitionOfTheSamePoints();
	areaOfIntegrationInA = createMeshVisualizingAreaOfIntegration();
	areaOfIntegrationInB = createMeshVisualizingAreaOfIntegration();

	return GraphicsCore::instance()->run();
}