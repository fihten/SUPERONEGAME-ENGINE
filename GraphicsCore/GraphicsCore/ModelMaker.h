#pragma once
#include <d3dx11effect.h>
#include <memory>
#include <vector>
#include <string>

#define CELL_DIMENSION_X 32 // for spheres this is 36
#define CELL_DIMENSION_Y 32 // for spheres this is 36

#define RADIUS_IN_CELLS 7 

struct GridIntegrals
{
	ID3DX11Effect* hGridIntegralsFX = nullptr;
	ID3DX11EffectTechnique* hClearGridIntegralsTech = nullptr;
	ID3DX11EffectTechnique* hCalculateGridIntegralsTech = nullptr;

	ID3DX11EffectShaderResourceVariable* hPhotos = nullptr;
	ID3DX11EffectUnorderedAccessViewVariable* hPhotosIntegrals = nullptr;

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
		ID3D11UnorderedAccessView* photosIntegrals,
		int width, int height, int count
	);
	void calculatePhotosIntegrals(
		ID3D11ShaderResourceView* photos,
		ID3D11UnorderedAccessView* photosIntegrals,
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

	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsA = nullptr;
	ID3DX11EffectShaderResourceVariable* hPhotosIntegralsB = nullptr;

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
		ID3D11ShaderResourceView* photosIntegralsA,
		ID3D11UnorderedAccessView* AA,
		ID3D11UnorderedAccessView* AAfraction,
		ID3D11UnorderedAccessView* maxA,
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

	ID3D11Texture2D* photosIntegralsA = nullptr;
	ID3D11UnorderedAccessView* photosIntegralsAuav = nullptr;
	ID3D11ShaderResourceView* photosIntegralsAsrv = nullptr;

	ID3D11Texture2D* photosIntegralsB = nullptr;
	ID3D11UnorderedAccessView* photosIntegralsBuav = nullptr;
	ID3D11ShaderResourceView* photosIntegralsBsrv = nullptr;

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

public:
	static ModelMaker* instance();
	static void init();

	void loadPhotos(const std::vector<std::string>& paths);
};