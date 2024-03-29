#include "ModelMaker.h"
#include "GraphicsCore.h"
#include <D3DX11.h>

std::unique_ptr<ModelMaker> ModelMaker::ptr;

ModelMaker* ModelMaker::instance()
{
	if (!ptr)
	{
		ptr = std::make_unique<ModelMaker>();
	}
	return ptr.get();
}

void GridIntegrals::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sGridIntegrals = std::string(shadersFolder) + "\\GridIntegrals.fx";

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sGridIntegrals.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	auto device = GraphicsCore::instance()->device;
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &hGridIntegralsFX);

	hClearGridIntegralsTech = hGridIntegralsFX->GetTechniqueByName("ClearGridIntegrals");
	hCalculateGridIntegralsTech = hGridIntegralsFX->GetTechniqueByName("CalculateGridIntegrals");
	
	hPhotos = hGridIntegralsFX->GetVariableByName("photos")->AsShaderResource();
	hPhotosIntegrals = hGridIntegralsFX->GetVariableByName("photosIntegrals")->AsUnorderedAccessView();

	hWidth = hGridIntegralsFX->GetVariableByName("width");
	hHeight = hGridIntegralsFX->GetVariableByName("height");
	hTexturesCount = hGridIntegralsFX->GetVariableByName("texturesCount");

	hAngle0 = hGridIntegralsFX->GetVariableByName("angle0");
	hScale0 = hGridIntegralsFX->GetVariableByName("scale0");

	hAngle1 = hGridIntegralsFX->GetVariableByName("angle1");
	hScale1 = hGridIntegralsFX->GetVariableByName("scale1");

	hOffsetX = hGridIntegralsFX->GetVariableByName("offsetX");
	hOffsetY = hGridIntegralsFX->GetVariableByName("offsetY");

	hCellDimensionX = hGridIntegralsFX->GetVariableByName("cellDimensionX");
	hCellDimensionY = hGridIntegralsFX->GetVariableByName("cellDimensionY");
}

void GridIntegrals::clearPhotosIntegrals(
	ID3D11UnorderedAccessView* photosIntegrals,
	int width, int height, int count
)
{
	auto context = GraphicsCore::instance()->context;

	// Set shader variables
	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	int cellDimensionX = CELL_DIMENSION_X;
	hCellDimensionX->SetRawValue(&cellDimensionX, 0, sizeof(cellDimensionX));

	int cellDimensionY = CELL_DIMENSION_Y;
	hCellDimensionY->SetRawValue(&cellDimensionY, 0, sizeof(cellDimensionY));

	hPhotosIntegrals->SetUnorderedAccessView(photosIntegrals);

	// define number of thread groups

	int cellsAlongX = std::ceil((float)width / cellDimensionX);
	int cellsAlongY = std::ceil((float)height / cellDimensionY);

	uint32_t groups_x = std::ceil((float)(cellsAlongX) / 16);
	uint32_t groups_y = std::ceil((float)(cellsAlongY) / 16);
	uint32_t groups_z = std::ceil((float)(count) / 4);

	hClearGridIntegralsTech->GetPassByName("P0")->Apply(0, context);
	context->Dispatch(groups_x, groups_y, groups_z);

	hPhotosIntegrals->SetUnorderedAccessView(nullptr);
}

void GridIntegrals::calculatePhotosIntegrals(
	ID3D11ShaderResourceView* photos,
	ID3D11UnorderedAccessView* photosIntegrals,
	int width, int height, int count,
	float angle0, float scale0,
	float angle1, float scale1,
	int offsetX, int offsetY
)
{
	auto context = GraphicsCore::instance()->context;

	hPhotos->SetResource(photos);
	hPhotosIntegrals->SetUnorderedAccessView(photosIntegrals);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	hAngle0->SetRawValue(&angle0, 0, sizeof(angle0));
	hScale0->SetRawValue(&scale0, 0, sizeof(scale0));

	hAngle1->SetRawValue(&angle1, 0, sizeof(angle1));
	hScale1->SetRawValue(&scale1, 0, sizeof(scale1));

	hOffsetX->SetRawValue(&offsetX, 0, sizeof(offsetX));
	hOffsetY->SetRawValue(&offsetY, 0, sizeof(offsetY));

	int cellDimensionX = CELL_DIMENSION_X;
	hCellDimensionX->SetRawValue(&cellDimensionX, 0, sizeof(cellDimensionX));

	int cellDimensionY = CELL_DIMENSION_Y;
	hCellDimensionY->SetRawValue(&cellDimensionY, 0, sizeof(cellDimensionY));

	hCalculateGridIntegralsTech->GetPassByName("P0")->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(width) / 16);
	uint32_t groups_y = std::ceil((float)(height) / 16);
	uint32_t groups_z = std::ceil((float)(count) / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	hPhotos->SetResource(nullptr);
	hPhotosIntegrals->SetUnorderedAccessView(nullptr);
}

