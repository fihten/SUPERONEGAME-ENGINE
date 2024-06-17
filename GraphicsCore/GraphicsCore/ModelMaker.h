/*

Code of ModelMaker was stolen from company 
that produces flight simulator.

	P.S.: This is joke
		P.S.: No

*/

#pragma once

#include "Vec2d.h"
#include "Vec4d.h"

#include <d3dx11effect.h>
#include <memory>
#include <vector>
#include <string>

#define CELL_DIMENSION_X 32 // for spheres this is 36
#define CELL_DIMENSION_Y 32 // for spheres this is 36

#define OFFSET_RANGE_X 256
#define OFFSET_RANGE_Y 256

#define OFFSET0_X 128
#define OFFSET0_Y 128

#define RADIUS_IN_CELLS 4 

struct GridIntegralsA
{
	ID3DX11Effect* hGridIntegralsFX = nullptr;
	ID3DX11EffectTechnique* hClearGridIntegrals = nullptr;
	ID3DX11EffectTechnique* hCalculateGridIntegrals = nullptr;

	ID3DX11EffectShaderResourceVariable* hPhotos = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsXho = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsYho = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsZho = nullptr;

	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsXhi = nullptr;
	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsYhi = nullptr;
	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsZhi = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsX = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsY = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsZ = nullptr;

	ID3DX11EffectVariable* hWidth = nullptr;
	ID3DX11EffectVariable* hHeight = nullptr;
	ID3DX11EffectVariable* hTexturesCount = nullptr;

	ID3DX11EffectVariable* hWidthA = nullptr;
	ID3DX11EffectVariable* hHeightA = nullptr;

	ID3DX11EffectVariable* hCellDimensionX = nullptr;
	ID3DX11EffectVariable* hCellDimensionY = nullptr;

	void unboundViews();

public:
	void init();
	void clearPhotosIntegrals(
		ID3D11UnorderedAccessView* photosIntegralsXh,
		ID3D11UnorderedAccessView* photosIntegralsYh,
		ID3D11UnorderedAccessView* photosIntegralsZh,
		ID3D11UnorderedAccessView* photosIntegralsX,
		ID3D11UnorderedAccessView* photosIntegralsY,
		ID3D11UnorderedAccessView* photosIntegralsZ,
		int widthA, int heightA, int textureCount,
		int height
	);
	void calculatePhotosIntegrals(
		ID3D11ShaderResourceView* photos,
		ID3D11UnorderedAccessView* photosIntegralsXho,
		ID3D11UnorderedAccessView* photosIntegralsYho,
		ID3D11UnorderedAccessView* photosIntegralsZho,
		ID3D11ShaderResourceView* photosIntegralsXhi,
		ID3D11ShaderResourceView* photosIntegralsYhi,
		ID3D11ShaderResourceView* photosIntegralsZhi,
		ID3D11UnorderedAccessView* photosIntegralsX,
		ID3D11UnorderedAccessView* photosIntegralsY,
		ID3D11UnorderedAccessView* photosIntegralsZ,
		int width, int height, int textureCount,
		int widthA, int HeightA
	);
};

struct GridIntegralsB
{
	ID3DX11Effect* hGridIntegralsFX = nullptr;
	ID3DX11EffectTechnique* hClearGridIntegralsTech = nullptr;
	ID3DX11EffectTechnique* hCalculateGridIntegralsTech = nullptr;

	ID3DX11EffectShaderResourceVariable* hPhotos = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsX = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsY = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegralsZ = nullptr;

	ID3DX11EffectVariable* hWidth = nullptr;
	ID3DX11EffectVariable* hHeight = nullptr;
	ID3DX11EffectVariable* hTexturesCount = nullptr;

	ID3DX11EffectVariable* hWidthB = nullptr;
	ID3DX11EffectVariable* hHeightB = nullptr;

	ID3DX11EffectVariable* hAngle0 = nullptr;
	ID3DX11EffectVariable* hScale0 = nullptr;

	ID3DX11EffectVariable* hAngle1 = nullptr;
	ID3DX11EffectVariable* hScale1 = nullptr;

	ID3DX11EffectVariable* hCellDimensionX = nullptr;
	ID3DX11EffectVariable* hCellDimensionY = nullptr;

