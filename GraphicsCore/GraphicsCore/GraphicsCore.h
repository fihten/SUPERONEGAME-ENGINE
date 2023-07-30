#pragma once
#include "All.h"
#include "Mesh.h"
#include "Matrix4x4.h"
#include <WinDef.h>
#include <dxgiformat.h>
#include <dxgi.h>
#include <memory>
#include "Frustum.h"
#include "Segment.h"

#define MAX_BOUNDING_SPHERES_COUNT 65536

class GraphicsCore;

typedef LRESULT (*(CALLBACK WNDPROC))(HWND, UINT, WPARAM, LPARAM);
typedef void (*DRAW_FUNC)(GraphicsCore*);

class RoughlySelectedObjectVisitor
{
public:
	virtual void operator()(uint32_t objectID) = 0;
};

class GraphicsCore
{
	ID3D11Device* device = 0;
	ID3D11DeviceContext* context = 0;

	GraphicsCore() {}

public:
	static GraphicsCore* instance();

	void init(HINSTANCE instanceHandle, int show, WNDPROC WndProc, DRAW_FUNC drawFunc, UINT width, UINT height, bool windowed, bool enable4xMsaa);

	void startFrame();
	void endFrame();

	void draw(const Mesh& mesh);
	
	int run();
	void resize(UINT width, UINT height);

public:
	void updateBoundingSpheres(flt4 spheres[]);
	void setSpheresCount(uint32_t spheresCount);
	void setSelectorFrustumRough(Frustum& selectorFrustum);
	void setV(flt4x4& V);
	void findRoughlySelectedObjects();
	void traverseRoughlySelectedObjects(RoughlySelectedObjectVisitor* visitor);

private:
	void initRoughObjectsSelection();
	ID3DX11Effect* mRoughObjectsSelectionFX = nullptr;
	ID3DX11EffectTechnique* mRoughObjectsSelectionTech = nullptr;
	ID3DX11EffectShaderResourceVariable* mBoundingSpheres = nullptr;
	ID3DX11EffectVariable* mSpheresCount = nullptr;
	ID3DX11EffectVariable* mSelectorFrustumRough = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mSelectedObjects = nullptr;
	ID3DX11EffectMatrixVariable* mV = nullptr;

	uint32_t spheresCount = 0;

	ID3D11Buffer* mInputSelectedObjectsBuffer = nullptr;
	ID3D11UnorderedAccessView* mSelectedObjectsUAV = nullptr;
	ID3D11Buffer* mOutputSelectedObjectsBuffer = nullptr;

	ID3D11Buffer* mBoundingSpheresBuffer = nullptr;
	ID3D11ShaderResourceView* mBoundingSpheresBufferSRV = nullptr;

public:
	void setSelectorFrustumFine(Frustum& selectorFrustum);
	void updateSelectorFrustumDiagonals(Segment diagonals[]);
	void setWV(flt4x4& WV);
	void setThreshold(float threshold);
	void setGeometryForFineSelection(const Mesh& mesh);
	void setTrianglesCount(uint32_t trianglesCount);
	void checkIntersection();
	bool isObjectIntersected();

private:
	void initFineObjectsSelection();
	ID3DX11Effect* mFineObjectsSelectionFX = nullptr;
	ID3DX11EffectTechnique* mFineObjectsSelectionTech = nullptr;
	ID3DX11EffectVariable* mSelectorFrustumFine = nullptr;
	ID3DX11EffectShaderResourceVariable* mSelectorFrustumDiagonals = nullptr;
	ID3DX11EffectMatrixVariable* mWV = nullptr;
	ID3DX11EffectVariable* mThreshold = nullptr;
	
	ID3DX11EffectShaderResourceVariable* mVertices = nullptr;
	ID3DX11EffectShaderResourceVariable* mIndicies = nullptr;
	ID3DX11EffectVariable* mTrianglesCount = nullptr;
	
	ID3DX11EffectUnorderedAccessViewVariable* mSelectedTriangles = nullptr;
	ID3D11Buffer* mInputSelectedTrianglesBuffer = nullptr;
	ID3D11UnorderedAccessView* selectedTrianglesUAV = nullptr;
	ID3D11Buffer* mOutputSelectedTrianglesBuffer = nullptr;

	uint32_t trianglesCount = 0;

	ID3D11Buffer* mSelectorFrustumDiagonalsBuffer = nullptr;
	ID3D11ShaderResourceView* mSelectorFrustumDiagonalsBufferSRV = nullptr;

private:
	bool initWindow(HINSTANCE instanceHandle, int show, WNDPROC WndProc);

	flt4x4 getFloat4x4(const Mesh& mesh, const std::string& var) const;
	flt4 getFloat4(const Mesh& mesh, const std::string& var) const;
	flt3 getFloat3(const Mesh& mesh, const std::string& var) const;
	flt2 getFloat2(const Mesh& mesh, const std::string& var) const;
	flt1 getFloat1(const Mesh& mesh, const std::string& var) const;
	void* getStruct(const Mesh& mesh, const std::string& var, int* bytes = nullptr) const;
	
	ID3D11Buffer* getVertexBuffer(
		const Mesh& mesh,
		uint32_t* elementSize,
		const char* technique = nullptr,
		const char* pass = nullptr,
		bool structured = false
		) const;
	void* fetchVerticesFromMesh(
		const Mesh& mesh,
		const char* technique = nullptr,
		const char* pass = nullptr
		) const;

	ID3D11Buffer* getIndexBuffer(const Mesh& mesh, bool structured = false) const;

	ID3D11ShaderResourceView* getImage(const Mesh& mesh, const std::string& var) const;
	ID3D11ShaderResourceView* getImagesArray(const Mesh& mesh, const std::string& var) const;

	void setFloat4x4sOnGPU(const Mesh& mesh);
	void setFloat4sOnGPU(const Mesh& mesh);
	void setFloat3sOnGPU(const Mesh& mesh);
	void setFloat2sOnGPU(const Mesh& mesh);
	void setFloat1sOnGPU(const Mesh& mesh);
	void setStructsOnGPU(const Mesh& mesh);
	void setTexturesOnGPU(const Mesh& mesh);
	void setTexturesArraysOnGPU(const Mesh& mesh);

	void setVariablesOnGPU(const Mesh& mesh);

	void setGeometryOnGPU(const Mesh& mesh);

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

private:
	static GraphicsCore* pGraphicsCore;
};