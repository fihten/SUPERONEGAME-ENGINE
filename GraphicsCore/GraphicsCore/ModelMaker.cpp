#include "ModelMaker.h"
#include "GraphicsCore.h"
#include "Timer.h"
#include "Vec4d.h" 
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

void GridIntegralsA::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sGridIntegrals = std::string(shadersFolder) + "\\GridIntegralsA.fx";

	DWORD shaderFlags = 0;
	//#if defined(DEBUG) || defined(_DEBUG)
	//	shaderFlags |= D3D10_SHADER_DEBUG;
	//	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
	//#endif

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

	hClearGridIntegrals = hGridIntegralsFX->GetTechniqueByName("ClearGridIntegrals");
	hCalculateGridIntegrals = hGridIntegralsFX->GetTechniqueByName("CalculateGridIntegrals");

	hPhotos = hGridIntegralsFX->GetVariableByName("photos")->AsShaderResource();

	hPhotosIntegralsXho = hGridIntegralsFX->GetVariableByName("photosIntegralsXho")->AsUnorderedAccessView();
	hPhotosIntegralsYho = hGridIntegralsFX->GetVariableByName("photosIntegralsYho")->AsUnorderedAccessView();
	hPhotosIntegralsZho = hGridIntegralsFX->GetVariableByName("photosIntegralsZho")->AsUnorderedAccessView();

	hPhotosIntegralsXhi = hGridIntegralsFX->GetVariableByName("photosIntegralsXhi")->AsShaderResource();
	hPhotosIntegralsYhi = hGridIntegralsFX->GetVariableByName("photosIntegralsYhi")->AsShaderResource();
	hPhotosIntegralsZhi = hGridIntegralsFX->GetVariableByName("photosIntegralsZhi")->AsShaderResource();

	hPhotosIntegralsX = hGridIntegralsFX->GetVariableByName("photosIntegralsX")->AsUnorderedAccessView();
	hPhotosIntegralsY = hGridIntegralsFX->GetVariableByName("photosIntegralsY")->AsUnorderedAccessView();
	hPhotosIntegralsZ = hGridIntegralsFX->GetVariableByName("photosIntegralsZ")->AsUnorderedAccessView();

	hWidth = hGridIntegralsFX->GetVariableByName("width");
	hHeight = hGridIntegralsFX->GetVariableByName("height");
	hTexturesCount = hGridIntegralsFX->GetVariableByName("texturesCount");

	hCellDimensionX = hGridIntegralsFX->GetVariableByName("cellDimensionX");
	hCellDimensionY = hGridIntegralsFX->GetVariableByName("cellDimensionY");
}

void GridIntegralsA::clearPhotosIntegrals(
	ID3D11UnorderedAccessView* photosIntegralsXh,
	ID3D11UnorderedAccessView* photosIntegralsYh,
	ID3D11UnorderedAccessView* photosIntegralsZh,
	ID3D11UnorderedAccessView* photosIntegralsX,
	ID3D11UnorderedAccessView* photosIntegralsY,
	ID3D11UnorderedAccessView* photosIntegralsZ,
	int width, int height, int textureCount
)
{
	auto context = GraphicsCore::instance()->context;

	hPhotosIntegralsXho->SetUnorderedAccessView(photosIntegralsXh);
	hPhotosIntegralsYho->SetUnorderedAccessView(photosIntegralsYh);
	hPhotosIntegralsZho->SetUnorderedAccessView(photosIntegralsZh);

	hPhotosIntegralsX->SetUnorderedAccessView(photosIntegralsX);
	hPhotosIntegralsY->SetUnorderedAccessView(photosIntegralsY);
	hPhotosIntegralsZ->SetUnorderedAccessView(photosIntegralsZ);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&textureCount, 0, sizeof(textureCount));

	int cellDimensionX = CELL_DIMENSION_X;
	hCellDimensionX->SetRawValue(&cellDimensionX, 0, sizeof(cellDimensionX));

	int cellDimensionY = CELL_DIMENSION_Y;
	hCellDimensionY->SetRawValue(&cellDimensionY, 0, sizeof(cellDimensionY));

	hClearGridIntegrals->GetPassByIndex(0)->Apply(0, context);

	uint32_t groupsX = std::ceil((float)(width - cellDimensionX + 1) / 16);
	uint32_t groupsY = std::ceil((float)(height) / 16);
	uint32_t groupsZ = std::ceil((float)(textureCount) / 4);

	context->Dispatch(groupsX, groupsY, groupsZ);

	hClearGridIntegrals->GetPassByIndex(1)->Apply(0, context);

	groupsX = std::ceil((float)(width - cellDimensionX + 1) / 16);
	groupsY = std::ceil((float)(height - cellDimensionY + 1) / 16);
	groupsZ = std::ceil((float)(textureCount) / 4);

	context->Dispatch(groupsX, groupsY, groupsZ);

	ID3D11ShaderResourceView* nullSRVs[4] = { nullptr,nullptr,nullptr,nullptr };
	context->CSSetShaderResources(0, 4, nullSRVs);

	ID3D11UnorderedAccessView* nullUAVs[6] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	context->CSSetUnorderedAccessViews(0, 6, nullUAVs, 0);
}