void OperationsOnGridIntegrals::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sOperationsOnGridIntegrals = std::string(shadersFolder) + "\\OperationsOnGridIntegrals.fx";

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sOperationsOnGridIntegrals.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	auto device = GraphicsCore::instance()->device;
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &hOperationsOnGridIntegralsFX);

	hMakeOperationsOnGridIntegralsTech = hOperationsOnGridIntegralsFX->GetTechniqueByName("MakeOperationsOnGridIntegrals");

	hPhotosIntegralsA = hOperationsOnGridIntegralsFX->GetVariableByName("photosIntegralsA")->AsShaderResource();
	hPhotosIntegralsB = hOperationsOnGridIntegralsFX->GetVariableByName("photosIntegralsB")->AsShaderResource();

	hAA = hOperationsOnGridIntegralsFX->GetVariableByName("AA")->AsUnorderedAccessView();
	hAB = hOperationsOnGridIntegralsFX->GetVariableByName("AB")->AsUnorderedAccessView();
	hBB = hOperationsOnGridIntegralsFX->GetVariableByName("BB")->AsUnorderedAccessView();

	hAAfraction = hOperationsOnGridIntegralsFX->GetVariableByName("AAfraction")->AsUnorderedAccessView();
	hABfraction = hOperationsOnGridIntegralsFX->GetVariableByName("ABfraction")->AsUnorderedAccessView();
	hBBfraction = hOperationsOnGridIntegralsFX->GetVariableByName("BBfraction")->AsUnorderedAccessView();

	hMaxA = hOperationsOnGridIntegralsFX->GetVariableByName("maxA")->AsUnorderedAccessView();
	hMaxB = hOperationsOnGridIntegralsFX->GetVariableByName("maxB")->AsUnorderedAccessView();

	hMapAtoB = hOperationsOnGridIntegralsFX->GetVariableByName("mapAtoB")->AsShaderResource();

	hWidth = hOperationsOnGridIntegralsFX->GetVariableByName("width");
	hHeight = hOperationsOnGridIntegralsFX->GetVariableByName("height");
	hTexturesCount = hOperationsOnGridIntegralsFX->GetVariableByName("texturesCount");

	hCellRadius = hOperationsOnGridIntegralsFX->GetVariableByName("cellRadius");
}

void OperationsOnGridIntegrals::clearA(
	ID3D11UnorderedAccessView* AA,
	ID3D11UnorderedAccessView* AAfraction,
	ID3D11UnorderedAccessView* maxA,
	int width, int height, int count
)
{
	auto context = GraphicsCore::instance()->context;

	hAA->SetUnorderedAccessView(AA);
	hAAfraction->SetUnorderedAccessView(AAfraction);
	hMaxA->SetUnorderedAccessView(maxA);

	width = std::ceil((float)width / CELL_DIMENSION_X);
	height = std::ceil((float)height / CELL_DIMENSION_Y);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	int cellRadius = RADIUS_IN_CELLS;
	hCellRadius->SetRawValue(&cellRadius, 0, sizeof(cellRadius));

	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)width / 16);
	uint32_t groups_y = std::ceil((float)height / 16);
	uint32_t groups_z = std::ceil((float)count / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	hAA->SetUnorderedAccessView(nullptr);
	hAAfraction->SetUnorderedAccessView(nullptr);
	hMaxA->SetUnorderedAccessView(nullptr);
}

