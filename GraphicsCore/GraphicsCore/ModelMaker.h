#pragma once
#include <d3dx11effect.h>
#include <memory>
#include <vector>
#include <string>

#define CELL_DIMENSION_X 32 // for spheres this is 36
#define CELL_DIMENSION_Y 32 // for spheres this is 36

#define RADIUS_IN_CELLS 7 

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

	ID3DX11EffectVariable* hCellDimensionX = nullptr;
	ID3DX11EffectVariable* hCellDimensionY = nullptr;

public:
	void init();
	void clearPhotosIntegrals(
		ID3D11UnorderedAccessView* photosIntegralsXh,
		ID3D11UnorderedAccessView* photosIntegralsYh,
		ID3D11UnorderedAccessView* photosIntegralsZh,
		ID3D11UnorderedAccessView* photosIntegralsX,
		ID3D11UnorderedAccessView* photosIntegralsY,
		ID3D11UnorderedAccessView* photosIntegralsZ,
		int width, int height, int textureCount
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
		int width, int height, int textureCount
	);
};

struct GridIntegrals
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

	ID3DX11EffectVariable* hAngle0 = nullptr;
	ID3DX11EffectVariable* hScale0 = nullptr;

	ID3DX11EffectVariable* hAngle1 = nullptr;
	ID3DX11EffectVariable* hScale1 = nullptr;

	ID3DX11EffectVariable* hOffsetX = nullptr;
	ID3DX11EffectVariable* hOffsetY = nullptr;

	ID3DX11EffectVariable* hCellDimensionX = nullptr;
	ID3DX11EffectVariable* hCellDimensionY = nullptr;
public:
	void init();
	void clearPhotosIntegrals(
		ID3D11UnorderedAccessView* photosIntegralsX,
		ID3D11UnorderedAccessView* photosIntegralsY,
		ID3D11UnorderedAccessView* photosIntegralsZ,
		int width, int height, int count
	);
	void calculatePhotosIntegrals(
		ID3D11ShaderResourceView* photos,
		ID3D11UnorderedAccessView* photosIntegralsX,
		ID3D11UnorderedAccessView* photosIntegralsY,
		ID3D11UnorderedAccessView* photosIntegralsZ,
		int width, int height, int count,
		float angle0, float scale0,
		float angle1, float scale1,
		int offsetX, int offsetY
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

	ID3DX11EffectShaderResourceVariable* hMapAtoB = nullptr;

	ID3DX11EffectVariable* hWidth = nullptr;
	ID3DX11EffectVariable* hHeight = nullptr;
	ID3DX11EffectVariable* hTexturesCount = nullptr;

	ID3DX11EffectVariable* hCellRadius = nullptr;
public:
	void init();
	void clearA(
		ID3D11UnorderedAccessView* AA,
		ID3D11UnorderedAccessView* AAfraction,
		ID3D11UnorderedAccessView* maxA,
		int width, int height, int count
	);
	void clearAB(
		ID3D11UnorderedAccessView* AB,
		ID3D11UnorderedAccessView* ABfraction,
		ID3D11UnorderedAccessView* BB,
		ID3D11UnorderedAccessView* BBfraction,
		ID3D11UnorderedAccessView* maxB,
		int width, int height, int count
	);
	void calculateA(
		ID3D11ShaderResourceView* photosIntegralsAx,
		ID3D11ShaderResourceView* photosIntegralsAy,
		ID3D11ShaderResourceView* photosIntegralsAz,
		ID3D11UnorderedAccessView* AA,
		ID3D11UnorderedAccessView* AAfraction,
		ID3D11UnorderedAccessView* maxA,
		int width, int height, int count
	);
	void calculateAB(
		ID3D11ShaderResourceView* photosIntegralsAx,
		ID3D11ShaderResourceView* photosIntegralsAy,
		ID3D11ShaderResourceView* photosIntegralsAz,
		ID3D11ShaderResourceView* photosIntegralsBx,
		ID3D11ShaderResourceView* photosIntegralsBy,
		ID3D11ShaderResourceView* photosIntegralsBz,
		ID3D11ShaderResourceView* mapAtoB,
		ID3D11UnorderedAccessView* AB,
		ID3D11UnorderedAccessView* ABfraction,
		ID3D11UnorderedAccessView* BB,
		ID3D11UnorderedAccessView* BBfraction,
		ID3D11UnorderedAccessView* maxB,
		int width, int height, int count
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

	ID3DX11EffectShaderResourceVariable* hMapAtoB = nullptr;

	ID3DX11EffectVariable* hWidth = nullptr;
	ID3DX11EffectVariable* hHeight = nullptr;
	ID3DX11EffectVariable* hTexturesCount = nullptr;

	ID3DX11EffectVariable* hAngle0 = nullptr;
	ID3DX11EffectVariable* hScale0 = nullptr;

	ID3DX11EffectVariable* hAngle1 = nullptr;
	ID3DX11EffectVariable* hScale1 = nullptr;

	ID3DX11EffectVariable* hOffsetX = nullptr;
	ID3DX11EffectVariable* hOffsetY = nullptr;

	ID3DX11EffectVariable* hCellRadius = nullptr;
	ID3DX11EffectVariable* hUnitX = nullptr;
	ID3DX11EffectVariable* hUnitY = nullptr;

	ID3DX11EffectUnorderedAccessViewVariable* hError = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hAtoB = nullptr;
public:
	void init();
	void clear(
		ID3D11UnorderedAccessView* error,
		ID3D11UnorderedAccessView* AtoB,
		int width, int height, int count
	);
	void calculateErrors(
		ID3D11ShaderResourceView* AA,
		ID3D11ShaderResourceView* AB,
		ID3D11ShaderResourceView* BB,
		ID3D11ShaderResourceView* AAfraction,
		ID3D11ShaderResourceView* ABfraction,
		ID3D11ShaderResourceView* BBfraction,
		ID3D11ShaderResourceView* maxA,
		ID3D11ShaderResourceView* maxB,
		ID3D11ShaderResourceView* mapAtoB,
		ID3D11UnorderedAccessView* error,
		ID3D11UnorderedAccessView* AtoB,
		int width, int height, int count,
		float angle0, float scale0,
		float angle1, float scale1,
		int offsetX, int offsetY
	);
};

class ModelMaker
{
	static std::unique_ptr<ModelMaker> ptr;

	GridIntegrals gridIntegrals;
	OperationsOnGridIntegrals operationsOnGridIntegrals;
	LeastSquaresOfJacobianDeterminant leastSquaresOfJacobianDeterminant;

	ID3D11Texture2D* setOfPhotos = nullptr;
	ID3D11ShaderResourceView* setOfPhotosSRV = nullptr;
	int width = 0;
	int height = 0;
	int texturesCount = 0;

	bool bSetOfPhotosIsLoaded = false;

	ID3D11Texture2D* photosIntegralsAx = nullptr;
	ID3D11Texture2D* photosIntegralsAy = nullptr;
	ID3D11Texture2D* photosIntegralsAz = nullptr;

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

	ID3D11Buffer* mapAtoB = nullptr;
	ID3D11ShaderResourceView* mapAtoBsrv = nullptr;

	ID3D11Texture2D* error = nullptr;
	ID3D11UnorderedAccessView* errorUAV = nullptr;

	ID3D11Texture2D* AtoB = nullptr;
	ID3D11UnorderedAccessView* AtoBuav = nullptr;

public:
	static ModelMaker* instance();
	static void init();

	void loadPhotos(const std::vector<std::string>& paths);
	void defineTheSamePointsOnSetOfPhotos();
};