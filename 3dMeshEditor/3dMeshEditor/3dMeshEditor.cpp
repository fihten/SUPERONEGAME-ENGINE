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
#include "SphereCreator.h"
#include "CubeCreator.h"
#include "ConeCreator.h"
#include "Resource.h"
#include "Utils.h"
#include <Windows.h>
#include <commdlg.h>
#include <windef.h>
#include <sstream>

#pragma warning(disable : 4996)

Transition transitionModifier;
Rotation rotationModifier;
Scaling scalingModifier;
SphereCreator sphereCreator;
CubeCreator cubeCreator;
ConeCreator coneCreator;

Modifier* modifier;

DrawVisitor drawVisitor;

HWND hwnd;
HWND toolsHwnd;

HICON hTranslationIcon;
HICON hRotationIcon;
HICON hScalingIcon;

static OPENFILENAME ofn;

WCHAR fileTitle[MAX_PATH];
WCHAR title[MAX_PATH];

bool needSave = false;

void PopFileInitialize(HWND hwnd)
{
	static TCHAR szFilter[] = TEXT("Scenes (*.SCN)\0*.scn\0\0");
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = NULL; 
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL; 
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = 0; 
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = TEXT("scn");
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
}

BOOL PopFileOpenDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT;
	return GetOpenFileName(&ofn);
}

BOOL PopFileSaveDlg(HWND hwnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = pstrFileName;
	ofn.lpstrFileTitle = pstrTitleName;
	ofn.Flags = OFN_OVERWRITEPROMPT;
	return GetSaveFileName(&ofn);
}

short AskAboutSave(HWND hwnd, TCHAR* szTitleName)
{
	TCHAR szBuffer[64 + MAX_PATH];
	int iReturn;
	wsprintf(szBuffer, TEXT("Save current changes in %s?"),
		szTitleName[0] ? szTitleName : L"");
	iReturn = MessageBox(hwnd, szBuffer, L"",
		MB_YESNOCANCEL | MB_ICONQUESTION);
	if (iReturn == IDYES)
		if (!SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0))
			iReturn = IDCANCEL;
	return iReturn;
}
INT_PTR CALLBACK toolsDlg(HWND, UINT msg, WPARAM wparam, LPARAM lparam){	switch (msg)	{	case WM_COMMAND:	{		switch (wparam)		{		case IDC_TRANSLATION:		{			modifier = &transitionModifier;			SetFocus(hwnd);			break;		}		case IDC_ROTATION:		{			modifier = &rotationModifier;			SetFocus(hwnd);			break;		}		case IDC_SCALING:		{			modifier = &scalingModifier;			SetFocus(hwnd);			break;		}		}		break;	}	}	return 0;}

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
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, // style flags
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

	transitionModifier.setWindow(hwnd);
	rotationModifier.setWindow(hwnd);
	scalingModifier.setWindow(hwnd);
	sphereCreator.setWindow(hwnd);
	cubeCreator.setWindow(hwnd);
	coneCreator.setWindow(hwnd);

	modifier = &coneCreator;

	// Even though we just created a window, it is not initially
	// shown. Therefore, the final step is to show and update the
	// window we just created, which can be done with the following
	// two function calls. Observe that we pass the handle to the
	// window we want to show and update so that these functions know
	// which window to show and update.
	ShowWindow(hwnd, show);
	UpdateWindow(hwnd);

	toolsHwnd = CreateDialog(instanceHandle, (LPCTSTR)IDD_TOOLS, hwnd, toolsDlg);
	ShowWindow(toolsHwnd, SW_SHOW);

	hTranslationIcon = LoadIcon(instanceHandle, (LPCTSTR)IDI_TRANSLATION);
	SendMessage(GetDlgItem(toolsHwnd, IDC_TRANSLATION), BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hTranslationIcon);

	hRotationIcon = LoadIcon(instanceHandle, (LPCTSTR)IDI_ROTATION);
	SendMessage(GetDlgItem(toolsHwnd, IDC_ROTATION), BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hRotationIcon);

	hScalingIcon = LoadIcon(instanceHandle, (LPCTSTR)IDI_SCALING);
	SendMessage(GetDlgItem(toolsHwnd, IDC_SCALING), BM_SETIMAGE, (WPARAM)IMAGE_ICON, (LPARAM)hScalingIcon);

	return hwnd;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static float mousePosX0 = 0;
	static float mousePosY0 = 0;

	static float mousePosX1 = 0;
	static float mousePosY1 = 0;

	static bool bMarginSelection = false;

	if (modifier != nullptr)
	{
		auto behaviour = modifier->processWindowMessage(msg, wparam, lparam);
		if (behaviour == Modifier::Behaviour::FINISH)
			return 0;
	}

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
		flt3 segV0;
		flt3 segV1;
		calculateRay(hwnd, lparam, segV0, segV1);
		Selector::instance()->selectObject(segV0, segV1);
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
	case WM_COMMAND:
	{
		switch (LOWORD(wparam))
		{
		case ID_FILE_NEW:
		{
			if (needSave && IDCANCEL == AskAboutSave(hwnd, title))
				return 0;

			fileTitle[0] = L'\0';
			title[0] = L'\0';

			MainScene::instance()->clear();
			MainScene::instance()->updateGpu();

			return 0;
		}
		case ID_FILE_OPEN:
		{
			if (needSave && IDCANCEL == AskAboutSave(hwnd, title))
				return 0;

			PopFileOpenDlg(hwnd, fileTitle, title);

			char fileTitleA[MAX_PATH];
			wcstombs(fileTitleA, fileTitle, MAX_PATH);
			MainScene::instance()->load(fileTitleA);

			return 0;
		}
		case ID_FILE_SAVE:
		{
			if (fileTitle[0] == L'\0')
				PopFileSaveDlg(hwnd, fileTitle, title);

			char fileTitleA[MAX_PATH];
			wcstombs(fileTitleA, fileTitle, MAX_PATH);
			MainScene::instance()->save(fileTitleA);

			needSave = false;

			return 0;
		}
		case ID_FILE_SAVEAS:
		{
			PopFileSaveDlg(hwnd, fileTitle, title);

			char fileTitleA[MAX_PATH];
			wcstombs(fileTitleA, fileTitle, MAX_PATH);
			MainScene::instance()->save(fileTitleA);

			needSave = false;

			return 0;
		}
		case IDM_EXIT:

			return 0;
		}
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
	modifier->draw();
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	StringManager::init();
	GraphicsCore::instance()->init(hInstance, iCmdShow, WndProc, windowCreator, drawFunc, 640, 480, true, false);

	sphereCreator.init();
	cubeCreator.init();
	coneCreator.init();

//	fillSceneForObjectsSelectionTesting();
	PopFileInitialize(hwnd);

	return GraphicsCore::instance()->run();
}