void OperationsOnGridIntegrals::clearAB(
	ID3D11UnorderedAccessView* AB,
	ID3D11UnorderedAccessView* ABfraction,
	ID3D11UnorderedAccessView* BB,
	ID3D11UnorderedAccessView* BBfraction,
	ID3D11UnorderedAccessView* maxB,
	int width, int height, int count
)
{
	auto context = GraphicsCore::instance()->context;

	hAB->SetUnorderedAccessView(AB);
	hABfraction->SetUnorderedAccessView(ABfraction);

	hBB->SetUnorderedAccessView(BB);
	hBBfraction->SetUnorderedAccessView(BBfraction);

	hMaxB->SetUnorderedAccessView(maxB);

	width = std::ceil((float)width / CELL_DIMENSION_X);
	height = std::ceil((float)height / CELL_DIMENSION_Y);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	int cellRadius = RADIUS_IN_CELLS;
	hCellRadius->SetRawValue(&cellRadius, 0, sizeof(cellRadius));

	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(1)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)width / 16);
	uint32_t groups_y = std::ceil((float)height / 16);
	uint32_t groups_z = std::ceil((float)count / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	hAB->SetUnorderedAccessView(nullptr);
	hABfraction->SetUnorderedAccessView(nullptr);

	hBB->SetUnorderedAccessView(nullptr);
	hBBfraction->SetUnorderedAccessView(nullptr);

	hMaxB->SetUnorderedAccessView(nullptr);
}

void OperationsOnGridIntegrals::calculateA(
	ID3D11ShaderResourceView* photosIntegralsA,
	ID3D11UnorderedAccessView* AA,
	ID3D11UnorderedAccessView* AAfraction,
	ID3D11UnorderedAccessView* maxA,
	int width, int height, int count
)
{
	auto context = GraphicsCore::instance()->context;

	hPhotosIntegralsA->SetResource(photosIntegralsA);
	hAA->SetUnorderedAccessView(AA);
	hAAfraction->SetUnorderedAccessView(AAfraction);
	hMaxA->SetUnorderedAccessView(maxA);

	width = std::ceil((float)width / CELL_DIMENSION_X);
	height = std::ceil((float)height / CELL_DIMENSION_Y);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	int cellRadius = RADIUS_IN_CELLS;
	hCellRadius->SetRawValue(&cellRadius, 0, sizeof(cellRadius));

	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(2)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)width / 16);
	uint32_t groups_y = std::ceil((float)height / 16);
	uint32_t groups_z = std::ceil((float)count / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	hPhotosIntegralsA->SetResource(nullptr);
	hAA->SetUnorderedAccessView(nullptr);
	hAAfraction->SetUnorderedAccessView(nullptr);
	hMaxA->SetUnorderedAccessView(nullptr);
}

void OperationsOnGridIntegrals::calculateAB(
	ID3D11ShaderResourceView* photosIntegralsA,
	ID3D11ShaderResourceView* photosIntegralsB,
	ID3D11ShaderResourceView* mapAtoB,
	ID3D11UnorderedAccessView* AB,
	ID3D11UnorderedAccessView* ABfraction,
	ID3D11UnorderedAccessView* BB,
	ID3D11UnorderedAccessView* BBfraction,
	ID3D11UnorderedAccessView* maxB,
	int width, int height, int count
)
{
	auto context = GraphicsCore::instance()->context;

	hPhotosIntegralsA->SetResource(photosIntegralsA);
	hPhotosIntegralsB->SetResource(photosIntegralsB);
	hMapAtoB->SetResource(mapAtoB);

	hAB->SetUnorderedAccessView(AB);
	hABfraction->SetUnorderedAccessView(ABfraction);

	hBB->SetUnorderedAccessView(BB);
	hBBfraction->SetUnorderedAccessView(BBfraction);

	hMaxB->SetUnorderedAccessView(maxB);

	width = std::ceil((float)width / CELL_DIMENSION_X);
	height = std::ceil((float)height / CELL_DIMENSION_Y);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	int cellRadius = RADIUS_IN_CELLS;
	hCellRadius->SetRawValue(&cellRadius, 0, sizeof(cellRadius));

	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(3)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)width / 16);
	uint32_t groups_y = std::ceil((float)height / 16);
	uint32_t groups_z = std::ceil((float)count / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	hPhotosIntegralsA->SetResource(nullptr);
	hPhotosIntegralsB->SetResource(nullptr);
	hMapAtoB->SetResource(nullptr);

	hAB->SetUnorderedAccessView(nullptr);
	hABfraction->SetUnorderedAccessView(nullptr);

	hBB->SetUnorderedAccessView(nullptr);
	hBBfraction->SetUnorderedAccessView(nullptr);

	hMaxB->SetUnorderedAccessView(nullptr);
}

