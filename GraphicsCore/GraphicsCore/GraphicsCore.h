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
#define INTEGRALS_ALONG_SECTORS 6
#define INTEGRALS INTEGRALS_ALONG_RADIUS * INTEGRALS_ALONG_SECTORS

class GraphicsCore;
class ModelMaker;
struct GridIntegrals;
struct OperationsOnGridIntegrals;
struct LeastSquaresOfJacobianDeterminant;

typedef void (*DRAW_FUNC)(GraphicsCore*);
typedef HWND(*WND_CREATOR)(HINSTANCE instanceHandle, int width, int height, int show, WNDPROC WndProc);
typedef void(*INTEGRAL_MODIFIER)(float fIntegrals[3], uint32_t uiIntegrals[4]);

class SelectedObjectVisitor
{
public:
	virtual void operator()(uint32_t objectID) = 0;
};

class GraphicsCore
{
	friend class ModelMaker;
	friend struct GridIntegralsA;
	friend struct GridIntegralsB;
	friend struct OperationsOnGridIntegrals;
	friend struct LeastSquaresOfJacobianDeterminant;
	friend struct FindNearestDefinedPoint;
	friend struct TransformTo3dVertices;

	ID3D11Device* device = 0;
	ID3D11DeviceContext* context = 0;

	GraphicsCore() {}

public:
	static GraphicsCore* instance();

	void init(HINSTANCE instanceHandle, int show, WNDPROC WndProc, WND_CREATOR windowCreator, DRAW_FUNC drawFunc, UINT width, UINT height, bool windowed, bool enable4xMsaa);

	void startFrame();
	void endFrame();

	void draw(Mesh& mesh, bool forceUpdatingOfGeometryOnGPU = false);
	
	int run();
	void resize(UINT width, UINT height);

public:
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
	ID3D11UnorderedAccessView* mBoundingSpheresBufferUAV = nullptr;

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
	void setClosestTriangles();

	void initDistancesToClosestObjects();
	void initClosestObjects();
	void initClosestTriangles();

	void findSelectedObjectsFineBySegments(bool findTriangles);
	void getSelectedObjectsFineBySegments(uint32_t objects[], uint32_t count);
	void getSelectedObjectsTrianglesFineBySegments(uint32_t triangles[], uint32_t count);
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
	ID3DX11EffectUnorderedAccessViewVariable* mClosestTriangles = nullptr;

	ID3D11Buffer* mDistancesToClosestObjectsBuffer = nullptr;
	ID3D11UnorderedAccessView* mDistancesToClosestObjectsUAV = nullptr;

	ID3D11Buffer* mClosestObjectsBuffer = nullptr;
	ID3D11UnorderedAccessView* mClosestObjectsUAV = nullptr;
	ID3D11Buffer* mOutputClosestObjectsBuffer = nullptr;

	ID3D11Buffer* mClosestTrianglesBuffer = nullptr;
	ID3D11UnorderedAccessView* mClosestTrianglesUAV = nullptr;
	ID3D11Buffer* mOutputClosestTrianglesBuffer = nullptr;

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
	int radius1;// = ((2 * RADIUS_IN_CELLS + 1) * CELL_DIMENSION_X) / 2;
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

	float maxAngle = 15.0f * M_PI / 180.0f;
	float minScale = 0.9f;
	float maxScale = 1.1f;
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

	ID3DX11EffectShaderResourceVariable* mTextureMDSP = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mIntegralsMDSP = nullptr;

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

	ID3DX11EffectVariable* mCellDimensionX = nullptr;
	ID3DX11EffectVariable* mCellDimensionY = nullptr;
	ID3DX11EffectVariable* mRadius = nullptr;

	ID3D11Buffer* mIntegralsBufferMDSP = nullptr;
	ID3D11Buffer* mIntegralsBufferCopyMDSP = nullptr;
	ID3D11UnorderedAccessView* mIntegralsBufferMDSPuav = nullptr;

	template<typename INTEGRAL_MODIFIER>
	void calculateIntegralsAtTexturePoint(
		ID3D11ShaderResourceView* texSRV,
		int x, int y,
		float angle0, float scale0,
		float angle1, float scale1,
		float integrals[],
		INTEGRAL_MODIFIER modifier
	);

	template<typename INTEGRAL_MODIFIER>
	void calculateIntegralsAtTexturePoint(
		ID3D11ShaderResourceView* texSRV,
		int x, int y,
		float angle0, float scale0,
		float angle1, float scale1,
		int sectors, int sectorElements,
		float integrals[],
		INTEGRAL_MODIFIER modifier 
	);

