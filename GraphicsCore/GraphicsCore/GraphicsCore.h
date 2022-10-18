#pragma once
#include "All.h"
#include "Mesh.h"
#include <WinDef.h>
#include <dxgiformat.h>
#include <dxgi.h>

class GraphicsCore;

typedef LRESULT (*(CALLBACK WNDPROC))(HWND, UINT, WPARAM, LPARAM);
typedef void (*DRAW_FUNC)(GraphicsCore*);

class GraphicsCore
{
	ID3D11Device* device = 0;
	ID3D11DeviceContext* context = 0;

public:
	void init(HINSTANCE instanceHandle, int show, WNDPROC WndProc, DRAW_FUNC drawFunc, UINT width, UINT height, bool windowed, bool enable4xMsaa);
	
	void startFrame();
	void endFrame();

	void draw(Mesh& mesh);
	
	int run();
	void resize(UINT width, UINT height);
	
private:
	bool initWindow(HINSTANCE instanceHandle, int show, WNDPROC WndProc);

private:
	UINT mWidth = 0;
	UINT mHeight = 0;

	bool mEnable4xMsaa = false;
	UINT m4xMsaaQuality = 0;

	HWND mWindow = 0;

	bool mWindowed = false;

	IDXGISwapChain* mSwapChain = 0;
	ID3D11RenderTargetView* mRenderTargetView;

	ID3D11Texture2D* mDepthStencilBuffer = 0;
	ID3D11DepthStencilView* mDepthStencilView;

	DRAW_FUNC drawFunc = nullptr;
};