	void unboundViews();

public:
	void init();
	void clearPhotosIntegrals(
		ID3D11UnorderedAccessView* photosIntegralsX,
		ID3D11UnorderedAccessView* photosIntegralsY,
		ID3D11UnorderedAccessView* photosIntegralsZ,
		int widthB, int heightB, int count
	);
	void calculatePhotosIntegrals(
		ID3D11ShaderResourceView* photos,
		ID3D11UnorderedAccessView* photosIntegralsX,
		ID3D11UnorderedAccessView* photosIntegralsY,
		ID3D11UnorderedAccessView* photosIntegralsZ,
		int width, int height, int count,
		int widthB, int heightB,
		float angle0, float scale0,
		float angle1, float scale1
	);
};

struct OperationsOnGridIntegrals
{
	ID3DX11Effect* hOperationsOnGridIntegralsFX = nullptr;
	ID3DX11EffectTechnique* hMakeOperationsOnGridIntegralsTech = nullptr;

	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsAx = nullptr;
	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsAy = nullptr;
	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsAz = nullptr;

	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsBx = nullptr;
	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsBy = nullptr;
	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsBz = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hAA = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hAB = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hBB = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hAAfraction = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hABfraction = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hBBfraction = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hMaxA = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hMaxB = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hAAho = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hAAfractionHo = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hMaxAho = nullptr;

	ID3DX11EffectShaderResourceVariable* hAAhi = nullptr;
	ID3DX11EffectShaderResourceVariable* hAAfractionHi = nullptr;
	ID3DX11EffectShaderResourceVariable* hMaxAhi = nullptr;

	ID3DX11EffectVariable* hWidthAreal = nullptr;
	ID3DX11EffectVariable* hHeightAreal = nullptr;

	ID3DX11EffectVariable* hWidthB = nullptr;
	ID3DX11EffectVariable* hHeightB = nullptr;

	ID3DX11EffectVariable* hWidthAA = nullptr;
	ID3DX11EffectVariable* hWidthAB = nullptr;
	ID3DX11EffectVariable* hWidthABreal = nullptr;
	ID3DX11EffectVariable* hWidthBB = nullptr;

	ID3DX11EffectVariable* hHeightAA = nullptr;
	ID3DX11EffectVariable* hHeightAB = nullptr;
	ID3DX11EffectVariable* hHeightABreal = nullptr;
	ID3DX11EffectVariable* hHeightBB = nullptr;

	ID3DX11EffectVariable* hTexturesCount = nullptr;

	ID3DX11EffectVariable* hCellDimension = nullptr;
	ID3DX11EffectVariable* hOffsetRange = nullptr;
	ID3DX11EffectVariable* hOffset0 = nullptr;

