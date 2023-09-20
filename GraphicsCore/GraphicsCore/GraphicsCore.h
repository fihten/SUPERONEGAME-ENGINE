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
#include "VariableLocation.h"
#include "InputLayoutResource.h"
#include "VariableResources.h"
#include "OBjectInfo.h"

#define MAX_BOUNDING_SPHERES_COUNT 1024
#define MAX_VERTICES_COUNT 16384
#define MAX_TRIANGLES_COUNT 16384
#define MAX_OBJECTS_COUNT 64
#define MAX_SELECTING_SEGMENTS_COUNT 8

class GraphicsCore;

typedef LRESULT (*(CALLBACK WNDPROC))(HWND, UINT, WPARAM, LPARAM);
typedef void (*DRAW_FUNC)(GraphicsCore*);

class SelectedObjectVisitor
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

	void draw(Mesh& mesh);
	
	int run();
	void resize(UINT width, UINT height);

public:
	void updateBoundingSpheres(flt4 spheres[]);
	void setBoundingSpheresRoughByFrustum();
	void setSpheresCount(uint32_t spheresCount);
	void setSelectorFrustumRough(Frustum& selectorFrustum);
	void setSelectedObjectsForWritingRoughByFrustum();
	void setV(const flt4x4& V);
	void findRoughlySelectedObjects();
	void traverseRoughlySelectedObjects(SelectedObjectVisitor* visitor);

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
	ID3D11ShaderResourceView* mSelectedObjectsSRV = nullptr;
	ID3D11Buffer* mOutputSelectedObjectsBuffer = nullptr;

	ID3D11Buffer* mBoundingSpheresBuffer = nullptr;
	ID3D11ShaderResourceView* mBoundingSpheresBufferSRV = nullptr;

public:
	void setSelectorFrustumFine(Frustum& selectorFrustum);
	void updateSelectorFrustumDiagonals(Segment diagonals[]);
	void setSelectorFrustumDiagonals();
	void setVFine(const flt4x4& V);
	void setThreshold(float threshold);
	void updateVertices(flt3 vertices[]);
	void setVerticesFineByFrustum();
	void updateIndices(uint32_t indices[]);
	void setIndicesFineByFrustum();
	void updateObjectsInfo(ObjectInfo objectsInfo[], uint32_t count);
	void setObjectsInfoFineByFrustum();
	void setRoughlySelectedObjects();
	void setObjectsCount(uint32_t objectsCount);

	void initSelectedTrianglesWithZeros();
	void setSelectedTrianglesWriteFineByFrustum();
	void checkIntersection();
	void traverseFineSelectedObjects(SelectedObjectVisitor* visitor);
	void applyContextForFineSelection();

	uint32_t maxTrianglesCount = 0;

private:
	void initFineObjectsSelection();
	ID3DX11Effect* mFineObjectsSelectionFX = nullptr;
	ID3DX11EffectTechnique* mFineObjectsSelectionTech = nullptr;
	ID3DX11EffectVariable* mSelectorFrustumFine = nullptr;
	ID3DX11EffectShaderResourceVariable* mSelectorFrustumDiagonals = nullptr;
	ID3DX11EffectMatrixVariable* mVFine = nullptr;
	ID3DX11EffectVariable* mThreshold = nullptr;
	
	ID3DX11EffectShaderResourceVariable* mVertices = nullptr;
	ID3DX11EffectShaderResourceVariable* mIndicies = nullptr;
	
	ID3DX11EffectShaderResourceVariable* mObjectsInfo = nullptr;

	ID3DX11EffectShaderResourceVariable* mRoughlySelectedObjects = nullptr;
	ID3DX11EffectVariable* mObjectsCount = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* mSelectedTriangles = nullptr;

	ID3D11Buffer* mSelectorFrustumDiagonalsBuffer = nullptr;
	ID3D11ShaderResourceView* mSelectorFrustumDiagonalsBufferSRV = nullptr;

	ID3D11Buffer* mVerticesBuffer = nullptr;
	ID3D11ShaderResourceView* mVerticesBufferSRV = nullptr;

	ID3D11Buffer* mIndicesBuffer = nullptr;
	ID3D11ShaderResourceView* mIndicesBufferSRV = nullptr;

	ID3D11Buffer* mObjectsInfoBuffer = nullptr;
	ID3D11ShaderResourceView* mObjectsInfoBufferSRV = nullptr;

	ID3D11Buffer* mInputSelectedTrianglesBuffer = nullptr;
	ID3D11UnorderedAccessView* selectedTrianglesUAV = nullptr;
	ID3D11Buffer* mOutputSelectedTrianglesBuffer = nullptr;