void GridIntegralsA::calculatePhotosIntegrals(
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
)
{
	auto context = GraphicsCore::instance()->context;

	hPhotos->SetResource(photos);

	hPhotosIntegralsXho->SetUnorderedAccessView(photosIntegralsXho);
	hPhotosIntegralsYho->SetUnorderedAccessView(photosIntegralsYho);
	hPhotosIntegralsZho->SetUnorderedAccessView(photosIntegralsZho);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&textureCount, 0, sizeof(textureCount));

	int cellDimensionX = CELL_DIMENSION_X;
	hCellDimensionX->SetRawValue(&cellDimensionX, 0, sizeof(cellDimensionX));

	int cellDimensionY = CELL_DIMENSION_Y;
	hCellDimensionY->SetRawValue(&cellDimensionY, 0, sizeof(cellDimensionY));

	hCalculateGridIntegrals->GetPassByIndex(0)->Apply(0, context);

	uint32_t groupsX = std::ceil((float)(width - cellDimensionX + 1) / 16);
	uint32_t groupsY = std::ceil((float)(height) / 16);
	uint32_t groupsZ = std::ceil((float)(textureCount) / 4);

	context->Dispatch(groupsX, groupsY, groupsZ);

	ID3D11ShaderResourceView* nullSRVs[4] = { nullptr,nullptr,nullptr,nullptr };
	context->CSSetShaderResources(0, 4, nullSRVs);

	ID3D11UnorderedAccessView* nullUAVs[6] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	context->CSSetUnorderedAccessViews(0, 6, nullUAVs, 0);

	hPhotosIntegralsXhi->SetResource(photosIntegralsXhi);
	hPhotosIntegralsYhi->SetResource(photosIntegralsYhi);
	hPhotosIntegralsZhi->SetResource(photosIntegralsZhi);

	hPhotosIntegralsX->SetUnorderedAccessView(photosIntegralsX);
	hPhotosIntegralsY->SetUnorderedAccessView(photosIntegralsY);
	hPhotosIntegralsZ->SetUnorderedAccessView(photosIntegralsZ);

	hCalculateGridIntegrals->GetPassByIndex(1)->Apply(0, context);

	groupsX = std::ceil((float)(width - cellDimensionX + 1) / 16);
	groupsY = std::ceil((float)(height - cellDimensionY + 1) / 16);
	groupsZ = std::ceil((float)(textureCount) / 4);

	context->Dispatch(groupsX, groupsY, groupsZ);

	context->CSSetShaderResources(0, 4, nullSRVs);
	context->CSSetUnorderedAccessViews(0, 6, nullUAVs, 0);
}

void GridIntegralsB::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sGridIntegrals = std::string(shadersFolder) + "\\GridIntegralsB.fx";

	DWORD shaderFlags = 0;
//#if defined(DEBUG) || defined(_DEBUG)
//	shaderFlags |= D3D10_SHADER_DEBUG;
//	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif

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
	hPhotosIntegralsX = hGridIntegralsFX->GetVariableByName("photosIntegralsX")->AsUnorderedAccessView();
	hPhotosIntegralsY = hGridIntegralsFX->GetVariableByName("photosIntegralsY")->AsUnorderedAccessView();
	hPhotosIntegralsZ = hGridIntegralsFX->GetVariableByName("photosIntegralsZ")->AsUnorderedAccessView();

	hWidth = hGridIntegralsFX->GetVariableByName("width");
	hHeight = hGridIntegralsFX->GetVariableByName("height");
	hTexturesCount = hGridIntegralsFX->GetVariableByName("texturesCount");

	hAngle0 = hGridIntegralsFX->GetVariableByName("angle0");
	hScale0 = hGridIntegralsFX->GetVariableByName("scale0");

	hAngle1 = hGridIntegralsFX->GetVariableByName("angle1");
	hScale1 = hGridIntegralsFX->GetVariableByName("scale1");

	hCellDimensionX = hGridIntegralsFX->GetVariableByName("cellDimensionX");
	hCellDimensionY = hGridIntegralsFX->GetVariableByName("cellDimensionY");
}

void GridIntegralsB::clearPhotosIntegrals(
	ID3D11UnorderedAccessView* photosIntegralsX,
	ID3D11UnorderedAccessView* photosIntegralsY,
	ID3D11UnorderedAccessView* photosIntegralsZ,
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

	hPhotosIntegralsX->SetUnorderedAccessView(photosIntegralsX);
	hPhotosIntegralsY->SetUnorderedAccessView(photosIntegralsY);
	hPhotosIntegralsZ->SetUnorderedAccessView(photosIntegralsZ);

	// define number of thread groups

	int cellsAlongX = std::ceil((float)width / cellDimensionX);
	int cellsAlongY = std::ceil((float)height / cellDimensionY);

	uint32_t groups_x = std::ceil((float)(cellsAlongX) / 16);
	uint32_t groups_y = std::ceil((float)(cellsAlongY) / 16);
	uint32_t groups_z = std::ceil((float)(count) / 4);

	hClearGridIntegralsTech->GetPassByName("P0")->Apply(0, context);
	context->Dispatch(groups_x, groups_y, groups_z);

	ID3D11UnorderedAccessView* uavsNull[] = { nullptr,nullptr,nullptr };
	ID3D11ShaderResourceView* srvsNull = nullptr;
	context->CSSetShaderResources(0, 1, &srvsNull);
	context->CSSetUnorderedAccessViews(0, 3, uavsNull, nullptr);
}