	ID3DX11EffectVariable* hRadius = nullptr;
	ID3DX11EffectVariable* hIndexOfA = nullptr;
public:
	void init();
	void unboundViews();
	void clearAAandMaxA(
		ID3D11UnorderedAccessView* AA,
		ID3D11UnorderedAccessView* AAfraction,
		ID3D11UnorderedAccessView* maxA,
		ID3D11UnorderedAccessView* AAh,
		ID3D11UnorderedAccessView* AAfractionH,
		ID3D11UnorderedAccessView* maxAh,
		int widthAA, int heightAA, int texturesCount,
		int heightAreal
	);
	void clearBBandMaxB(
		ID3D11UnorderedAccessView* BB,
		ID3D11UnorderedAccessView* BBfraction,
		ID3D11UnorderedAccessView* maxB,
		int widthBB, int heightBB, int texturesCount
	);
	void clearAB(
		ID3D11UnorderedAccessView* AB,
		ID3D11UnorderedAccessView* ABfraction,
		int widthAB, int heightAB, int texturesCount,
		int widthABreal, int heightABreal
	);
	void calculateAAandMaxA(
		ID3D11ShaderResourceView* photosIntegralsAx,
		ID3D11ShaderResourceView* photosIntegralsAy,
		ID3D11ShaderResourceView* photosIntegralsAz,
		ID3D11ShaderResourceView* AAhi,
		ID3D11ShaderResourceView* AAfractionHi,
		ID3D11ShaderResourceView* maxAhi,
		ID3D11UnorderedAccessView* AA,
		ID3D11UnorderedAccessView* AAfraction,
		ID3D11UnorderedAccessView* maxA,
		ID3D11UnorderedAccessView* AAho,
		ID3D11UnorderedAccessView* AAfractionHo,
		ID3D11UnorderedAccessView* maxAho,
		int widthAreal, int heightAreal, int texturesCount,
		int widthAA, int heightAA
	);
	void calculateBBandMaxB(
		ID3D11ShaderResourceView* photosIntegralsBx,
		ID3D11ShaderResourceView* photosIntegralsBy,
		ID3D11ShaderResourceView* photosIntegralsBz,
		ID3D11UnorderedAccessView* BB,
		ID3D11UnorderedAccessView* BBfraction,
		ID3D11UnorderedAccessView* maxB,
		int widthB, int heightB, int texturesCount
	);
	void calculateAB(
		ID3D11ShaderResourceView* photosIntegralsAx,
		ID3D11ShaderResourceView* photosIntegralsAy,
		ID3D11ShaderResourceView* photosIntegralsAz,
		ID3D11ShaderResourceView* photosIntegralsBx,
		ID3D11ShaderResourceView* photosIntegralsBy,
		ID3D11ShaderResourceView* photosIntegralsBz,
		ID3D11UnorderedAccessView* AB,
		ID3D11UnorderedAccessView* ABfraction,
		int widthA, int heightA, int texturesCount,
		int widthAreal, int heightAreal,
		int widthAB, int heightAB,
		int widthABreal, int heightABreal, int indexOfA
	);
};

struct LeastSquaresOfJacobianDeterminant
{
	ID3DX11Effect* hLeastSquaresOfJacobianDeterminantFX = nullptr;
	ID3DX11EffectTechnique* hClearTech = nullptr;
	ID3DX11EffectTechnique* hApplyLeastSquareMethodTech = nullptr;

	ID3DX11EffectShaderResourceVariable* hAA = nullptr;
	ID3DX11EffectShaderResourceVariable* hAB = nullptr;
	ID3DX11EffectShaderResourceVariable* hBB = nullptr;

	ID3DX11EffectShaderResourceVariable* hAAfraction = nullptr;
	ID3DX11EffectShaderResourceVariable* hABfraction = nullptr;
	ID3DX11EffectShaderResourceVariable* hBBfraction = nullptr;

	ID3DX11EffectShaderResourceVariable* hMaxA = nullptr;
	ID3DX11EffectShaderResourceVariable* hMaxB = nullptr;

	ID3DX11EffectVariable* hWidth = nullptr;
	ID3DX11EffectVariable* hHeight = nullptr;
	ID3DX11EffectVariable* hWidthAA = nullptr;
	ID3DX11EffectVariable* hWidthAB = nullptr;
	ID3DX11EffectVariable* hWidthABreal = nullptr;
	ID3DX11EffectVariable* hWidthBB = nullptr;

	ID3DX11EffectVariable* hHeightAA = nullptr;
	ID3DX11EffectVariable* hHeightAB = nullptr;
	ID3DX11EffectVariable* hHeightABreal = nullptr;
	ID3DX11EffectVariable* hHeightBB = nullptr;

	ID3DX11EffectVariable* hTexturesCount = nullptr;

	ID3DX11EffectVariable* hAngle0 = nullptr;
	ID3DX11EffectVariable* hScale0 = nullptr;

	ID3DX11EffectVariable* hAngle1 = nullptr;
	ID3DX11EffectVariable* hScale1 = nullptr;

	ID3DX11EffectVariable* hRadius = nullptr;
	ID3DX11EffectVariable* hCellDimension = nullptr;
	ID3DX11EffectVariable* hOffsetRange = nullptr;
	ID3DX11EffectVariable* hOffset0 = nullptr;

