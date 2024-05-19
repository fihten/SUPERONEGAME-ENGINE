#include "GraphicsCore.h"
#include "StringManager.h"
#include "Mesh.h"
#include "ModelMaker.h"
#include <algorithm>

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

			int w = ModelMaker::instance()->getWidth();
			int h = ModelMaker::instance()->getHeight();

			Vec2d<int> ptA(uA * w - 0.5f, vA * h - 0.5f);
			auto ptAB = ModelMaker::instance()->findTheSamePoint(ptA);

			uA = ((float)ptAB.x() + 0.5f) / w;
			vA = ((float)ptAB.y() + 0.5f) / h;

			uB = ((float)ptAB.z() + 0.5f) / w;
			vB = ((float)ptAB.w() + 0.5f) / h;

			char buffer[256];
			std::sprintf(buffer, "\nuA - uB = %d; vA - vB = %d;\n", ptAB.x() - ptAB.z(), ptAB.y() - ptAB.w());
			OutputDebugStringA(buffer);
		}

		flt2 uvA(uA, vA);
		ParamKey uvA_key{ StringManager::toStringId("posInA"), -1, string_id(-1) };
		testMesh.setParam(uvA_key, uvA);

		flt2 uvB(uB, vB);
		ParamKey mappedPos_key{ StringManager::toStringId("mappedPos"), -1, string_id(-1) };
		testMesh.setParam(mappedPos_key, uvB);
	}
	break;
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
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, nullptr, drawFunc, 640, 480, true, false);


	testMesh = createMeshForTestingDefinitionOfTheSamePoints();

	{
		ModelMaker::instance()->init();

		std::vector<std::string> paths;
		paths.emplace_back("C:\\3dEngine\\Textures\\imageA.jpg");
		paths.emplace_back("C:\\3dEngine\\Textures\\imageB.jpg");
		ModelMaker::instance()->loadPhotos(paths);

		ModelMaker::instance()->defineTheSamePointsOnSetOfPhotos();

		ModelMaker::instance()->freeResources();
	}

	return GraphicsCore::instance()->run();
}