void GridIntegralsB::calculatePhotosIntegrals(
	ID3D11ShaderResourceView* photos,
	ID3D11UnorderedAccessView* photosIntegralsX,
	ID3D11UnorderedAccessView* photosIntegralsY,
	ID3D11UnorderedAccessView* photosIntegralsZ,
	int width, int height, int count,
	float angle0, float scale0,
	float angle1, float scale1
)
{
	auto context = GraphicsCore::instance()->context;

	hPhotos->SetResource(photos);
	hPhotosIntegralsX->SetUnorderedAccessView(photosIntegralsX);
	hPhotosIntegralsY->SetUnorderedAccessView(photosIntegralsY);
	hPhotosIntegralsZ->SetUnorderedAccessView(photosIntegralsZ);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	hAngle0->SetRawValue(&angle0, 0, sizeof(angle0));
	hScale0->SetRawValue(&scale0, 0, sizeof(scale0));

	hAngle1->SetRawValue(&angle1, 0, sizeof(angle1));
	hScale1->SetRawValue(&scale1, 0, sizeof(scale1));

	int cellDimensionX = CELL_DIMENSION_X;
	hCellDimensionX->SetRawValue(&cellDimensionX, 0, sizeof(cellDimensionX));

	int cellDimensionY = CELL_DIMENSION_Y;
	hCellDimensionY->SetRawValue(&cellDimensionY, 0, sizeof(cellDimensionY));

	hCalculateGridIntegralsTech->GetPassByName("P0")->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(width) / 16);
	uint32_t groups_y = std::ceil((float)(height) / 16);
	uint32_t groups_z = std::ceil((float)(count) / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	ID3D11UnorderedAccessView* uavsNull[] = { nullptr,nullptr,nullptr };
	ID3D11ShaderResourceView* srvsNull = nullptr;
	context->CSSetShaderResources(0, 1, &srvsNull);
	context->CSSetUnorderedAccessViews(0, 3, uavsNull, nullptr);
}

void OperationsOnGridIntegrals::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sOperationsOnGridIntegrals = std::string(shadersFolder) + "\\OperationsOnGridIntegrals.fx";

	DWORD shaderFlags = 0;
//#if defined(DEBUG) || defined(_DEBUG)
//	shaderFlags |= D3D10_SHADER_DEBUG;
//	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif

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

	hPhotosIntegralsAx = hOperationsOnGridIntegralsFX->GetVariableByName("photosIntegralsAx")->AsShaderResource();
	hPhotosIntegralsAy = hOperationsOnGridIntegralsFX->GetVariableByName("photosIntegralsAy")->AsShaderResource();
	hPhotosIntegralsAz = hOperationsOnGridIntegralsFX->GetVariableByName("photosIntegralsAz")->AsShaderResource();

	hPhotosIntegralsBx = hOperationsOnGridIntegralsFX->GetVariableByName("photosIntegralsBx")->AsShaderResource();
	hPhotosIntegralsBy = hOperationsOnGridIntegralsFX->GetVariableByName("photosIntegralsBy")->AsShaderResource();
	hPhotosIntegralsBz = hOperationsOnGridIntegralsFX->GetVariableByName("photosIntegralsBz")->AsShaderResource();

	hAA = hOperationsOnGridIntegralsFX->GetVariableByName("AA")->AsUnorderedAccessView();
	hAB = hOperationsOnGridIntegralsFX->GetVariableByName("AB")->AsUnorderedAccessView();
	hBB = hOperationsOnGridIntegralsFX->GetVariableByName("BB")->AsUnorderedAccessView();

	hAAfraction = hOperationsOnGridIntegralsFX->GetVariableByName("AAfraction")->AsUnorderedAccessView();
	hABfraction = hOperationsOnGridIntegralsFX->GetVariableByName("ABfraction")->AsUnorderedAccessView();
	hBBfraction = hOperationsOnGridIntegralsFX->GetVariableByName("BBfraction")->AsUnorderedAccessView();

	hMaxA = hOperationsOnGridIntegralsFX->GetVariableByName("maxA")->AsUnorderedAccessView();
	hMaxB = hOperationsOnGridIntegralsFX->GetVariableByName("maxB")->AsUnorderedAccessView();

	hWidthA = hOperationsOnGridIntegralsFX->GetVariableByName("widthA");
	hHeightA = hOperationsOnGridIntegralsFX->GetVariableByName("heightA");

	hWidthAreal = hOperationsOnGridIntegralsFX->GetVariableByName("widthAreal");
	hHeightAreal = hOperationsOnGridIntegralsFX->GetVariableByName("heightAreal");

	hWidthB = hOperationsOnGridIntegralsFX->GetVariableByName("widthB");
	hHeightB = hOperationsOnGridIntegralsFX->GetVariableByName("heightB");

	hWidthAA = hOperationsOnGridIntegralsFX->GetVariableByName("widthAA");
	hWidthAB = hOperationsOnGridIntegralsFX->GetVariableByName("widthAB");
	hWidthABreal = hOperationsOnGridIntegralsFX->GetVariableByName("widthABreal");
	hWidthBB = hOperationsOnGridIntegralsFX->GetVariableByName("widthBB");

	hHeightAA = hOperationsOnGridIntegralsFX->GetVariableByName("heightAA");
	hHeightAB = hOperationsOnGridIntegralsFX->GetVariableByName("heightAB");
	hHeightABreal = hOperationsOnGridIntegralsFX->GetVariableByName("heightABreal");
	hHeightBB = hOperationsOnGridIntegralsFX->GetVariableByName("heightBB");

	hTexturesCount = hOperationsOnGridIntegralsFX->GetVariableByName("texturesCount");

	hCellDimension = hOperationsOnGridIntegralsFX->GetVariableByName("cellDimension");
	hOffsetRange = hOperationsOnGridIntegralsFX->GetVariableByName("offsetRange");
	hOffset0 = hOperationsOnGridIntegralsFX->GetVariableByName("offset0");

	hRadius = hOperationsOnGridIntegralsFX->GetVariableByName("radius");
	hIndexOfA = hOperationsOnGridIntegralsFX->GetVariableByName("indexOfA");
}