	ID3DX11EffectVariable* hIndexOfA = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hError = nullptr;
	ID3DX11EffectShaderResourceVariable* hErrorIn = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hAtoBx = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hAtoBy = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hAtoBz = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hAtoBw = nullptr;
public:
	void init();
	void unboundViews();
	void clear(
		ID3D11UnorderedAccessView* error,
		ID3D11UnorderedAccessView* AtoBx,
		ID3D11UnorderedAccessView* AtoBy,
		ID3D11UnorderedAccessView* AtoBz,
		ID3D11UnorderedAccessView* AtoBw,
		int widthBB, int heightBB, int texturesCount
	);
	void calculateMapping(
		ID3D11ShaderResourceView* AA,
		ID3D11ShaderResourceView* AAfraction,
		ID3D11ShaderResourceView* AB,
		ID3D11ShaderResourceView* ABfraction,
		ID3D11ShaderResourceView* BB,
		ID3D11ShaderResourceView* BBfraction,
		ID3D11ShaderResourceView* maxA,
		ID3D11ShaderResourceView* maxB,
		ID3D11ShaderResourceView* errorIn,
		ID3D11UnorderedAccessView* error,
		ID3D11UnorderedAccessView* AtoBx,
		ID3D11UnorderedAccessView* AtoBy,
		ID3D11UnorderedAccessView* AtoBz,
		ID3D11UnorderedAccessView* AtoBw,
		int width, int height,
		int widthAB, int heightAB, int texturesCount,
		int indexOfA,
		int widthABreal, int heightABreal,
		int widthAA, int heightAA,
		float angle0, float scale0,
		float angle1, float scale1
	);
};

struct FindNearestDefinedPoint
{
	ID3DX11Effect* hEffect = nullptr;
	ID3DX11EffectTechnique* hTechnique = nullptr;

	ID3DX11EffectShaderResourceVariable* hError = nullptr;
	ID3DX11EffectShaderResourceVariable* hAtoBx = nullptr;
	ID3DX11EffectShaderResourceVariable* hAtoBy = nullptr;
	ID3DX11EffectShaderResourceVariable* hAtoBz = nullptr;
	ID3DX11EffectShaderResourceVariable* hAtoBw = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hMinDistanceOut = nullptr;
	ID3DX11EffectShaderResourceVariable* hMinDistanceIn = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hMappingOfPoint = nullptr;

	ID3DX11EffectVariable* hWidth = nullptr;
	ID3DX11EffectVariable* hHeight = nullptr;
	ID3DX11EffectVariable* hIndex = nullptr;

	ID3DX11EffectVariable* hPt = nullptr;
	ID3DX11EffectVariable* hThreshold = nullptr;

	ID3D11Buffer* minDistance = nullptr;
	ID3D11UnorderedAccessView* minDistanceUAV = nullptr;
	ID3D11ShaderResourceView* minDistanceSRV = nullptr;

	ID3D11Buffer* mappingOfPoint = nullptr;
	ID3D11Buffer* mappingOfPointCopy = nullptr;
	ID3D11UnorderedAccessView* mappingOfPointUAV = nullptr;

public:
	void init();
	Vec4d<int> findNearestPoint(
		ID3D11ShaderResourceView* error,
		ID3D11ShaderResourceView* AtoBx,
		ID3D11ShaderResourceView* AtoBy,
		ID3D11ShaderResourceView* AtoBz,
		ID3D11ShaderResourceView* AtoBw,
		const Vec2d<int>& point
	);
};

struct TransformTo3dVertices
{
	ID3DX11Effect* hEffect = nullptr;
	ID3DX11EffectTechnique* hTechnique = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* Rc_out = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* Ac_out = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* Bc_out = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* Ad_out = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* Bd_out = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* Cd_out = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* R_out = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* A_out = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* B_out = nullptr;

	ID3DX11EffectShaderResourceVariable* Rc = nullptr;
	ID3DX11EffectShaderResourceVariable* Ac = nullptr;
	ID3DX11EffectShaderResourceVariable* Bc = nullptr;

	ID3DX11EffectShaderResourceVariable* Ad = nullptr;
	ID3DX11EffectShaderResourceVariable* Bd = nullptr;
	ID3DX11EffectShaderResourceVariable* Cd = nullptr;

	ID3DX11EffectShaderResourceVariable* R = nullptr;
	ID3DX11EffectShaderResourceVariable* A = nullptr;
	ID3DX11EffectShaderResourceVariable* B = nullptr;

