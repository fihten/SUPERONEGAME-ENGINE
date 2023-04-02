#pragma once
#include "All.h"
#include "Mesh.h"
#include "Matrix4x4.h"
#include <WinDef.h>
#include <dxgiformat.h>
#include <dxgi.h>
#include <memory>

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

	flt4x4 getFloat4x4(Mesh& mesh, const std::string& var) const;
	flt3 getFloat3(Mesh& mesh, const std::string& var) const;
	void* getStruct(Mesh& mesh, const std::string& var, int* bytes = nullptr) const;
	
	void setFloat4x4sOnGPU(Mesh& mesh);
	void setFloat3sOnGPU(Mesh& mesh);
	void setStructsOnGPU(Mesh& mesh);

	void setVariablesOnGPU(Mesh& mesh);

private:
	UINT mWidth = 0;
	UINT mHeight = 0;

	bool mEnable4xMsaa = false;
	UINT m4xMsaaQuality = 0;

	HWND mWindow = 0;

	bool mWindowed = false;

	IDXGISwapChain* mSwapChain = 0;
	ID3D11RenderTargetView* mRenderTargetView = 0;

	ID3D11Texture2D* mDepthStencilBuffer = 0;
	ID3D11DepthStencilView* mDepthStencilView = 0;

	DRAW_FUNC drawFunc = nullptr;
};