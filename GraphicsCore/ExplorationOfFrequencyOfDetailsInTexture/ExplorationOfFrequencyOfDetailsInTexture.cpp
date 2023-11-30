#include "GraphicsCore.h"
#include "StringManager.h"
#include "Mesh.h"
#include <algorithm>
#include <cmath>

int radius = 10;
int thickness = 4;

Mesh testMesh;
Mesh areaOfIntegrationInA;

Mesh fourierCoeffsOfRatCos;
Mesh fourierCoeffsOfGatCos;
Mesh fourierCoeffsOfBatCos;

Mesh fourierCoeffsOfRatSin;
Mesh fourierCoeffsOfGatSin;
Mesh fourierCoeffsOfBatSin;

enum FourierCoeffs
{
	atCos,
	atSin,
	ofR,
	ofG,
	ofB
};
FourierCoeffs chanel = ofR;
FourierCoeffs basis = atCos;

bool bUpdateCoeffsAtCosOfR = true;
bool bUpdateCoeffsAtCosOfG = true;
bool bUpdateCoeffsAtCosOfB = true;

bool bUpdateCoeffsAtSinOfR = true;
bool bUpdateCoeffsAtSinOfG = true;
bool bUpdateCoeffsAtSinOfB = true;

float uA = 0;
float vA = 0;

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

		bUpdateCoeffsAtCosOfR = true;
		bUpdateCoeffsAtCosOfG = true;
		bUpdateCoeffsAtCosOfB = true;

		bUpdateCoeffsAtSinOfR = true;
		bUpdateCoeffsAtSinOfG = true;
		bUpdateCoeffsAtSinOfB = true;

		break; 
	}
	case WM_CHAR:
		switch (wParam)
		{
		case '+':
			radius++;
			
		case '-':
		{
			ParamKey radiusOfArrow_key{ StringManager::toStringId("radiusOfArrow"), -1, string_id(-1) };
			areaOfIntegrationInA.setParam(radiusOfArrow_key, wParam == '+' ? radius : radius - 1);

			bUpdateCoeffsAtCosOfR = true;
			bUpdateCoeffsAtCosOfG = true;
			bUpdateCoeffsAtCosOfB = true;

			bUpdateCoeffsAtSinOfR = true;
			bUpdateCoeffsAtSinOfG = true;
			bUpdateCoeffsAtSinOfB = true;

			if (wParam == '+')
				break;

			radius--;
			radius = std::max<int>(radius, 1);
			break; 
		}
		case 'c':
			basis = atCos;
			break;
		case 's':
			basis = atSin;
			break;
		case 'r':
			chanel = ofR;
			break;
		case 'g':
			chanel = ofG;
			break;
		case 'b':
			chanel = ofB;
			break;
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

void drawFunc(GraphicsCore* graphicsCore)
{
	graphicsCore->startFrame();
	graphicsCore->draw(testMesh);
	graphicsCore->draw(areaOfIntegrationInA);

	int wA = GraphicsCore::instance()->getWidthOfTextureA();
	int hA = GraphicsCore::instance()->getHeightOfTextureA();
	Vec2d<int> centerOfArrowInA(uA * wA - 0.5f, vA * hA - 0.5f);

	bool forceUpdatingGeometryOnGPU = false;
	switch (basis)
	{
	case atCos:
		switch (chanel)
		{
		case ofR:
			if (bUpdateCoeffsAtCosOfR)
			{
				float fourierCoeffs[1000];
				int N = 0;
				GraphicsCore::instance()->calculateFourierCoefficientsAtCosForRChanel(
					centerOfArrowInA, radius, thickness, fourierCoeffs, N
				);

				fourierCoeffsOfRatCos = createHistogram(fourierCoeffs, N);

				bUpdateCoeffsAtCosOfR = false;
				forceUpdatingGeometryOnGPU = true;
			}

			GraphicsCore::instance()->draw(fourierCoeffsOfRatCos, forceUpdatingGeometryOnGPU);
			break;
		case ofG:
			if (bUpdateCoeffsAtCosOfG)
			{
				float fourierCoeffs[1000];
				int N = 0;
				GraphicsCore::instance()->calculateFourierCoefficientsAtCosForGChanel(
					centerOfArrowInA, radius, thickness, fourierCoeffs, N
				);

				fourierCoeffsOfGatCos = createHistogram(fourierCoeffs, N);

				bUpdateCoeffsAtCosOfG = false;
				forceUpdatingGeometryOnGPU = true;
			}

			GraphicsCore::instance()->draw(fourierCoeffsOfGatCos, forceUpdatingGeometryOnGPU);
			break;
		case ofB:
			if (bUpdateCoeffsAtCosOfB)
			{
				float fourierCoeffs[1000];
				int N = 0;
				GraphicsCore::instance()->calculateFourierCoefficientsAtCosForBChanel(
					centerOfArrowInA, radius, thickness, fourierCoeffs, N
				);

				fourierCoeffsOfBatCos = createHistogram(fourierCoeffs, N);

				bUpdateCoeffsAtCosOfB = false;
				forceUpdatingGeometryOnGPU = true;
			}

			GraphicsCore::instance()->draw(fourierCoeffsOfBatCos, forceUpdatingGeometryOnGPU);
			break;
		}
		break;
	case atSin:
		switch (chanel)
		{
		case ofR:
			if (bUpdateCoeffsAtSinOfR)
			{
				float fourierCoeffs[1000];
				int N = 0;
				GraphicsCore::instance()->calculateFourierCoefficientsAtSinForRChanel(
					centerOfArrowInA, radius, thickness, fourierCoeffs, N
				);

				fourierCoeffsOfRatSin = createHistogram(fourierCoeffs, N);

				bUpdateCoeffsAtSinOfR = false;
				forceUpdatingGeometryOnGPU = true;
			}

			GraphicsCore::instance()->draw(fourierCoeffsOfRatSin, forceUpdatingGeometryOnGPU);
			break;
		case ofG:
			if (bUpdateCoeffsAtSinOfG)
			{
				float fourierCoeffs[1000];
				int N = 0;
				GraphicsCore::instance()->calculateFourierCoefficientsAtSinForGChanel(
					centerOfArrowInA, radius, thickness, fourierCoeffs, N
				);

				fourierCoeffsOfGatSin = createHistogram(fourierCoeffs, N);

				bUpdateCoeffsAtSinOfG = false;
				forceUpdatingGeometryOnGPU = true;
			}

			GraphicsCore::instance()->draw(fourierCoeffsOfGatSin, forceUpdatingGeometryOnGPU);
			break;
		case ofB:
			if (bUpdateCoeffsAtSinOfB)
			{
				float fourierCoeffs[1000];
				int N = 0;
				GraphicsCore::instance()->calculateFourierCoefficientsAtSinForBChanel(
					centerOfArrowInA, radius, thickness, fourierCoeffs, N
				);

				fourierCoeffsOfBatSin = createHistogram(fourierCoeffs, N);

				bUpdateCoeffsAtSinOfB = false;
				forceUpdatingGeometryOnGPU = true;
			}

			GraphicsCore::instance()->draw(fourierCoeffsOfBatSin, forceUpdatingGeometryOnGPU);
			break;
		}
		break;
	}

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