void OperationsOnGridIntegrals::unboundViews()
{
	auto context = GraphicsCore::instance()->context;

	ID3D11ShaderResourceView* nullSRVs[6] = {
		nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	context->CSSetShaderResources(0, 6, nullSRVs);

	ID3D11UnorderedAccessView* nullUAVs[8] = {
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr
	};
	context->CSSetUnorderedAccessViews(0, 8, nullUAVs, nullptr);
}

void OperationsOnGridIntegrals::clearAAandMaxA(
	ID3D11UnorderedAccessView* AA,
	ID3D11UnorderedAccessView* AAfraction,
	ID3D11UnorderedAccessView* maxA,
	int widthAA, int heightAA, int texturesCount
)
{
	hAA->SetUnorderedAccessView(AA);
	hAAfraction->SetUnorderedAccessView(AAfraction);
	hMaxA->SetUnorderedAccessView(maxA);

	hWidthAA->SetRawValue(&widthAA, 0, sizeof(widthAA));
	hHeightAA->SetRawValue(&heightAA, 0, sizeof(heightAA));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	auto context = GraphicsCore::instance()->context;
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthAA / 16);
	uint32_t groups_y = std::ceil((float)heightAA / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);
	unboundViews();
}

void OperationsOnGridIntegrals::clearBBandMaxB(
	ID3D11UnorderedAccessView* BB,
	ID3D11UnorderedAccessView* BBfraction,
	ID3D11UnorderedAccessView* maxB,
	int widthBB, int heightBB, int texturesCount
)
{
	hBB->SetUnorderedAccessView(BB);
	hBBfraction->SetUnorderedAccessView(BBfraction);
	hMaxB->SetUnorderedAccessView(maxB);

	hWidthBB->SetRawValue(&widthBB, 0, sizeof(widthBB));
	hHeightBB->SetRawValue(&heightBB, 0, sizeof(heightBB));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	auto context = GraphicsCore::instance()->context;
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(1)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthBB / 16);
	uint32_t groups_y = std::ceil((float)heightBB / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);
	unboundViews();
}

void LeastSquaresOfJacobianDeterminant::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sLeastSquaresOfJacobianDeterminant = std::string(shadersFolder) + "\\LeastSquaresOfJacobianDeterminant.fx";

	DWORD shaderFlags = 0;
