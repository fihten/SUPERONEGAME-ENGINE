#pragma once
#include "All.h"
#include "Mesh.h"
#include "Matrix4x4.h"
#include <WinDef.h>
#include <dxgiformat.h>
#include <dxgi.h>
#include <memory>
#include "Envelope.h"

class GraphicsCore;

typedef LRESULT (*(CALLBACK WNDPROC))(HWND, UINT, WPARAM, LPARAM);
typedef void (*DRAW_FUNC)(GraphicsCore*);

class Selector;

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

private:
	void initRoughObjectsSelection();
	ID3DX11Effect* mRoughObjectsSelectionFX = nullptr;
	ID3DX11EffectTechnique* mRoughObjectsSelectionTech = nullptr;
	ID3DX11EffectShaderResourceVariable* mEnvelopes = nullptr;
	ID3DX11EffectVariable* mEnvelopesCount = nullptr;
	ID3DX11EffectVariable* mSelectorEnvelopeRough = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mSelectedObjects = nullptr;
	ID3DX11EffectMatrixVariable* mVP = nullptr;

	uint32_t envelopesCount = 0;
	
	ID3D11Buffer* mInputSelectedObjectsBuffer = nullptr;
	ID3D11UnorderedAccessView* selectedObjectsUAV = nullptr;

	ID3D11Buffer* mOutputSelectedObjectsBuffer = nullptr;

	ID3D11Buffer* mEnvelopesBuffer = nullptr;
	ID3D11ShaderResourceView* envelopesBufferSRV = nullptr;

	void setEnvelopes(ID3D11ShaderResourceView* envelopesSRV);
	void setEnvelopesCount(uint32_t envelopesCount);
	void setSelectorEnvelopeRough(Envelope& selectorEnvelope);
	void setVP(flt4x4& VP);

	void findRoughlySelectedObjects();

	void getRoughlySelectedObjects(uint32_t* selectedObjects);

private:
	void initFineObjectsSelection();
	ID3DX11Effect* mFineObjectsSelectionFX = nullptr;
	ID3DX11EffectTechnique* mFineObjectsSelectionTech = nullptr;
	ID3DX11EffectVariable* mSelectorEnvelopeFine = nullptr;
	ID3DX11EffectMatrixVariable* mWVP = nullptr;
	ID3DX11EffectVariable* mThreshold = nullptr;
	ID3DX11EffectShaderResourceVariable* mVertices = nullptr;
	ID3DX11EffectShaderResourceVariable* mIndicies = nullptr;
	ID3DX11EffectVariable* mTrianglesCount = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mSelectedTriangles = nullptr;

	ID3D11Buffer* mInputSelectedTrianglesBuffer = nullptr;
	ID3D11UnorderedAccessView* selectedTrianglesUAV = nullptr;

	ID3D11Buffer* mOutputSelectedTrianglesBuffer = nullptr;

	void setSelectorEnvelopeFine(Envelope& selectorEnvelope);
	void setWVP(flt4x4& WVP);
	void setThreshold(float threshold);
	void setVertices(ID3D11ShaderResourceView* verticesSRV);
	void setIndicies(ID3D11ShaderResourceView* indiciesSRV);
	void setTrianglesCount(uint32_t trianglesCount);

	uint32_t trianglesCount = 0;

	void checkIntersection();
	bool isObjectIntersected();

	friend Selector;

private:
	bool initWindow(HINSTANCE instanceHandle, int show, WNDPROC WndProc);

	flt4x4 getFloat4x4(const Mesh& mesh, const std::string& var) const;
	flt4 getFloat4(const Mesh& mesh, const std::string& var) const;
	flt3 getFloat3(const Mesh& mesh, const std::string& var) const;
	flt2 getFloat2(const Mesh& mesh, const std::string& var) const;
	flt1 getFloat1(const Mesh& mesh, const std::string& var) const;
	void* getStruct(const Mesh& mesh, const std::string& var, int* bytes = nullptr) const;
	
	ID3D11Buffer* getVertexBuffer(const Mesh& mesh, uint32_t* elementSize) const;
	void* fetchVerticesFromMesh(const Mesh& mesh) const;

	ID3D11Buffer* getIndexBuffer(const Mesh& mesh) const;

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