void LeastSquaresOfJacobianDeterminant::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sLeastSquaresOfJacobianDeterminant = std::string(shadersFolder) + "\\LeastSquaresOfJacobianDeterminant.fx";

	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sLeastSquaresOfJacobianDeterminant.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	auto device = GraphicsCore::instance()->device;
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &hLeastSquaresOfJacobianDeterminantFX);

	hClearTech = hLeastSquaresOfJacobianDeterminantFX->GetTechniqueByName("Clear");
	hApplyLeastSquareMethodTech = hLeastSquaresOfJacobianDeterminantFX->GetTechniqueByName("ApplyLeastSquareMethod");

	hAA = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("AA")->AsShaderResource();
	hAB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("AB")->AsShaderResource();
	hBB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("BB")->AsShaderResource();

	hAAfraction = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("AAfraction")->AsShaderResource();
	hABfraction = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("ABfraction")->AsShaderResource();
	hBBfraction = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("BBfraction")->AsShaderResource();

	hMaxA = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("maxA")->AsShaderResource();
	hMaxB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("maxB")->AsShaderResource();

	hMapAtoB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("mapAtoB")->AsShaderResource();

	hWidth = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("width");
	hHeight = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("height");
	hTexturesCount = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("texturesCount");

	hAngle0 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("angle0");
	hScale0 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("scale0");

	hAngle1 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("angle1");
	hScale1 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("scale1");

	hOffsetX = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("offsetX");
	hOffsetY = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("offsetY");

	hCellRadius = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("cellRadius");
	hUnitX = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("unitX");
	hUnitY = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("unitY");

	hError = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("error")->AsUnorderedAccessView();
	hAtoB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("AtoB")->AsUnorderedAccessView();
}

void LeastSquaresOfJacobianDeterminant::clear(
	ID3D11UnorderedAccessView* error,
	ID3D11UnorderedAccessView* AtoB,
	int width, int height, int count
)
{
	auto context = GraphicsCore::instance()->context;

	width = std::ceil((float)width / CELL_DIMENSION_X);
	height = std::ceil((float)height / CELL_DIMENSION_Y);

	int cellDiameter = 2 * RADIUS_IN_CELLS + 1;
	width = std::ceil((float)width / cellDiameter);
	height = std::ceil((float)height / cellDiameter);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	hError->SetUnorderedAccessView(error);
	hAtoB->SetUnorderedAccessView(AtoB);

	hClearTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)width / 16);
	uint32_t groups_y = std::ceil((float)height / 16);
	uint32_t groups_z = std::ceil((float)count / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	hError->SetUnorderedAccessView(nullptr);
	hAtoB->SetUnorderedAccessView(nullptr);
}