//#if defined(DEBUG) || defined(_DEBUG)
//	shaderFlags |= D3D10_SHADER_DEBUG;
//	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
//#endif

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

	hWidthAA = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("widthAA");
	hWidthAB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("widthAB");
	hWidthBB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("widthBB");

	hHeightAA = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("heightAA");
	hHeightAB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("heightAB");
	hHeightBB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("heightBB");

	hTexturesCount = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("texturesCount");

	hAngle0 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("angle0");
	hScale0 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("scale0");

	hAngle1 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("angle1");
	hScale1 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("scale1");

	hCellRadius = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("cellRadius");
	hCellDimension = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("cellDimension");
	hOffsetRange = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("offsetRange");
	hOffset0 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("offset0");

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

	int diameter = 2 * RADIUS_IN_CELLS + 1;

	int widthAA = width - CELL_DIMENSION_X + 1;
	widthAA = std::ceil((float)(widthAA) / diameter);

	int heightAA = height - CELL_DIMENSION_Y + 1;
	heightAA = std::ceil((float)(heightAA) / diameter);

	hWidthAA->SetRawValue(&widthAA, 0, sizeof(widthAA));
	hHeightAA->SetRawValue(&heightAA, 0, sizeof(heightAA));

	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	hError->SetUnorderedAccessView(error);
	hAtoB->SetUnorderedAccessView(AtoB);

	hClearTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthAA / 16);
	uint32_t groups_y = std::ceil((float)heightAA / 16);
	uint32_t groups_z = std::ceil((float)count / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	ID3D11UnorderedAccessView* uavsNull[] = {
		nullptr,nullptr };
	ID3D11ShaderResourceView* srvsNull[] = {
		nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	context->CSSetShaderResources(0, 9, srvsNull);
	context->CSSetUnorderedAccessViews(0, 2, uavsNull, nullptr);
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
	float angle1, float scale1
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

	int radius = RADIUS_IN_CELLS;
	int diameter = 2 * radius + 1;
	
	int widthAB = width - CELL_DIMENSION_X + 1;
	widthAB = std::ceil((float)(widthAB) / CELL_DIMENSION_X);
	widthAB *= OFFSET_RANGE_X;
	widthAB = std::ceil((float)(widthAB) / diameter);

	int heightAB = height - CELL_DIMENSION_Y + 1;
	heightAB = std::ceil((float)(heightAB) / CELL_DIMENSION_Y);
	heightAB *= OFFSET_RANGE_Y;
	heightAB = std::ceil((float)(heightAB) / diameter);

	hWidthAB->SetRawValue(&widthAB, 0, sizeof(widthAB));
	hHeightAB->SetRawValue(&heightAB, 0, sizeof(heightAB));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	hAngle0->SetRawValue(&angle0, 0, sizeof(angle0));
	hScale0->SetRawValue(&scale0, 0, sizeof(scale0));

	hAngle1->SetRawValue(&angle0, 0, sizeof(angle1));
	hScale1->SetRawValue(&scale0, 0, sizeof(scale1));

	Vec2d<int> cellDimension{ CELL_DIMENSION_X,CELL_DIMENSION_Y };
	hCellDimension->SetRawValue(&cellDimension, 0, sizeof(cellDimension));
	
	Vec2d<int> offsetRange{ OFFSET_RANGE_X,OFFSET_RANGE_Y };
	hOffsetRange->SetRawValue(&offsetRange, 0, sizeof(offsetRange));

	Vec2d<int> offset0{ OFFSET0_X,OFFSET0_Y };
	hOffset0->SetRawValue(&offset0, 0, sizeof(offset0));

	hApplyLeastSquareMethodTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthAB / 16);
	uint32_t groups_y = std::ceil((float)heightAB / 16);
	uint32_t groups_z = std::ceil((float)count / 4);
	context->Dispatch(groups_x, groups_y, groups_z);

	ID3D11UnorderedAccessView* uavsNull[] = {
		nullptr,nullptr };
	ID3D11ShaderResourceView* srvsNull[] = {
		nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	context->CSSetShaderResources(0, 9, srvsNull);
	context->CSSetUnorderedAccessViews(0, 2, uavsNull, nullptr);
}

void FindNearestDefinedPoint::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sFindNearestDefinedPoint = std::string(shadersFolder) + "\\findNearestDefinedPoint.fx";

	DWORD shaderFlags = 0;
	//#if defined(DEBUG) || defined(_DEBUG)
	//	shaderFlags |= D3D10_SHADER_DEBUG;
	//	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
	//#endif

	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFileA(sFindNearestDefinedPoint.c_str(), 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return;
	}
	auto device = GraphicsCore::instance()->device;
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &hEffect);

	hTechnique = hEffect->GetTechniqueByName("findNearestDefinedPoint");
	hError = hEffect->GetVariableByName("error")->AsShaderResource();
	hAtoB = hEffect->GetVariableByName("AtoB")->AsShaderResource();
	hMinDistanceOut = hEffect->GetVariableByName("minDistanceOut")->AsUnorderedAccessView();
	hMinDistanceIn = hEffect->GetVariableByName("minDistanceIn")->AsShaderResource();
	hMappingOfPoint = hEffect->GetVariableByName("mappingOfPoint")->AsUnorderedAccessView();
	hWidth = hEffect->GetVariableByName("width");
	hHeight = hEffect->GetVariableByName("height");
	hIndex = hEffect->GetVariableByName("index");
	hPt = hEffect->GetVariableByName("pt");
	hThreshold = hEffect->GetVariableByName("threshold");

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(uint32_t);
	device->CreateBuffer(&bufferDesc, nullptr, &minDistance);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 1;
	device->CreateUnorderedAccessView(minDistance, &uavDesc, &minDistanceUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.NumElements = 1;
	srvDesc.Buffer.ElementWidth = sizeof(uint32_t);
	device->CreateShaderResourceView(minDistance, &srvDesc, &minDistanceSRV);

	bufferDesc.ByteWidth = 4 * sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(uint32_t);
	device->CreateBuffer(&bufferDesc, nullptr, &mappingOfPoint);

	bufferDesc.Usage = D3D11_USAGE_STAGING;
	bufferDesc.BindFlags = 0;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	device->CreateBuffer(&bufferDesc, nullptr, &mappingOfPointCopy);

	uavDesc.Buffer.NumElements = 4;
	device->CreateUnorderedAccessView(mappingOfPoint, &uavDesc, &mappingOfPointUAV);
}