	flt2 leastSquaresMethode(
		float integralsA[],
		float integralsB[],
		int count = INTEGRALS
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

public:
	void calculateFourierCoefficientsAtCosForRChanel(
		const Vec2d<int>& pt, int radius, int thickness, float coeffs[], int& N);
	void calculateFourierCoefficientsAtCosForGChanel(
		const Vec2d<int>& pt, int radius, int thickness, float coeffs[], int& N);
	void calculateFourierCoefficientsAtCosForBChanel(
		const Vec2d<int>& pt, int radius, int thickness, float coeffs[], int& N);

	void calculateFourierCoefficientsAtSinForRChanel(
		const Vec2d<int>& pt, int radius, int thickness, float coeffs[], int& N);
	void calculateFourierCoefficientsAtSinForGChanel(
		const Vec2d<int>& pt, int radius, int thickness, float coeffs[], int& N);
	void calculateFourierCoefficientsAtSinForBChanel(
		const Vec2d<int>& pt, int radius, int thickness, float coeffs[], int& N);
private:
	void initFourierTransform();
	ID3DX11Effect* mFourierTransformFX = nullptr;

	ID3DX11EffectTechnique* mFourierTransformCosR_TECH = nullptr;
	ID3DX11EffectTechnique* mFourierTransformCosG_TECH = nullptr;
	ID3DX11EffectTechnique* mFourierTransformCosB_TECH = nullptr;

	ID3DX11EffectTechnique* mFourierTransformSinR_TECH = nullptr;
	ID3DX11EffectTechnique* mFourierTransformSinG_TECH = nullptr;
	ID3DX11EffectTechnique* mFourierTransformSinB_TECH = nullptr;

	ID3DX11EffectShaderResourceVariable* mTextureForFourierTransform = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* mFourierCoefficientsAtCos_R = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mFourierCoefficientsAtCos_G = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mFourierCoefficientsAtCos_B = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* mFourierCoefficientsAtSin_R = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mFourierCoefficientsAtSin_G = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* mFourierCoefficientsAtSin_B = nullptr;

	ID3DX11EffectVariable* mRadiusFourierTransform = nullptr;
	ID3DX11EffectVariable* mThicknessFourierTransform = nullptr;
	ID3DX11EffectVariable* mWidthFourierTransform = nullptr;
	ID3DX11EffectVariable* mHeightFourierTransform = nullptr;
	ID3DX11EffectVariable* mNumberOfElementsInFourierRow = nullptr;
	ID3DX11EffectVariable* mR0FourierTransform = nullptr;

	ID3D11Buffer* mFourierCoefficientsAtCos_R_buffer = nullptr;
	ID3D11Buffer* mFourierCoefficientsAtCos_R_bufferCopy = nullptr;
	ID3D11UnorderedAccessView* mFourierCoefficientsAtCos_R_uav = nullptr;

	ID3D11Buffer* mFourierCoefficientsAtCos_G_buffer = nullptr;
	ID3D11Buffer* mFourierCoefficientsAtCos_G_bufferCopy = nullptr;
	ID3D11UnorderedAccessView* mFourierCoefficientsAtCos_G_uav = nullptr;

	ID3D11Buffer* mFourierCoefficientsAtCos_B_buffer = nullptr;
	ID3D11Buffer* mFourierCoefficientsAtCos_B_bufferCopy = nullptr;
	ID3D11UnorderedAccessView* mFourierCoefficientsAtCos_B_uav = nullptr;

	ID3D11Buffer* mFourierCoefficientsAtSin_R_buffer = nullptr;
	ID3D11Buffer* mFourierCoefficientsAtSin_R_bufferCopy = nullptr;
	ID3D11UnorderedAccessView* mFourierCoefficientsAtSin_R_uav = nullptr;

	ID3D11Buffer* mFourierCoefficientsAtSin_G_buffer = nullptr;
	ID3D11Buffer* mFourierCoefficientsAtSin_G_bufferCopy = nullptr;
	ID3D11UnorderedAccessView* mFourierCoefficientsAtSin_G_uav = nullptr;

	ID3D11Buffer* mFourierCoefficientsAtSin_B_buffer = nullptr;
	ID3D11Buffer* mFourierCoefficientsAtSin_B_bufferCopy = nullptr;
	ID3D11UnorderedAccessView* mFourierCoefficientsAtSin_B_uav = nullptr;

	const int numberOfElementsInFourierRow = 1000;

	private:
		struct BoundingSpheresCalculation
		{
			ID3DX11Effect* mCalculateSpheresFX = nullptr;
			ID3DX11EffectTechnique* mClearRadiusTech = nullptr;
			ID3DX11EffectTechnique* mCalculateRadiusTech = nullptr;
			ID3DX11EffectTechnique* mCalculateRadius_checkSelectionStatusTech = nullptr;
			ID3DX11EffectTechnique* mCalculateSpheresTech = nullptr;
			ID3DX11EffectTechnique* mCalculateSpheres_checkSelectionStatusTech = nullptr;

			ID3DX11EffectUnorderedAccessViewVariable* mBoundingSpheresUavH = nullptr;
			ID3DX11EffectUnorderedAccessViewVariable* mRadiusesUavH = nullptr;

			ID3DX11EffectVariable* mSpheresCountH = nullptr;
			ID3DX11EffectVariable* mConvertToUINTH = nullptr;

			ID3DX11EffectShaderResourceVariable* mVerticesSrvH = nullptr;
			ID3DX11EffectShaderResourceVariable* mObjectsInfoSrvH = nullptr;
			ID3DX11EffectVariable* mObjectsCountH = nullptr;

			ID3DX11EffectShaderResourceVariable* mRoughlySelectedObjectsSrvH = nullptr;

			ID3D11Buffer* mRadiuses = nullptr;
			ID3D11UnorderedAccessView* mRadiusesUav = nullptr;

			float mConvertToUINT = 1000000.0f;
		};
		BoundingSpheresCalculation boundingSpheresCalculation;
		void initBoundingSpheresCalculation();
public:
		void calculateBoundingSpheres(bool bCheckSelectionStatus);

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

	ID3D11DepthStencilState* getDepthStencilState(Mesh& mesh);
	void setDepthStencilState(Mesh& mesh);

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