void LeastSquaresOfJacobianDeterminant::calculateErrors(
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
)
{
	auto context = GraphicsCore::instance()->context;

	hAA->SetResource(AA);
	hAB->SetResource(AB);
	hBB->SetResource(BB);

	hAAfraction->SetResource(AAfraction);
	hABfraction->SetResource(ABfraction);
	hBBfraction->SetResource(BBfraction);

	hMaxA->SetResource(maxA);
	hMaxB->SetResource(maxB);

	hMapAtoB->SetResource(mapAtoB);

	hError->SetUnorderedAccessView(error);
	hAtoB->SetUnorderedAccessView(AtoB);

	width = std::ceil((float)width / CELL_DIMENSION_X);
	height = std::ceil((float)height / CELL_DIMENSION_Y);

	int cellRadius = RADIUS_IN_CELLS;
	int cellDiameter = 2 * cellRadius + 1;
	width = std::ceil((float)width / cellDiameter);
	height = std::ceil((float)height / cellDiameter);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	hAngle0->SetRawValue(&angle0, 0, sizeof(angle0));
	hScale0->SetRawValue(&scale0, 0, sizeof(scale0));

	hAngle1->SetRawValue(&angle0, 0, sizeof(angle1));
	hScale1->SetRawValue(&scale0, 0, sizeof(scale1));

	hOffsetX->SetRawValue(&offsetX, 0, sizeof(offsetX));
	hOffsetY->SetRawValue(&offsetY, 0, sizeof(offsetY));

	hCellRadius->SetRawValue(&cellRadius, 0, sizeof(cellRadius));

	int unitX = CELL_DIMENSION_X;
	hUnitX->SetRawValue(&unitX, 0, sizeof(unitX));

	int unitY = CELL_DIMENSION_Y;
	hUnitY->SetRawValue(&unitY, 0, sizeof(unitY));

	hApplyLeastSquareMethodTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)width / 16);
	uint32_t groups_y = std::ceil((float)height / 16);
	uint32_t groups_z = std::ceil((float)count / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	hAA->SetResource(nullptr);
	hAB->SetResource(nullptr);
	hBB->SetResource(nullptr);

	hAAfraction->SetResource(nullptr);
	hABfraction->SetResource(nullptr);
	hBBfraction->SetResource(nullptr);

	hMaxA->SetResource(nullptr);
	hMaxB->SetResource(nullptr);

	hMapAtoB->SetResource(nullptr);

	hError->SetUnorderedAccessView(nullptr);
	hAtoB->SetUnorderedAccessView(nullptr);
}

void ModelMaker::init()
{
	ptr->gridIntegrals.init();
	ptr->operationsOnGridIntegrals.init();
	ptr->leastSquaresOfJacobianDeterminant.init();
}