Vec2d<int> FindNearestDefinedPoint::findNearestPoint(
	ID3D11ShaderResourceView* error,
	ID3D11ShaderResourceView* AtoB,
	const Vec2d<int>& point
)
{
	auto context = GraphicsCore::instance()->context;

	hError->SetResource(error);
	hAtoB->SetResource(AtoB);

	uint32_t initialMinDistance = 0xffffffff;
	context->UpdateSubresource(minDistance, 0, nullptr, &initialMinDistance, 0, 0);

	hMinDistanceOut->SetUnorderedAccessView(minDistanceUAV);

	ID3D11Texture2D* errorTex = nullptr;
	error->GetResource((ID3D11Resource * *)(&errorTex));

	D3D11_TEXTURE2D_DESC texDesc;
	errorTex->GetDesc(&texDesc);

	hWidth->SetRawValue(&texDesc.Width, 0, sizeof(texDesc.Width));
	hHeight->SetRawValue(&texDesc.Height, 0, sizeof(texDesc.Height));
	
	int index = 0;
	hIndex->SetRawValue(&index, 0, sizeof(index));

	hPt->SetRawValue(&point, 0, sizeof(point));

	float threshold = 0.1f;
	hThreshold->SetRawValue(&threshold, 0, sizeof(threshold));

	hTechnique->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(texDesc.Width) / 16);
	uint32_t groups_y = std::ceil((float)(texDesc.Height) / 16);
	uint32_t groups_z = 1;

	context->Dispatch(groups_x, groups_y, groups_z);

	ID3D11ShaderResourceView* nullSRVs[3] = { nullptr,nullptr,nullptr };
	ID3D11UnorderedAccessView* nullUAVs[2] = { nullptr,nullptr };

	context->CSSetShaderResources(0, 3, nullSRVs);
	context->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);

	hError->SetResource(error);
	hAtoB->SetResource(AtoB);
	hMinDistanceIn->SetResource(minDistanceSRV);
	hMappingOfPoint->SetUnorderedAccessView(mappingOfPointUAV);

	hWidth->SetRawValue(&texDesc.Width, 0, sizeof(texDesc.Width));
	hHeight->SetRawValue(&texDesc.Height, 0, sizeof(texDesc.Height));
	hIndex->SetRawValue(&index, 0, sizeof(index));
	hPt->SetRawValue(&point, 0, sizeof(point));
	hThreshold->SetRawValue(&threshold, 0, sizeof(threshold));

	hTechnique->GetPassByIndex(1)->Apply(0, context);
	context->Dispatch(groups_x, groups_y, groups_z);

	context->CSSetShaderResources(0, 3, nullSRVs);
	context->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);

	context->CopyResource(mappingOfPointCopy, mappingOfPoint);

	D3D11_MAPPED_SUBRESOURCE data;
	context->Map(mappingOfPointCopy, 0, D3D11_MAP_READ, 0, &data);

	Vec4d<uint32_t> mapping = *((Vec4d<uint32_t>*)data.pData);
	Vec2d<int> res{ mapping.z() >> 16, mapping.z() & 0xffff };

	context->Unmap(mappingOfPointCopy, 0);

	return res;
}

void ModelMaker::init()
{
	ptr->gridIntegralsA.init();
	ptr->gridIntegralsB.init();
	ptr->operationsOnGridIntegrals.init();
	ptr->leastSquaresOfJacobianDeterminant.init();
	ptr->findNearestDefinedPoint.init();
}

