#include "GraphicsCore.h"
#include "StringManager.h"
#include "Mesh.h"
#include <algorithm>
#include <cmath>

int radius = 1;
Mesh testMesh;
Mesh areaOfIntegrationInA;
Mesh statisticOfTextureAtPoint;

int N = 0;
int y[1000];
bool updateStatisticMesh = true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP:

		float mousePosX = LOWORD(lParam) + 0.5f;
		float mousePosY = HIWORD(lParam) + 0.5f;

		RECT rect;
		GetClientRect(hwnd, &rect);

		float width = rect.right - rect.left;
		float height = std::floor(0.5f * (rect.bottom - rect.top));

		static float uA = 0;
		static float vA = 0;
		if (mousePosY < height)
		{
			uA = (mousePosX + 0.5f) / width;
			vA = (mousePosY + 0.5f) / height;
		}

		flt2 uvA(uA, vA);
		ParamKey uvA_key{ StringManager::toStringId("posInA"), -1, string_id(-1) };
		testMesh.setParam(uvA_key, uvA);

		// set params of area of integration within texture A

		int wA = GraphicsCore::instance()->getWidthOfTextureA();
		int hA = GraphicsCore::instance()->getHeightOfTextureA();

		Vec2d<int> centerOfArrowInA(uA * wA - 0.5f, vA * hA - 0.5f);
		ParamKey centerOfArrow_key{ StringManager::toStringId("centerOfArrow"), -1, string_id(-1) };
		areaOfIntegrationInA.setParam(centerOfArrow_key, centerOfArrowInA);

		GraphicsCore::instance()->calculateStatisticOfTextureAtPoint(centerOfArrowInA, radius, y, N);
		updateStatisticMesh = true;

		break;
	case WM_CHAR:
		switch (wParam)
		{
		case '+':
			radius++;
			
		case '-':

			GraphicsCore::instance()->calculateStatisticOfTextureAtPoint(
				centerOfArrowInA, wParam == '+' ? radius : radius - 1, y, N
			);
			updateStatisticMesh = true;

			ParamKey radiusOfArrow_key{ StringManager::toStringId("radiusOfArrow"), -1, string_id(-1) };
			areaOfIntegrationInA.setParam(radiusOfArrow_key, wParam == '+' ? radius : radius - 1);

			if (wParam == '+')
				break;

			radius--;
			radius = std::max<int>(radius, 1);
			break;
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void drawFunc(GraphicsCore* graphicsCore)
{
	graphicsCore->startFrame();
	graphicsCore->draw(testMesh);
	if (updateStatisticMesh)
	{
		int maxY = 0;
		for (int i = 0; i < N; i++)
			maxY = std::max<int>(maxY, y[i]);

		float fy[1000];
		for (int i = 0; i < N; i++)
			fy[i] = (float)(y[i]) / (float)(maxY);

		statisticOfTextureAtPoint = createHistogram(fy, N);
	}
	graphicsCore->draw(statisticOfTextureAtPoint);
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

	areaOfIntegrationInA = createMeshVisualizingAreaOfIntegration();

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

	return GraphicsCore::instance()->run();
}