void ModelMaker::loadPhotos(const std::vector<std::string>& paths)
{
	if (bSetOfPhotosIsLoaded)
	{
		setOfPhotos->Release();
		setOfPhotos = nullptr;

		setOfPhotosSRV->Release();
		setOfPhotosSRV = nullptr;

		photosIntegralsA->Release();
		photosIntegralsA = nullptr;

		photosIntegralsAuav->Release();
		photosIntegralsAuav = nullptr;

		photosIntegralsAsrv->Release();
		photosIntegralsAsrv = nullptr;

		photosIntegralsB->Release();
		photosIntegralsB = nullptr;

		photosIntegralsBuav->Release();
		photosIntegralsBuav = nullptr;

		photosIntegralsBsrv->Release();
		photosIntegralsBsrv = nullptr;

		AA->Release();
		AA = nullptr;

		AB->Release();
		AB = nullptr;

		BB->Release();
		BB = nullptr;

		AAfraction->Release();
		AAfraction = nullptr;

		ABfraction->Release();
		ABfraction = nullptr;

		BBfraction->Release();
		BBfraction = nullptr;

		AAuav->Release();
		AAuav = nullptr;

		ABuav->Release();
		ABuav = nullptr;

		BBuav->Release();
		BBuav = nullptr;

		AAfractionUAV->Release();
		AAfractionUAV = nullptr;

		ABfractionUAV->Release();
		ABfractionUAV = nullptr;

		BBfractionUAV->Release();
		BBfractionUAV = nullptr;

		AAsrv->Release();
		AAsrv = nullptr;

		ABsrv->Release();
		ABsrv = nullptr;

		BBsrv->Release();
		BBsrv = nullptr;

		AAfractionSRV->Release();
		AAfractionSRV = nullptr;

		ABfractionSRV->Release();
		ABfractionSRV = nullptr;

		BBfractionSRV->Release();
		BBfractionSRV = nullptr;

		maxA->Release();
		maxA = nullptr;

		maxB->Release();
		maxB = nullptr;

		maxAsrv->Release();
		maxAsrv = nullptr;

		maxBsrv->Release();
		maxBsrv = nullptr;

		maxAuav->Release();
		maxAuav = nullptr;

		maxBuav->Release();
		maxBuav = nullptr;

		mapAtoB->Release();
		mapAtoB = nullptr;

		mapAtoBsrv->Release();
		mapAtoBsrv = nullptr;

		error->Release();
		error = nullptr;

		errorUAV->Release();
		errorUAV = nullptr;

		AtoB->Release();
		AtoB = nullptr;

		AtoBuav->Release();
		AtoBuav = nullptr;
	}

	auto device = GraphicsCore::instance()->device;
	auto context = GraphicsCore::instance()->context;

	int count = paths.size();
	std::vector<ID3D11Texture2D*> photos(count);
	for (int i = 0; i < count; i++)
	{
		const std::string& path = paths[i];

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		loadInfo.Width = D3DX11_FROM_FILE;
		loadInfo.Height = D3DX11_FROM_FILE;
		loadInfo.Depth = D3DX11_FROM_FILE;
		loadInfo.FirstMipLevel = 0;
		loadInfo.MipLevels = D3DX11_FROM_FILE;
		loadInfo.Usage = D3D11_USAGE_STAGING;
		loadInfo.BindFlags = 0;
		loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		loadInfo.MiscFlags = 0;
		loadInfo.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		loadInfo.Filter = D3DX11_FILTER_LINEAR;
		loadInfo.MipFilter = D3DX11_FILTER_LINEAR;
		loadInfo.pSrcInfo = 0;
		D3DX11CreateTextureFromFileA(
			device,
			path.c_str(),
			&loadInfo,
			0,
			(ID3D11Resource * *)& photos[i],
			0
		);
	}

	D3D11_TEXTURE2D_DESC texElementDesc;
	photos[0]->GetDesc(&texElementDesc);
	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = count;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;
	device->CreateTexture2D(&texArrayDesc, 0, &setOfPhotos);

	for (int texElement = 0; texElement < count; ++texElement)
	{
		int mipLevel = 0;
		
		D3D11_MAPPED_SUBRESOURCE mappedTex2D;
		context->Map(photos[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D);
		context->UpdateSubresource(
			setOfPhotos,
			D3D11CalcSubresource(
				mipLevel,
				texElement,
				texElementDesc.MipLevels
			),
			0,
			mappedTex2D.pData,
			mappedTex2D.RowPitch,
			mappedTex2D.DepthPitch
		);
		context->Unmap(photos[texElement], mipLevel);
	}

	width = texElementDesc.Width;
	height = texElementDesc.Height;
	texturesCount = count;

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = texElementDesc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = count;
	device->CreateShaderResourceView(setOfPhotos, &srv_desc, &setOfPhotosSRV);

	int cellsAlongX = std::ceil((float)(width) / CELL_DIMENSION_X);
	int cellsAlongY = std::ceil((float)(height) / CELL_DIMENSION_Y);

	texArrayDesc.Width = cellsAlongX;
	texArrayDesc.Height = cellsAlongY;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = count;
	texArrayDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsA);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsB);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = count;
	device->CreateUnorderedAccessView(photosIntegralsA, &uav_desc, &photosIntegralsAuav);
	device->CreateUnorderedAccessView(photosIntegralsB, &uav_desc, &photosIntegralsBuav);

	srv_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = count;
	device->CreateShaderResourceView(photosIntegralsA, &srv_desc, &photosIntegralsAsrv);
	device->CreateShaderResourceView(photosIntegralsB, &srv_desc, &photosIntegralsBsrv);

	int diameterInCells = 2 * RADIUS_IN_CELLS + 1;

	texArrayDesc.Width = std::ceil(cellsAlongX / diameterInCells);
	texArrayDesc.Height = std::ceil(cellsAlongY / diameterInCells);
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = count;
	texArrayDesc.Format = DXGI_FORMAT_R32_UINT;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;
	device->CreateTexture2D(&texArrayDesc, nullptr, &AA);
	device->CreateTexture2D(&texArrayDesc, nullptr, &AB);
	device->CreateTexture2D(&texArrayDesc, nullptr, &BB);
	device->CreateTexture2D(&texArrayDesc, nullptr, &AAfraction);
	device->CreateTexture2D(&texArrayDesc, nullptr, &ABfraction);
	device->CreateTexture2D(&texArrayDesc, nullptr, &BBfraction);
	device->CreateTexture2D(&texArrayDesc, nullptr, &maxA);
	device->CreateTexture2D(&texArrayDesc, nullptr, &maxB);

	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = count;
	device->CreateUnorderedAccessView(AA, &uav_desc, &AAuav);
	device->CreateUnorderedAccessView(AB, &uav_desc, &ABuav);
	device->CreateUnorderedAccessView(BB, &uav_desc, &BBuav);
	device->CreateUnorderedAccessView(AAfraction, &uav_desc, &AAfractionUAV);
	device->CreateUnorderedAccessView(ABfraction, &uav_desc, &ABfractionUAV);
	device->CreateUnorderedAccessView(BBfraction, &uav_desc, &BBfractionUAV);
	device->CreateUnorderedAccessView(maxA, &uav_desc, &maxAuav);
	device->CreateUnorderedAccessView(maxB, &uav_desc, &maxBuav);

	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = count;
	device->CreateShaderResourceView(AA, &srv_desc, &AAsrv);
	device->CreateShaderResourceView(AB, &srv_desc, &ABsrv);
	device->CreateShaderResourceView(BB, &srv_desc, &BBsrv);
	device->CreateShaderResourceView(AAfraction, &srv_desc, &AAfractionSRV);
	device->CreateShaderResourceView(ABfraction, &srv_desc, &ABfractionSRV);
	device->CreateShaderResourceView(BBfraction, &srv_desc, &BBfractionSRV);
	device->CreateShaderResourceView(maxA, &srv_desc, &maxAsrv);
	device->CreateShaderResourceView(maxB, &srv_desc, &maxBsrv);

	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.ByteWidth = count * sizeof(uint32_t);
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	buffer_desc.StructureByteStride = sizeof(uint32_t);
	device->CreateBuffer(&buffer_desc, nullptr, &mapAtoB);

	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_desc.Buffer.FirstElement = 0;
	srv_desc.Buffer.NumElements = count;
	device->CreateShaderResourceView(mapAtoB, &srv_desc, &mapAtoBsrv);

	texArrayDesc.Format = DXGI_FORMAT_R32_FLOAT;
	texArrayDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	device->CreateTexture2D(&texArrayDesc, nullptr, &error);

	texArrayDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	texArrayDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	device->CreateTexture2D(&texArrayDesc, nullptr, &AtoB);

	uav_desc.Format = DXGI_FORMAT_R32_FLOAT;
	device->CreateUnorderedAccessView(error, &uav_desc, &errorUAV);

	uav_desc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	device->CreateUnorderedAccessView(AtoB, &uav_desc, &AtoBuav);
}