	ID3DX11EffectShaderResourceVariable* pointsOnPhotos = nullptr;
	ID3DX11EffectShaderResourceVariable* mapToVertexAndCamera = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* I = nullptr;
	ID3DX11EffectShaderResourceVariable* Iin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* J = nullptr;
	ID3DX11EffectShaderResourceVariable* Jin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* K = nullptr;
	ID3DX11EffectShaderResourceVariable* Kin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* xyzc = nullptr;
	ID3DX11EffectShaderResourceVariable* xyzc_in = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* xyz = nullptr;
	ID3DX11EffectShaderResourceVariable* xyz_in = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* error = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dIdA = nullptr;
	ID3DX11EffectShaderResourceVariable* dIdAin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dIdB = nullptr;
	ID3DX11EffectShaderResourceVariable* dIdBin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dIdC = nullptr;
	ID3DX11EffectShaderResourceVariable* dIdCin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dJdA = nullptr;
	ID3DX11EffectShaderResourceVariable* dJdAin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dJdB = nullptr;
	ID3DX11EffectShaderResourceVariable* dJdBin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dJdC = nullptr;
	ID3DX11EffectShaderResourceVariable* dJdCin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dKdA = nullptr;
	ID3DX11EffectShaderResourceVariable* dKdAin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dKdB = nullptr;
	ID3DX11EffectShaderResourceVariable* dKdBin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dKdC = nullptr;
	ID3DX11EffectShaderResourceVariable* dKdCin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dXYZCdR = nullptr;
	ID3DX11EffectShaderResourceVariable* dXYZCdRin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dXYZCdA = nullptr;
	ID3DX11EffectShaderResourceVariable* dXYZCdAin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dXYZCdB = nullptr;
	ID3DX11EffectShaderResourceVariable* dXYZCdBin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dXYZdR = nullptr;
	ID3DX11EffectShaderResourceVariable* dXYZdRin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dXYZdA = nullptr;
	ID3DX11EffectShaderResourceVariable* dXYZdAin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* dXYZdB = nullptr;
	ID3DX11EffectShaderResourceVariable* dXYZdBin = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* gradError_r = nullptr;
	ID3DX11EffectShaderResourceVariable* gradError_r_in = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* gradError_a = nullptr;
	ID3DX11EffectShaderResourceVariable* gradError_a_in = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* minGradComponent_a = nullptr;

	ID3DX11EffectVariable* amountOfCameras = nullptr;
	ID3DX11EffectVariable* amountOfVertices = nullptr;
	ID3DX11EffectVariable* amountOfPointsOnPhotos = nullptr;

	ID3DX11EffectVariable* t_r = nullptr;
	ID3DX11EffectVariable* t_a = nullptr;

public:
	ID3D11Buffer* Rc_buf = nullptr;
	ID3D11Buffer* Ac_buf = nullptr;
	ID3D11Buffer* Bc_buf = nullptr;

	ID3D11Buffer* Ad_buf = nullptr;
	ID3D11Buffer* Bd_buf = nullptr;
	ID3D11Buffer* Cd_buf = nullptr;

	ID3D11Buffer* R_buf = nullptr;
	ID3D11Buffer* A_buf = nullptr;
	ID3D11Buffer* B_buf = nullptr;

	ID3D11Buffer* pointsOnPhotos_buf = nullptr;
	ID3D11Buffer* mapToVertexAndCamera_buf = nullptr;

	ID3D11Buffer* I_buf = nullptr;
	ID3D11Buffer* J_buf = nullptr;
	ID3D11Buffer* K_buf = nullptr;

	ID3D11Buffer* xyzc_buf = nullptr;
	ID3D11Buffer* xyz_buf = nullptr;

	ID3D11Buffer* error_buf = nullptr;
	ID3D11Buffer* error_copy_buf = nullptr;

	ID3D11Buffer* dIdA_buf = nullptr;
	ID3D11Buffer* dIdB_buf = nullptr;
	ID3D11Buffer* dIdC_buf = nullptr;

	ID3D11Buffer* dJdA_buf = nullptr;
	ID3D11Buffer* dJdB_buf = nullptr;
	ID3D11Buffer* dJdC_buf = nullptr;