public:
	void updateSelectingSegments(Segment segments[]);
	void setSelectingSegmentsRoughBySegments();

	void setSelectingSegmentsCountRoughBySegments(uint32_t count);

	void setBoundingSpheresRoughBySegments();
	void setBoundingSpheresCountRoughBySegments(uint32_t count);

	void setSelectedObjectsWriteRoughBySegments();

	void findRoughlySelectedObjectsBySegments();

private:
	void initRoughObjectsSelectionBySegments();

	ID3DX11Effect* mRoughObjectsSelectionBySegmentsFX = nullptr;
	ID3DX11EffectTechnique* mRoughObjectsSelectionBySegmentsTech = nullptr;

	ID3DX11EffectShaderResourceVariable* mSelectingSegments = nullptr;
	ID3DX11EffectVariable* mSelectingSegmentsCount = nullptr;
	
	ID3DX11EffectShaderResourceVariable* mBoundingSpheresRoughBySegments = nullptr;
	ID3DX11EffectVariable* mBoundingSpheresCountRoughBySegments = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* mSelectedObjectsRoughBySegments = nullptr;

	ID3D11Buffer* mSelectingSegmentsBuffer = nullptr;
	ID3D11ShaderResourceView* mSelectingSegmentsBufferSRV = nullptr;

	ID3D11Buffer* mSelectedObjectsRoughBySegmentsBuffer = nullptr;
	ID3D11UnorderedAccessView* mSelectedObjectsRoughBySegmentsUAV = nullptr;
	ID3D11ShaderResourceView* mSelectedObjectsRoughBySegmentsSRV = nullptr;

	uint32_t selectingSegmentsCount = 0;

public:
	void setSelectingSegmentsFineBySegments();
	void setSelectedObjectsFineBySegments();

	void setSelectingSegmentsCountFineBySegments(uint32_t count);
	void setObjectsCountFineBySegments(uint32_t count);

	void setVerticesFineBySegments();
	void setIndicesFineBySegments();
	void setObjectsInfoFineBySegments();

	void setDistancesToClosestObjects();
	void setClosestObjects();

	void initDistancesToClosestObjects();
	void initClosestObjects();

	void findSelectedObjectsFineBySegments();
	void getSelectedObjectsFineBySegments(uint32_t objects[], uint32_t count);
private:
	void initFineObjectsSelectionBySegments();

	ID3DX11Effect* mFineObjectsSelectionBySegmentsFX = nullptr;
	ID3DX11EffectTechnique* mFineObjectsSelectionBySegmentsTech = nullptr;

	ID3DX11EffectShaderResourceVariable* mSelectingSegmentsFineBySegments = nullptr;
	ID3DX11EffectShaderResourceVariable* mSelectedObjectsFineBySegments = nullptr;

	ID3DX11EffectVariable* mSelectingSegmentsCountFineBySegments = nullptr;
	ID3DX11EffectVariable* mObjectsCountFineBySegments = nullptr;

	ID3DX11EffectShaderResourceVariable* mVerticesFineBySegments = nullptr;
	ID3DX11EffectShaderResourceVariable* mIndicesFineBySegments = nullptr;
	ID3DX11EffectShaderResourceVariable* mObjectsInfoFineBySegments = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* mDistancesToClosestObjects = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mClosestObjects = nullptr;

	ID3D11Buffer* mDistancesToClosestObjectsBuffer = nullptr;
	ID3D11UnorderedAccessView* mDistancesToClosestObjectsUAV = nullptr;

	ID3D11Buffer* mClosestObjectsBuffer = nullptr;
	ID3D11UnorderedAccessView* mClosestObjectsUAV = nullptr;
	ID3D11Buffer* mOutputClosestObjectsBuffer = nullptr;

public:
	void openTextureA(const std::string& path);
	void openTextureB(const std::string& path);

	void interpolateTextureA();