void ModelMaker::defineTheSamePointsOnSetOfPhotos() 
{
	gridIntegrals.clearPhotosIntegrals(photosIntegralsAuav, width, height, texturesCount);
	gridIntegrals.calculatePhotosIntegrals(
		setOfPhotosSRV, photosIntegralsAuav,
		width, height, texturesCount,
		0, 1,
		0, 1,
		0, 0
	);

	operationsOnGridIntegrals.clearA(AAuav, AAfractionUAV, maxAuav, width, height, texturesCount);
	operationsOnGridIntegrals.calculateA(
		photosIntegralsAsrv, AAuav, AAfractionUAV, maxAuav,
		width, height, texturesCount
	);

	leastSquaresOfJacobianDeterminant.clear(errorUAV, AtoBuav, width, height, texturesCount);

	int maxOffset = 10;
	int N = 10;
	for (int offsetX = -maxOffset; offsetX <= maxOffset; offsetX++)
	{
		for (int offsetY = -maxOffset; offsetY <= maxOffset; offsetY++)
		{
			for (int a0i = 0; a0i <= N; a0i++)
			{
				for (int s0i = 0; s0i <= N; s0i++)
				{
					for (int a1i = 0; a1i <= N; a1i++)
					{
						for (int s1i = 0; s1i <= N; s1i++)
						{

						}
					}
				}
			}
		}
	}
}