	ID3D11Buffer* dKdA_buf = nullptr;
	ID3D11Buffer* dKdB_buf = nullptr;
	ID3D11Buffer* dKdC_buf = nullptr;

	ID3D11Buffer* dXYZCdR_buf = nullptr;
	ID3D11Buffer* dXYZCdA_buf = nullptr;
	ID3D11Buffer* dXYZCdB_buf = nullptr;

	ID3D11Buffer* dXYZdR_buf = nullptr;
	ID3D11Buffer* dXYZdA_buf = nullptr;
	ID3D11Buffer* dXYZdB_buf = nullptr;

	ID3D11Buffer* gradError_r_buf = nullptr;
	ID3D11Buffer* gradError_a_buf = nullptr;

	ID3D11Buffer* minGradComponent_a_buf = nullptr;
	ID3D11Buffer* minGradComponent_a_copy_buf = nullptr;

public:
	ID3D11ShaderResourceView* Rc_srv = nullptr;
	ID3D11ShaderResourceView* Ac_srv = nullptr;
	ID3D11ShaderResourceView* Bc_srv = nullptr;

	ID3D11ShaderResourceView* Ad_srv = nullptr;
	ID3D11ShaderResourceView* Bd_srv = nullptr;
	ID3D11ShaderResourceView* Cd_srv = nullptr;

	ID3D11ShaderResourceView* R_srv = nullptr;
	ID3D11ShaderResourceView* A_srv = nullptr;
	ID3D11ShaderResourceView* B_srv = nullptr;

	ID3D11ShaderResourceView* pointsOnPhotos_srv = nullptr;
	ID3D11ShaderResourceView* mapToVertexAndCamera_srv = nullptr;

	ID3D11ShaderResourceView* I_srv = nullptr;
	ID3D11ShaderResourceView* J_srv = nullptr;
	ID3D11ShaderResourceView* K_srv = nullptr;

	ID3D11ShaderResourceView* dIdA_srv = nullptr;
	ID3D11ShaderResourceView* dIdB_srv = nullptr;
	ID3D11ShaderResourceView* dIdC_srv = nullptr;

	ID3D11ShaderResourceView* dJdA_srv = nullptr;
	ID3D11ShaderResourceView* dJdB_srv = nullptr;
	ID3D11ShaderResourceView* dJdC_srv = nullptr;

	ID3D11ShaderResourceView* dKdA_srv = nullptr;
	ID3D11ShaderResourceView* dKdB_srv = nullptr;
	ID3D11ShaderResourceView* dKdC_srv = nullptr;

	ID3D11ShaderResourceView* xyzc_srv = nullptr;
	ID3D11ShaderResourceView* dXYZCdR_srv = nullptr;
	ID3D11ShaderResourceView* dXYZCdA_srv = nullptr;
	ID3D11ShaderResourceView* dXYZCdB_srv = nullptr;

	ID3D11ShaderResourceView* xyz_srv = nullptr;
	ID3D11ShaderResourceView* dXYZdR_srv = nullptr;
	ID3D11ShaderResourceView* dXYZdA_srv = nullptr;
	ID3D11ShaderResourceView* dXYZdB_srv = nullptr;

	ID3D11ShaderResourceView* gradError_r_srv = nullptr;
	ID3D11ShaderResourceView* gradError_a_srv = nullptr;

public:
	ID3D11UnorderedAccessView* Rc_uav = nullptr;
	ID3D11UnorderedAccessView* Ac_uav = nullptr;
	ID3D11UnorderedAccessView* Bc_uav = nullptr;

	ID3D11UnorderedAccessView* Ad_uav = nullptr;
	ID3D11UnorderedAccessView* Bd_uav = nullptr;
	ID3D11UnorderedAccessView* Cd_uav = nullptr;

	ID3D11UnorderedAccessView* R_uav = nullptr;
	ID3D11UnorderedAccessView* A_uav = nullptr;
	ID3D11UnorderedAccessView* B_uav = nullptr;

	ID3D11UnorderedAccessView* I_uav = nullptr;
	ID3D11UnorderedAccessView* J_uav = nullptr;
	ID3D11UnorderedAccessView* K_uav = nullptr;