private:
	void initTextureInterpolation();
	ID3DX11Effect* mTextureInterpolationFX = nullptr;
	ID3DX11EffectTechnique* mTextureInterpolationTech = nullptr;

	ID3DX11EffectShaderResourceVariable* mTextureBeforeInterpolating = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mTextureAfterInterpolating = nullptr;

	ID3D11ShaderResourceView* mTextureBeforeInterpolatingAsrv = nullptr;
	ID3D11ShaderResourceView* mTextureBeforeInterpolatingBsrv = nullptr;

	ID3D11UnorderedAccessView* mTextureAfterInterpolatingAuav = nullptr;
	ID3D11UnorderedAccessView* mTextureAfterInterpolatingBuav = nullptr;

	ID3D11ShaderResourceView* mTextureAfterInterpolatingAsrv = nullptr;
	ID3D11ShaderResourceView* mTextureAfterInterpolatingBsrv = nullptr;

	ID3D11Texture2D* mTextureAfterInterpolatingAtex = nullptr;
	ID3D11Texture2D* mTextureAfterInterpolatingBtex = nullptr;

	uint32_t widthOfA = 0;
	uint32_t heightOfA = 0;

	uint32_t widthOfB = 0;
	uint32_t heightOfB = 0;

private:
	void initDefinitionOfTheSamePoints();
	ID3DX11Effect* mDefinitionOfTheSamePointsFX = nullptr;
	ID3DX11EffectTechnique* mDefineTheSamePointsOnTwoImagesTech = nullptr;

	ID3DX11EffectShaderResourceVariable* mImageA = nullptr;
	ID3DX11EffectShaderResourceVariable* mImageB = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* mMapAtoB = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mErrorOfTheSamePointsDefinition = nullptr;

	ID3D11Texture2D* mMapAtoBtex = nullptr;

private:
	bool initWindow(HINSTANCE instanceHandle, int show, WNDPROC WndProc);

	const flt4x4* getFloat4x4(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const flt4* getFloat4(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const flt3* getFloat3(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const flt2* getFloat2(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const float* getFloat1(Mesh& mesh, string_id var, const VariableLocation& location) const;
	void* getStruct(Mesh& mesh, string_id var, const StructResource& sr, int* bytes = nullptr);
	
	ID3D11Buffer* getVertexBuffer(
		const Mesh& mesh,
		uint32_t* elementSize,
		string_id* technique = nullptr,
		string_id* pass = nullptr,
		bool structured = false
		) const;
	void* fetchVerticesFromMesh(
		const Mesh& mesh,
		const std::vector<InputLayoutResource::StreamInfo>* streamsInfo
		) const;

	ID3D11Buffer* getIndexBuffer(
		const Mesh& mesh,
		string_id* technique = nullptr,
		string_id* pass = nullptr,
		bool structured = false
	) const;

	ID3D11ShaderResourceView* getVertexBufferSRV(
		const Mesh& mesh,
		string_id* technique = nullptr,
		string_id* pass = nullptr
	) const;

	ID3D11ShaderResourceView* getIndexBufferSRV(
		const Mesh& mesh,
		string_id* technique = nullptr,
		string_id* pass = nullptr
	) const;

	ID3D11ShaderResourceView* getImage(Mesh& mesh, string_id var) const;
	ID3D11ShaderResourceView* getImagesArray(Mesh& mesh, string_id var) const;

	void setFloat4x4sOnGPU(Mesh& mesh);
	void setFloat4sOnGPU(Mesh& mesh);
	void setFloat3sOnGPU(Mesh& mesh);
	void setFloat2sOnGPU(Mesh& mesh);
	void setFloat1sOnGPU(Mesh& mesh);
	void setStructsOnGPU(Mesh& mesh);
	void setTexturesOnGPU(Mesh& mesh);
	void setTexturesArraysOnGPU(Mesh& mesh);

	void setVariablesOnGPU(Mesh& mesh);

	void setGeometryOnGPU(Mesh& mesh);

	ID3D11BlendState* getBlendState(Mesh& mesh);
	void setBlendState(Mesh& mesh);
	void setPipelineStates(Mesh& mesh);

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

	int param_index = 0;

private:
	static GraphicsCore* pGraphicsCore;
};