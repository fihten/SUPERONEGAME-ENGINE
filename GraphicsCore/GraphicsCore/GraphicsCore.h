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
#define INTEGRALS_ALONG_RADIUS 36
#define INTEGRALS_ALONG_SECTORS 3
#define INTEGRALS INTEGRALS_ALONG_RADIUS * INTEGRALS_ALONG_SECTORS

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

	void draw(Mesh& mesh, bool forceUpdatingOfGeometryOnGPU = false);
	
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
	int getWidthOfTextureA();
	int getHeightOfTextureA();

	void openTextureB(const std::string& path);
	int getWidthOfTextureB();
	int getHeightOfTextureB();

	int getRadiusOfBiggestAreaOfIntegration();

	void calculateIntegralsOfTextureA();
	void calculateIntegralsOfTextureB(float angle0, float scale0, float angle1, float scale1);

private:
	void initCalculationOfTextureIntegrals();
	ID3DX11Effect* mCalculationOfTextureIntegralsFX = nullptr;
	ID3DX11EffectTechnique* mCalculationOfTextureIntegralsTech = nullptr;

	ID3DX11EffectShaderResourceVariable* mTextureToIntegrate = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mIntegrals = nullptr;

	ID3DX11EffectVariable* mRadius0 = nullptr;
	ID3DX11EffectVariable* mRadius1 = nullptr;

	ID3DX11EffectVariable* mXleft = nullptr;
	ID3DX11EffectVariable* mXright = nullptr;

	ID3DX11EffectVariable* mYbottom = nullptr;
	ID3DX11EffectVariable* mYtop = nullptr;

	ID3DX11EffectVariable* mAngle0 = nullptr;
	ID3DX11EffectVariable* mScale0 = nullptr;

	ID3DX11EffectVariable* mAngle1 = nullptr;
	ID3DX11EffectVariable* mScale1 = nullptr;

	ID3D11ShaderResourceView* mTextureToIntegrateAsrv = nullptr;
	ID3D11ShaderResourceView* mTextureToIntegrateBsrv = nullptr;

	ID3D11UnorderedAccessView* mIntegralsAuav = nullptr;
	ID3D11UnorderedAccessView* mIntegralsBuav = nullptr;

	ID3D11ShaderResourceView* mIntegralsAsrv = nullptr;
	ID3D11ShaderResourceView* mIntegralsBsrv = nullptr;

	ID3D11Texture2D* mIntegralsAtex = nullptr;
	ID3D11Texture2D* mIntegralsBtex = nullptr;

	uint32_t widthOfA = 0;
	uint32_t heightOfA = 0;

	uint32_t widthOfB = 0;
	uint32_t heightOfB = 0;

	int radius0 = 50;
	int radius1 = std::sqrt(INTEGRALS_ALONG_RADIUS) * radius0;
	int sectors = INTEGRALS_ALONG_SECTORS;

public:
	void defineTheSamePoints();
	bool defineTheSamePoints(int axis0_x, int axis0_y, int axis1_x, int axis1_y);
	void defineTheSamePoints(
		Vec2d<int>& originInA, Vec2d<int>& domainSizeInA,
		Vec2d<int>& originInB, Vec2d<int>& domainSizeInB
	);
	flt2 mapAtoB(flt2& uvA);

private:
	void initDefinitionOfTheSamePoints();
	ID3DX11Effect* mDefinitionOfTheSamePointsFX = nullptr;
	ID3DX11EffectTechnique* mDefineTheSamePointsOnTwoImagesTech = nullptr;

	ID3DX11EffectShaderResourceVariable* mIntegralsOfA = nullptr;
	ID3DX11EffectVariable* mOriginInA = nullptr;
	ID3DX11EffectVariable* mDomainSizeInA = nullptr;

	ID3DX11EffectShaderResourceVariable* mIntegralsOfB = nullptr;
	ID3DX11EffectVariable* mOriginInB = nullptr;
	ID3DX11EffectVariable* mDomainSizeInB = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* mMapAtoB = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mErrorOfTheSamePointsDefinition = nullptr;

	ID3D11Texture2D* mMapAtoBtex = nullptr;
	ID3D11UnorderedAccessView* mMapAtoBuav = nullptr;
	ID3D11Texture2D* mMapAtoBtexCopy = nullptr;

	ID3D11Texture2D* mErrorOfTheSamePointsDefinitionTex = nullptr;
	ID3D11UnorderedAccessView* mErrorOfTheSamePointsDefinitionUAV = nullptr;

	float maxAngle = 60.0f * M_PI / 180.0f;
	float minScale = 0.5f;
	float maxScale = 1.5f;
	int positions_number = 0;
	int discrete_radius = 10;
	
	Vec2d<int> domainSizeInA = Vec2d<int>(64, 64);
	Vec2d<int> domainSizeInB = Vec2d<int>(64, 64);

public:
	void calculateIntegralsAtTwoPointsOfAandB(
		float uA, float vA,
		float uB, float vB,
		float& angle0, float& scale0,
		float& angle1, float& scale1
	);