	ID3D11UnorderedAccessView* dIdA_uav = nullptr;
	ID3D11UnorderedAccessView* dIdB_uav = nullptr;
	ID3D11UnorderedAccessView* dIdC_uav = nullptr;

	ID3D11UnorderedAccessView* dJdA_uav = nullptr;
	ID3D11UnorderedAccessView* dJdB_uav = nullptr;
	ID3D11UnorderedAccessView* dJdC_uav = nullptr;

	ID3D11UnorderedAccessView* dKdA_uav = nullptr;
	ID3D11UnorderedAccessView* dKdB_uav = nullptr;
	ID3D11UnorderedAccessView* dKdC_uav = nullptr;

	ID3D11UnorderedAccessView* xyzc_uav = nullptr;
	ID3D11UnorderedAccessView* dXYZCdR_uav = nullptr;
	ID3D11UnorderedAccessView* dXYZCdA_uav = nullptr;
	ID3D11UnorderedAccessView* dXYZCdB_uav = nullptr;

public:
	int amountOfCameras_ = 0;
	int amountOfVertices_ = 0;

public:
	void init();
	void setPointsOnPhotos(
		flt2* points,
		Vec2d<uint32_t>* mapping,
		int count
	);
};

class ModelMaker
{
	static std::unique_ptr<ModelMaker> ptr;

	GridIntegralsA gridIntegralsA;
	GridIntegralsB gridIntegralsB;
	OperationsOnGridIntegrals operationsOnGridIntegrals;
	LeastSquaresOfJacobianDeterminant leastSquaresOfJacobianDeterminant;
	FindNearestDefinedPoint findNearestDefinedPoint;

	ID3D11Texture2D* setOfPhotos = nullptr;
	ID3D11ShaderResourceView* setOfPhotosSRV = nullptr;
	int width = 0;
	int height = 0;
	int texturesCount = 0;

	int widthA = 0;
	int heightA = 0;

	int widthAreal = 0;
	int heightAreal = 0;

	int widthB = 0;
	int heightB = 0;

	int widthAA = 0;
	int heightAA = 0;

	int widthAB = 0;
	int heightAB = 0;

	int widthABreal = 0;
	int heightABreal = 0;
	int countOfABtextures = 0;

	int widthBB = 0;
	int heightBB = 0;

	bool bSetOfPhotosIsLoaded = false;

	ID3D11Texture2D* photosIntegralsAxh = nullptr;
	ID3D11Texture2D* photosIntegralsAyh = nullptr;
	ID3D11Texture2D* photosIntegralsAzh = nullptr;

	ID3D11Texture2D* photosIntegralsAx = nullptr;
	ID3D11Texture2D* photosIntegralsAy = nullptr;
	ID3D11Texture2D* photosIntegralsAz = nullptr;

	ID3D11UnorderedAccessView* photosIntegralsAxhUAV = nullptr;
	ID3D11UnorderedAccessView* photosIntegralsAyhUAV = nullptr;
	ID3D11UnorderedAccessView* photosIntegralsAzhUAV = nullptr;

	ID3D11ShaderResourceView* photosIntegralsAxhSRV = nullptr;
	ID3D11ShaderResourceView* photosIntegralsAyhSRV = nullptr;
	ID3D11ShaderResourceView* photosIntegralsAzhSRV = nullptr;

	ID3D11UnorderedAccessView* photosIntegralsAxUAV = nullptr;
	ID3D11UnorderedAccessView* photosIntegralsAyUAV = nullptr;
	ID3D11UnorderedAccessView* photosIntegralsAzUAV = nullptr;
	
	ID3D11ShaderResourceView* photosIntegralsAxSRV = nullptr;
	ID3D11ShaderResourceView* photosIntegralsAySRV = nullptr;
	ID3D11ShaderResourceView* photosIntegralsAzSRV = nullptr;

	ID3D11Texture2D* photosIntegralsBx = nullptr;
	ID3D11Texture2D* photosIntegralsBy = nullptr;
	ID3D11Texture2D* photosIntegralsBz = nullptr;

	ID3D11UnorderedAccessView* photosIntegralsBxUAV = nullptr;
	ID3D11UnorderedAccessView* photosIntegralsByUAV = nullptr;
	ID3D11UnorderedAccessView* photosIntegralsBzUAV = nullptr;