void ModelMaker::loadPhotos(const std::vector<std::string>& paths)
{
	freeResources(true);

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

	texArrayDesc.Width = width - CELL_DIMENSION_X + 1;
	texArrayDesc.Height = height;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = count;
	texArrayDesc.Format = DXGI_FORMAT_R32_UINT;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAxh);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAyh);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAzh);
	
	texArrayDesc.Height = height - CELL_DIMENSION_Y + 1;
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAx);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAy);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAz);

	int cellsAlongX = std::ceil((float)(width) / CELL_DIMENSION_X);
	int cellsAlongY = std::ceil((float)(height) / CELL_DIMENSION_Y);
	texArrayDesc.Width = cellsAlongX;
	texArrayDesc.Height = cellsAlongY;

	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsBx);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsBy);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsBz);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = count;
	device->CreateUnorderedAccessView(photosIntegralsAxh, &uav_desc, &photosIntegralsAxhUAV);
	device->CreateUnorderedAccessView(photosIntegralsAyh, &uav_desc, &photosIntegralsAyhUAV);
	device->CreateUnorderedAccessView(photosIntegralsAzh, &uav_desc, &photosIntegralsAzhUAV);
	device->CreateUnorderedAccessView(photosIntegralsAx, &uav_desc, &photosIntegralsAxUAV);
	device->CreateUnorderedAccessView(photosIntegralsAy, &uav_desc, &photosIntegralsAyUAV);
	device->CreateUnorderedAccessView(photosIntegralsAz, &uav_desc, &photosIntegralsAzUAV);
	device->CreateUnorderedAccessView(photosIntegralsBx, &uav_desc, &photosIntegralsBxUAV);
	device->CreateUnorderedAccessView(photosIntegralsBy, &uav_desc, &photosIntegralsByUAV);
	device->CreateUnorderedAccessView(photosIntegralsBz, &uav_desc, &photosIntegralsBzUAV);

	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = count;
	device->CreateShaderResourceView(photosIntegralsAxh, &srv_desc, &photosIntegralsAxhSRV);
	device->CreateShaderResourceView(photosIntegralsAyh, &srv_desc, &photosIntegralsAyhSRV);
	device->CreateShaderResourceView(photosIntegralsAzh, &srv_desc, &photosIntegralsAzhSRV);
	device->CreateShaderResourceView(photosIntegralsAx, &srv_desc, &photosIntegralsAxSRV);
	device->CreateShaderResourceView(photosIntegralsAy, &srv_desc, &photosIntegralsAySRV);
	device->CreateShaderResourceView(photosIntegralsAz, &srv_desc, &photosIntegralsAzSRV);
	device->CreateShaderResourceView(photosIntegralsBx, &srv_desc, &photosIntegralsBxSRV);
	device->CreateShaderResourceView(photosIntegralsBy, &srv_desc, &photosIntegralsBySRV);
	device->CreateShaderResourceView(photosIntegralsBz, &srv_desc, &photosIntegralsBzSRV);

	int diameter = 2 * RADIUS_IN_CELLS + 1;

	int widthAA = width - CELL_DIMENSION_X + 1;
	widthAA = std::ceil((float)(widthAA) / diameter);

	int heightAA = height - CELL_DIMENSION_Y + 1;
	heightAA = std::ceil((float)(heightAA) / diameter);

	int widthAB = width - CELL_DIMENSION_X + 1;
	widthAB = std::ceil((float)(widthAB) / CELL_DIMENSION_X);
	widthAB *= OFFSET_RANGE_X;
	widthAB = std::ceil((float)(widthAB) / diameter);

	int heightAB = height - CELL_DIMENSION_Y + 1;
	heightAB = std::ceil((float)(heightAB) / CELL_DIMENSION_Y);
	heightAB *= OFFSET_RANGE_Y;
	heightAB = std::ceil((float)(heightAB) / diameter);

	int widthBB = std::ceil((float)(width) / diameter);
	int heightBB = std::ceil((float)(height) / diameter);

	texArrayDesc.Width = widthAA;
	texArrayDesc.Height = heightAA;
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
	device->CreateTexture2D(&texArrayDesc, nullptr, &AAfraction);
	device->CreateTexture2D(&texArrayDesc, nullptr, &maxA);

	texArrayDesc.Width = widthAB;
	texArrayDesc.Height = heightAB;
	device->CreateTexture2D(&texArrayDesc, nullptr, &AB);
	device->CreateTexture2D(&texArrayDesc, nullptr, &ABfraction);

	texArrayDesc.Width = widthBB;
	texArrayDesc.Height = heightBB;
	device->CreateTexture2D(&texArrayDesc, nullptr, &BB);
	device->CreateTexture2D(&texArrayDesc, nullptr, &BBfraction);
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

	D3D11_SUBRESOURCE_DATA data;
	std::vector<uint32_t> vMapAtoB(texturesCount);
	for (uint32_t i = 0; i < texturesCount; i++)
	{
		vMapAtoB[i] = texturesCount - 1 - i;
	}
	data.pSysMem = vMapAtoB.data();
	device->CreateBuffer(&buffer_desc, &data, &mapAtoB);

	srv_desc.Format = DXGI_FORMAT_UNKNOWN;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srv_desc.Buffer.FirstElement = 0;
	srv_desc.Buffer.NumElements = count;
	device->CreateShaderResourceView(mapAtoB, &srv_desc, &mapAtoBsrv);

	texArrayDesc.Width = widthAA;
	texArrayDesc.Height = heightAA;
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

	bSetOfPhotosIsLoaded = true;
}

void ModelMaker::defineTheSamePointsOnSetOfPhotos() 
{
	float t0 = Timer::Instance()->elapsedTime();

	gridIntegralsA.clearPhotosIntegrals(
		photosIntegralsAxhUAV,
		photosIntegralsAyhUAV,
		photosIntegralsAzhUAV,
		photosIntegralsAxUAV,
		photosIntegralsAyUAV,
		photosIntegralsAzUAV,
		width, height, texturesCount);
	gridIntegralsA.calculatePhotosIntegrals(
		setOfPhotosSRV,
		photosIntegralsAxhUAV,
		photosIntegralsAyhUAV,
		photosIntegralsAzhUAV,
		photosIntegralsAxhSRV,
		photosIntegralsAyhSRV,
		photosIntegralsAzhSRV,
		photosIntegralsAxUAV,
		photosIntegralsAyUAV,
		photosIntegralsAzUAV,
		width, height, texturesCount
	);

	operationsOnGridIntegrals.clearA(
		AAuav, AAfractionUAV, maxAuav, width, height, texturesCount);
	operationsOnGridIntegrals.calculateA(
		photosIntegralsAxSRV,
		photosIntegralsAySRV,
		photosIntegralsAzSRV,
		AAuav, AAfractionUAV, maxAuav,
		width, height, texturesCount
	);

	leastSquaresOfJacobianDeterminant.clear(errorUAV, AtoBuav, width, height, texturesCount);

	int maxOffset = 10;
	int N = 10;

	int countOfSteps = (N + 1) * (N + 1) * (N + 1) * (N + 1);
	int elapsedSteps = 0;
	int percent = 0;

	float scaleMin = 0.8f;
	float scaleMax = 1.2f;
	float scaleStep = (scaleMax - scaleMin) / N;

	float angleMin = -M_PI / 12;
	float angleMax = M_PI / 12;
	float angleStep = (angleMax - angleMin) / N;
	
	for (int a0i = 0; a0i <= N; a0i++)
	{
		for (int s0i = 0; s0i <= N; s0i++)
		{
			for (int a1i = 0; a1i <= N; a1i++)
			{
				for (int s1i = 0; s1i <= N; s1i++)
				{
					int p = (float)(elapsedSteps * 100) / countOfSteps;
					if (p > percent)
					{
						percent = p;
						char buffer[256];
						sprintf(buffer, "percent=%d%\n", percent);
						OutputDebugStringA(buffer);
					}

					float angle0 = angleMin + a0i * angleStep;
					float scale0 = scaleMin + s0i * scaleStep;

					float angle1 = angleMin + a1i * angleStep;
					float scale1 = scaleMin + s1i * scaleStep;

					gridIntegralsB.clearPhotosIntegrals(
						photosIntegralsBxUAV,
						photosIntegralsByUAV,
						photosIntegralsBzUAV,
						width, height, texturesCount);
					gridIntegralsB.calculatePhotosIntegrals(
						setOfPhotosSRV,
						photosIntegralsBxUAV,
						photosIntegralsByUAV,
						photosIntegralsBzUAV,
						width, height, texturesCount,
						angle0, scale0,
						angle1 + angle0, scale1
					);
					operationsOnGridIntegrals.calculateAB(
						photosIntegralsAxSRV,
						photosIntegralsAySRV,
						photosIntegralsAzSRV,
						photosIntegralsBxSRV,
						photosIntegralsBySRV,
						photosIntegralsBzSRV,
						mapAtoBsrv,
						ABuav, ABfractionUAV,
						BBuav, BBfractionUAV,
						maxBuav,
						width, height, texturesCount
					);
					leastSquaresOfJacobianDeterminant.calculateErrors(
						AAsrv, ABsrv, BBsrv,
						AAfractionSRV, ABfractionSRV, BBfractionSRV,
						maxAsrv, maxBsrv, mapAtoBsrv,
						errorUAV, AtoBuav,
						width, height, texturesCount,
						angle0, scale0,
						angle1 + angle0, scale1
					);
					elapsedSteps++;
				}
			}
		}
	}

	float t1 = Timer::Instance()->elapsedTime();
	float dt = t1 - t0;
	char buffer[256];
	sprintf(buffer, "seconds=%d\n", (int)dt);
	OutputDebugStringA(buffer);
}