private:
	void initManualDefinitionOfTheSamePoint();
	ID3DX11Effect* mCalculateIntegralsAtTexturePointFX = nullptr;

	ID3DX11EffectTechnique* mCalculateIntegralsAtTexturePointTech = nullptr;
	ID3DX11EffectTechnique* mCalculateVarianceWithinAreaOfIntegrationTech = nullptr;

	ID3DX11EffectShaderResourceVariable* mTextureMDSP = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mIntegralsMDSP = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mVariancesMDSP = nullptr;

	ID3DX11EffectVariable* mRadius0MDSP = nullptr;
	ID3DX11EffectVariable* mRadius1MDSP = nullptr;

	ID3DX11EffectVariable* mSectorsMDSP = nullptr;

	ID3DX11EffectVariable* mLeftXMDSP = nullptr;
	ID3DX11EffectVariable* mRightXMDSP = nullptr;

	ID3DX11EffectVariable* mBottomYMDSP = nullptr;
	ID3DX11EffectVariable* mTopYMDSP = nullptr;

	ID3DX11EffectVariable* mX0MDSP = nullptr;
	ID3DX11EffectVariable* mY0MDSP = nullptr;

	ID3DX11EffectVariable* mAngle0MDSP = nullptr;
	ID3DX11EffectVariable* mScale0MDSP = nullptr;

	ID3DX11EffectVariable* mAngle1MDSP = nullptr;
	ID3DX11EffectVariable* mScale1MDSP = nullptr;

	ID3D11Buffer* mIntegralsBufferMDSP = nullptr;
	ID3D11Buffer* mIntegralsBufferCopyMDSP = nullptr;
	ID3D11UnorderedAccessView* mIntegralsBufferMDSPuav = nullptr;

	ID3D11Buffer* mVariancesBufferMDSP = nullptr;
	ID3D11Buffer* mVariancesBufferCopyMDSP = nullptr;
	ID3D11UnorderedAccessView* mVariancesBufferMDSPuav = nullptr;

	void calculateIntegralsAtTexturePoint(
		ID3D11ShaderResourceView* texSRV,
		int x, int y,
		float angle0, float scale0,
		float angle1, float scale1,
		float integrals[],
		float variances[]
	);

	flt2 leastSquaresMethode(
		float integralsA[],
		float integralsB[]
	);
	
	void findDistributionOfErrors(
		float integralsA[],
		float integralsB[],
		float distribution[],
		int N
	);
	float distanceFromNormalDistribution(float distribution[], int N);

public:
	void calculateStatisticOfTextureAtPoint(const Vec2d<int>& pt, int radius, int y[], int& N);

private:
	void initCalculationOfTextureStatistic();
	ID3DX11Effect* mCalculateStatisticOfTextureFX = nullptr;
	ID3DX11EffectTechnique* mCalculateStatisticOfTextureTech = nullptr;

	ID3DX11EffectShaderResourceVariable* mTextureCSOT = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mStatisticCSOT = nullptr;

	ID3DX11EffectVariable* mNCSOT = nullptr;
	ID3DX11EffectVariable* mWidthCSOT = nullptr;
	ID3DX11EffectVariable* mHeightCSOT = nullptr;
	ID3DX11EffectVariable* mRadiusCSOT = nullptr;
	ID3DX11EffectVariable* mR0CSOT = nullptr;

	ID3D11Buffer* mStatisticBuffer = nullptr;
	ID3D11Buffer* mStatisticBufferCopy = nullptr;

	ID3D11UnorderedAccessView* mStatisticBufferUAV = nullptr;

	int NCSOT = 50;

private:
	bool initWindow(HINSTANCE instanceHandle, int show, WNDPROC WndProc);

	const flt4x4* getFloat4x4(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const flt4* getFloat4(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const flt3* getFloat3(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const flt2* getFloat2(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const float* getFloat1(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const Vec2d<int>* getInt2(Mesh& mesh, string_id var, const VariableLocation& location) const;
	const int* getInt1(Mesh& mesh, string_id var, const VariableLocation& location) const;
	void* getStruct(Mesh& mesh, string_id var, const StructResource& sr, int* bytes = nullptr);
	
	ID3D11Buffer* getVertexBuffer(
		const Mesh& mesh,
		uint32_t* elementSize,
		string_id* technique = nullptr,
		string_id* pass = nullptr,
		bool structured = false,
		bool forceUpdatingOfGeometryOnGPU = false
		) const;
	void* fetchVerticesFromMesh(
		const Mesh& mesh,
		const std::vector<InputLayoutResource::StreamInfo>* streamsInfo
		) const;

	ID3D11Buffer* getIndexBuffer(
		const Mesh& mesh,
		string_id* technique = nullptr,
		string_id* pass = nullptr,
		bool structured = false,
		bool forceUpdatingOfGeometryOnGPU = false
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
	void setInt2sOnGPU(Mesh& mesh);
	void setInt1sOnGPU(Mesh& mesh);
	void setStructsOnGPU(Mesh& mesh);
	void setTexturesOnGPU(Mesh& mesh);
	void setTexturesArraysOnGPU(Mesh& mesh);

	void setVariablesOnGPU(Mesh& mesh);

	void setGeometryOnGPU(Mesh& mesh, bool forceUpdatingOfGeometryOnGPU = false);

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