	ID3D11ShaderResourceView* photosIntegralsBxSRV = nullptr;
	ID3D11ShaderResourceView* photosIntegralsBySRV = nullptr;
	ID3D11ShaderResourceView* photosIntegralsBzSRV = nullptr;

	ID3D11Texture2D* AA = nullptr;
	ID3D11Texture2D* AB = nullptr;
	ID3D11Texture2D* BB = nullptr;

	ID3D11Texture2D* AAfraction = nullptr;
	ID3D11Texture2D* ABfraction = nullptr;
	ID3D11Texture2D* BBfraction = nullptr;

	ID3D11UnorderedAccessView* AAuav = nullptr;
	ID3D11UnorderedAccessView* ABuav = nullptr;
	ID3D11UnorderedAccessView* BBuav = nullptr;

	ID3D11UnorderedAccessView* AAfractionUAV = nullptr;
	ID3D11UnorderedAccessView* ABfractionUAV = nullptr;
	ID3D11UnorderedAccessView* BBfractionUAV = nullptr;

	ID3D11ShaderResourceView* AAsrv = nullptr;
	ID3D11ShaderResourceView* ABsrv = nullptr;
	ID3D11ShaderResourceView* BBsrv = nullptr;

	ID3D11ShaderResourceView* AAfractionSRV = nullptr;
	ID3D11ShaderResourceView* ABfractionSRV = nullptr;
	ID3D11ShaderResourceView* BBfractionSRV = nullptr;

	ID3D11Texture2D* maxA = nullptr;
	ID3D11Texture2D* maxB = nullptr;

	ID3D11UnorderedAccessView* maxAuav = nullptr;
	ID3D11UnorderedAccessView* maxBuav = nullptr;

	ID3D11ShaderResourceView* maxAsrv = nullptr;
	ID3D11ShaderResourceView* maxBsrv = nullptr;

	ID3D11Texture2D* AAh = nullptr;
	ID3D11Texture2D* AAfractionH = nullptr;
	ID3D11Texture2D* maxAh = nullptr;

	ID3D11UnorderedAccessView* AAhUAV = nullptr;
	ID3D11UnorderedAccessView* AAfractionHuav = nullptr;
	ID3D11UnorderedAccessView* maxAhUAV = nullptr;

	ID3D11ShaderResourceView* AAhSRV = nullptr;
	ID3D11ShaderResourceView* AAfractionHsrv = nullptr;
	ID3D11ShaderResourceView* maxAhSRV = nullptr;

	ID3D11Texture2D* error = nullptr;
	ID3D11UnorderedAccessView* errorUAV = nullptr;
	ID3D11ShaderResourceView* errorSRV = nullptr;

	ID3D11Texture2D* AtoBx = nullptr;
	ID3D11Texture2D* AtoBy = nullptr;
	ID3D11Texture2D* AtoBz = nullptr;
	ID3D11Texture2D* AtoBw = nullptr;

	ID3D11UnorderedAccessView* AtoBxUAV = nullptr;
	ID3D11UnorderedAccessView* AtoByUAV = nullptr;
	ID3D11UnorderedAccessView* AtoBzUAV = nullptr;
	ID3D11UnorderedAccessView* AtoBwUAV = nullptr;

	ID3D11ShaderResourceView* AtoBxSRV = nullptr;
	ID3D11ShaderResourceView* AtoBySRV = nullptr;
	ID3D11ShaderResourceView* AtoBzSRV = nullptr;
	ID3D11ShaderResourceView* AtoBwSRV = nullptr;

	void initSetOfPhotos(const std::vector<std::string>& paths);
	void initPhotosIntegralsA();
	void initPhotosIntegralsB();
	void initAAandMaxA();
	void initAB();
	void initBBandMaxB();
	void initErrorsAndMapping();

	void initDimensionsOfTextures();

public:
	static ModelMaker* instance();
	static void init();

	void loadPhotos(const std::vector<std::string>& paths);
	void defineTheSamePointsOnSetOfPhotos();
	Vec4d<int> findTheSamePoint(const Vec2d<int>& pt);

	int getWidth();
	int getHeight();

	void freeResources(bool freeResult = false);
};