Vec2d<int> ModelMaker::findTheSamePoint(const Vec2d<int>& pt)
{

}

void ModelMaker::freeResources(bool freeResult)
{
	if (bSetOfPhotosIsLoaded)
	{
		setOfPhotos->Release();
		setOfPhotos = nullptr;

		setOfPhotosSRV->Release();
		setOfPhotosSRV = nullptr;

		photosIntegralsAxh->Release();
		photosIntegralsAxh = nullptr;

		photosIntegralsAyh->Release();
		photosIntegralsAyh = nullptr;

		photosIntegralsAzh->Release();
		photosIntegralsAzh = nullptr;

		photosIntegralsAx->Release();
		photosIntegralsAx = nullptr;

		photosIntegralsAy->Release();
		photosIntegralsAy = nullptr;

		photosIntegralsAz->Release();
		photosIntegralsAz = nullptr;

		photosIntegralsAxhUAV->Release();
		photosIntegralsAxhUAV = nullptr;

		photosIntegralsAyhUAV->Release();
		photosIntegralsAyhUAV = nullptr;

		photosIntegralsAzhUAV->Release();
		photosIntegralsAzhUAV = nullptr;

		photosIntegralsAxUAV->Release();
		photosIntegralsAxUAV = nullptr;

		photosIntegralsAyUAV->Release();
		photosIntegralsAyUAV = nullptr;

		photosIntegralsAzUAV->Release();
		photosIntegralsAzUAV = nullptr;

		photosIntegralsAxhSRV->Release();
		photosIntegralsAxhSRV = nullptr;

		photosIntegralsAyhSRV->Release();
		photosIntegralsAyhSRV = nullptr;

		photosIntegralsAzhSRV->Release();
		photosIntegralsAzhSRV = nullptr;

		photosIntegralsAxSRV->Release();
		photosIntegralsAxSRV = nullptr;

		photosIntegralsAySRV->Release();
		photosIntegralsAySRV = nullptr;

		photosIntegralsAzSRV->Release();
		photosIntegralsAzSRV = nullptr;

		photosIntegralsBx->Release();
		photosIntegralsBx = nullptr;

		photosIntegralsBy->Release();
		photosIntegralsBy = nullptr;

		photosIntegralsBz->Release();
		photosIntegralsBz = nullptr;

		photosIntegralsBxUAV->Release();
		photosIntegralsBxUAV = nullptr;

		photosIntegralsByUAV->Release();
		photosIntegralsByUAV = nullptr;

		photosIntegralsBzUAV->Release();
		photosIntegralsBzUAV = nullptr;

		photosIntegralsBxSRV->Release();
		photosIntegralsBxSRV = nullptr;

		photosIntegralsBySRV->Release();
		photosIntegralsBySRV = nullptr;

		photosIntegralsBzSRV->Release();
		photosIntegralsBzSRV = nullptr;

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

		if (freeResult)
		{
			error->Release();
			error = nullptr;

			errorUAV->Release();
			errorUAV = nullptr;

			AtoB->Release();
			AtoB = nullptr;

			AtoBuav->Release();
			AtoBuav = nullptr;
		}

		bSetOfPhotosIsLoaded = false;
	}
}