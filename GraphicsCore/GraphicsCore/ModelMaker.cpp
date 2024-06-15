#include "ModelMaker.h"
#include "GraphicsCore.h"
#include "Timer.h"
#include "Vec4d.h" 
#include <D3DX11.h>
#include <algorithm>

std::unique_ptr<ModelMaker> ModelMaker::ptr;

ModelMaker* ModelMaker::instance()
{
	if (!ptr)
	{
		ptr = std::make_unique<ModelMaker>();
	}
	return ptr.get();
}

void GridIntegralsA::unboundViews()
{
	auto context = GraphicsCore::instance()->context;

	ID3D11ShaderResourceView* nullSRVs[4] = { nullptr,nullptr,nullptr,nullptr };
	context->CSSetShaderResources(0, 4, nullSRVs);

	ID3D11UnorderedAccessView* nullUAVs[6] = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	context->CSSetUnorderedAccessViews(0, 6, nullUAVs, 0);
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

	hWidthA = hGridIntegralsFX->GetVariableByName("widthA");
	hHeightA = hGridIntegralsFX->GetVariableByName("heightA");

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
	int widthA, int heightA, int textureCount,
	int height
)
{
	auto context = GraphicsCore::instance()->context;

	hPhotosIntegralsXho->SetUnorderedAccessView(photosIntegralsXh);
	hPhotosIntegralsYho->SetUnorderedAccessView(photosIntegralsYh);
	hPhotosIntegralsZho->SetUnorderedAccessView(photosIntegralsZh);

	hWidthA->SetRawValue(&widthA, 0, sizeof(widthA));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&textureCount, 0, sizeof(textureCount));

	hClearGridIntegrals->GetPassByIndex(0)->Apply(0, context);

	uint32_t groupsX = std::ceil((float)(widthA) / 16);
	uint32_t groupsY = std::ceil((float)(height) / 16);
	uint32_t groupsZ = std::ceil((float)(textureCount) / 4);

	context->Dispatch(groupsX, groupsY, groupsZ);
	unboundViews();

	hPhotosIntegralsX->SetUnorderedAccessView(photosIntegralsX);
	hPhotosIntegralsY->SetUnorderedAccessView(photosIntegralsY);
	hPhotosIntegralsZ->SetUnorderedAccessView(photosIntegralsZ);

	hWidthA->SetRawValue(&widthA, 0, sizeof(widthA));
	hHeightA->SetRawValue(&heightA, 0, sizeof(heightA));
	hTexturesCount->SetRawValue(&textureCount, 0, sizeof(textureCount));

	hClearGridIntegrals->GetPassByIndex(1)->Apply(0, context);

	groupsX = std::ceil((float)(widthA) / 16);
	groupsY = std::ceil((float)(heightA) / 16);
	groupsZ = std::ceil((float)(textureCount) / 4);

	context->Dispatch(groupsX, groupsY, groupsZ);
	unboundViews();
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
	int width, int height, int textureCount,
	int widthA, int heightA
)
{
	auto context = GraphicsCore::instance()->context;

	hPhotos->SetResource(photos);

	hPhotosIntegralsXho->SetUnorderedAccessView(photosIntegralsXho);
	hPhotosIntegralsYho->SetUnorderedAccessView(photosIntegralsYho);
	hPhotosIntegralsZho->SetUnorderedAccessView(photosIntegralsZho);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hWidthA->SetRawValue(&widthA, 0, sizeof(widthA));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hTexturesCount->SetRawValue(&textureCount, 0, sizeof(textureCount));

	int cellDimensionX = CELL_DIMENSION_X;
	hCellDimensionX->SetRawValue(&cellDimensionX, 0, sizeof(cellDimensionX));

	hCalculateGridIntegrals->GetPassByIndex(0)->Apply(0, context);

	uint32_t groupsX = std::ceil((float)(width) / 16);
	uint32_t groupsY = std::ceil((float)(height) / 16);
	uint32_t groupsZ = std::ceil((float)(textureCount) / 4);

	context->Dispatch(groupsX, groupsY, groupsZ);
	unboundViews();

	hPhotosIntegralsXhi->SetResource(photosIntegralsXhi);
	hPhotosIntegralsYhi->SetResource(photosIntegralsYhi);
	hPhotosIntegralsZhi->SetResource(photosIntegralsZhi);

	hPhotosIntegralsX->SetUnorderedAccessView(photosIntegralsX);
	hPhotosIntegralsY->SetUnorderedAccessView(photosIntegralsY);
	hPhotosIntegralsZ->SetUnorderedAccessView(photosIntegralsZ);

	hWidthA->SetRawValue(&widthA, 0, sizeof(widthA));
	hHeight->SetRawValue(&height, 0, sizeof(height));
	hHeightA->SetRawValue(&heightA, 0, sizeof(heightA));
	hTexturesCount->SetRawValue(&textureCount, 0, sizeof(textureCount));

	int cellDimensionY = CELL_DIMENSION_Y;
	hCellDimensionY->SetRawValue(&cellDimensionY, 0, sizeof(cellDimensionY));

	hCalculateGridIntegrals->GetPassByIndex(1)->Apply(0, context);

	groupsX = std::ceil((float)(widthA) / 16);
	groupsY = std::ceil((float)(height) / 16);
	groupsZ = std::ceil((float)(textureCount) / 4);

	context->Dispatch(groupsX, groupsY, groupsZ);
	unboundViews();
}

void GridIntegralsB::unboundViews()
{
	auto context = GraphicsCore::instance()->context;

	ID3D11UnorderedAccessView* uavsNull[] = { nullptr,nullptr,nullptr };
	ID3D11ShaderResourceView* srvsNull = nullptr;
	context->CSSetShaderResources(0, 1, &srvsNull);
	context->CSSetUnorderedAccessViews(0, 3, uavsNull, nullptr);
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

	hWidthB = hGridIntegralsFX->GetVariableByName("widthB");
	hHeightB = hGridIntegralsFX->GetVariableByName("heightB");

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
	int widthB, int heightB, int count
)
{
	auto context = GraphicsCore::instance()->context;

	// Set shader variables
	hWidthB->SetRawValue(&widthB, 0, sizeof(widthB));
	hHeightB->SetRawValue(&heightB, 0, sizeof(heightB));
	hTexturesCount->SetRawValue(&count, 0, sizeof(count));

	hPhotosIntegralsX->SetUnorderedAccessView(photosIntegralsX);
	hPhotosIntegralsY->SetUnorderedAccessView(photosIntegralsY);
	hPhotosIntegralsZ->SetUnorderedAccessView(photosIntegralsZ);

	hClearGridIntegralsTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(widthB) / 16);
	uint32_t groups_y = std::ceil((float)(heightB) / 16);
	uint32_t groups_z = std::ceil((float)(count) / 4);

	context->Dispatch(groups_x, groups_y, groups_z);
	unboundViews();
}

void GridIntegralsB::calculatePhotosIntegrals(
	ID3D11ShaderResourceView* photos,
	ID3D11UnorderedAccessView* photosIntegralsX,
	ID3D11UnorderedAccessView* photosIntegralsY,
	ID3D11UnorderedAccessView* photosIntegralsZ,
	int width, int height, int count,
	int widthB, int heightB,
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

	hWidthB->SetRawValue(&widthB, 0, sizeof(widthB));
	hHeightB->SetRawValue(&heightB, 0, sizeof(heightB));

	hAngle0->SetRawValue(&angle0, 0, sizeof(angle0));
	hScale0->SetRawValue(&scale0, 0, sizeof(scale0));

	hAngle1->SetRawValue(&angle1, 0, sizeof(angle1));
	hScale1->SetRawValue(&scale1, 0, sizeof(scale1));

	int cellDimensionX = CELL_DIMENSION_X;
	hCellDimensionX->SetRawValue(&cellDimensionX, 0, sizeof(cellDimensionX));

	int cellDimensionY = CELL_DIMENSION_Y;
	hCellDimensionY->SetRawValue(&cellDimensionY, 0, sizeof(cellDimensionY));

	hCalculateGridIntegralsTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(width) / 16);
	uint32_t groups_y = std::ceil((float)(height) / 16);
	uint32_t groups_z = std::ceil((float)(count) / 4);
	context->Dispatch(groups_x, groups_y, groups_z);
	unboundViews();
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

	hAAho = hOperationsOnGridIntegralsFX->GetVariableByName("AAho")->AsUnorderedAccessView();
	hAAfractionHo = hOperationsOnGridIntegralsFX->GetVariableByName("AAfractionHo")->AsUnorderedAccessView();
	hMaxAho = hOperationsOnGridIntegralsFX->GetVariableByName("maxAho")->AsUnorderedAccessView();

	hAAhi = hOperationsOnGridIntegralsFX->GetVariableByName("AAhi")->AsShaderResource();
	hAAfractionHi = hOperationsOnGridIntegralsFX->GetVariableByName("AAfractionHi")->AsShaderResource();
	hMaxAhi = hOperationsOnGridIntegralsFX->GetVariableByName("maxAhi")->AsShaderResource();

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

	ID3D11ShaderResourceView* nullSRVs[9] = {
		nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr
	};
	context->CSSetShaderResources(0, 9, nullSRVs);
}

void OperationsOnGridIntegrals::clearAAandMaxA(
	ID3D11UnorderedAccessView* AA,
	ID3D11UnorderedAccessView* AAfraction,
	ID3D11UnorderedAccessView* maxA,
	ID3D11UnorderedAccessView* AAh,
	ID3D11UnorderedAccessView* AAfractionH,
	ID3D11UnorderedAccessView* maxAh,
	int widthAA, int heightAA, int texturesCount,
	int heightAreal
)
{
	hAAho->SetUnorderedAccessView(AAh);
	hAAfractionHo->SetUnorderedAccessView(AAfractionH);
	hMaxAho->SetUnorderedAccessView(maxAh);

	hWidthAA->SetRawValue(&widthAA, 0, sizeof(widthAA));
	hHeightAreal->SetRawValue(&heightAreal, 0, sizeof(heightAreal));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	auto context = GraphicsCore::instance()->context;
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthAA / 16);
	uint32_t groups_y = std::ceil((float)heightAreal / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);
	
	hAAho->SetUnorderedAccessView(nullptr);
	hAAfractionHo->SetUnorderedAccessView(nullptr);
	hMaxAho->SetUnorderedAccessView(nullptr);
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(0)->Apply(0, context);
	unboundViews();

	hAA->SetUnorderedAccessView(AA);
	hAAfraction->SetUnorderedAccessView(AAfraction);
	hMaxA->SetUnorderedAccessView(maxA);

	hWidthAA->SetRawValue(&widthAA, 0, sizeof(widthAA));
	hHeightAA->SetRawValue(&heightAA, 0, sizeof(heightAA));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(1)->Apply(0, context);

	groups_x = std::ceil((float)widthAA / 16);
	groups_y = std::ceil((float)heightAA / 16);
	groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);

	hAA->SetUnorderedAccessView(nullptr);
	hAAfraction->SetUnorderedAccessView(nullptr);
	hMaxA->SetUnorderedAccessView(nullptr);
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(1)->Apply(0, context);
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
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(2)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthBB / 16);
	uint32_t groups_y = std::ceil((float)heightBB / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);

	hBB->SetUnorderedAccessView(nullptr);
	hBBfraction->SetUnorderedAccessView(nullptr);
	hMaxB->SetUnorderedAccessView(nullptr);
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(2)->Apply(0, context);
	unboundViews();
}

void OperationsOnGridIntegrals::clearAB(
	ID3D11UnorderedAccessView* AB,
	ID3D11UnorderedAccessView* ABfraction,
	int widthAB, int heightAB, int texturesCount,
	int widthABreal, int heightABreal
)
{
	hAB->SetUnorderedAccessView(AB);
	hABfraction->SetUnorderedAccessView(ABfraction);

	hWidthAB->SetRawValue(&widthAB, 0, sizeof(widthAB));
	hHeightAB->SetRawValue(&heightAB, 0, sizeof(heightAB));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	hWidthABreal->SetRawValue(&widthABreal, 0, sizeof(widthABreal));
	hHeightABreal->SetRawValue(&heightABreal, 0, sizeof(heightABreal));

	auto context = GraphicsCore::instance()->context;
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(3)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthAB / 16);
	uint32_t groups_y = std::ceil((float)heightAB / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);

	hAB->SetUnorderedAccessView(nullptr);
	hABfraction->SetUnorderedAccessView(nullptr);
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(3)->Apply(0, context);
	unboundViews();
}

void OperationsOnGridIntegrals::calculateAAandMaxA(
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
)
{
	hPhotosIntegralsAx->SetResource(photosIntegralsAx);
	hPhotosIntegralsAy->SetResource(photosIntegralsAy);
	hPhotosIntegralsAz->SetResource(photosIntegralsAz);

	hAAho->SetUnorderedAccessView(AAho);
	hAAfractionHo->SetUnorderedAccessView(AAfractionHo);
	hMaxAho->SetUnorderedAccessView(maxAho);

	hWidthAreal->SetRawValue(&widthAreal, 0, sizeof(widthAreal));
	hHeightAreal->SetRawValue(&heightAreal, 0, sizeof(heightAreal));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	int radius = RADIUS_IN_CELLS;
	hRadius->SetRawValue(&radius, 0, sizeof(radius));

	Vec2d<int> cellDimension(CELL_DIMENSION_X, CELL_DIMENSION_Y);
	hCellDimension->SetRawValue(&cellDimension, 0, sizeof(cellDimension));

	hWidthAA->SetRawValue(&widthAA, 0, sizeof(widthAA));

	auto context = GraphicsCore::instance()->context;
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(4)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthAreal / 16);
	uint32_t groups_y = std::ceil((float)heightAreal / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);

	hAAho->SetUnorderedAccessView(nullptr);
	hAAfractionHo->SetUnorderedAccessView(nullptr);
	hMaxAho->SetUnorderedAccessView(nullptr);
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(4)->Apply(0, context);
	unboundViews();

	hAAhi->SetResource(AAhi);
	hAAfractionHi->SetResource(AAfractionHi);
	hMaxAhi->SetResource(maxAhi);

	hAA->SetUnorderedAccessView(AA);
	hAAfraction->SetUnorderedAccessView(AAfraction);
	hMaxA->SetUnorderedAccessView(maxA);

	hWidthAA->SetRawValue(&widthAA, 0, sizeof(widthAA));
	hHeightAreal->SetRawValue(&heightAreal, 0, sizeof(heightAreal));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	hRadius->SetRawValue(&radius, 0, sizeof(radius));
	hCellDimension->SetRawValue(&cellDimension, 0, sizeof(cellDimension));
	hHeightAA->SetRawValue(&heightAA, 0, sizeof(heightAA));

	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(5)->Apply(0, context);

	groups_x = std::ceil((float)widthAA / 16);
	groups_y = std::ceil((float)heightAreal / 16);
	groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);

	hAA->SetUnorderedAccessView(nullptr);
	hAAfraction->SetUnorderedAccessView(nullptr);
	hMaxA->SetUnorderedAccessView(nullptr);
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(5)->Apply(0, context);
	unboundViews();
}

void OperationsOnGridIntegrals::calculateBBandMaxB(
	ID3D11ShaderResourceView* photosIntegralsBx,
	ID3D11ShaderResourceView* photosIntegralsBy,
	ID3D11ShaderResourceView* photosIntegralsBz,
	ID3D11UnorderedAccessView* BB,
	ID3D11UnorderedAccessView* BBfraction,
	ID3D11UnorderedAccessView* maxB,
	int widthB, int heightB, int texturesCount
)
{
	hPhotosIntegralsBx->SetResource(photosIntegralsBx);
	hPhotosIntegralsBy->SetResource(photosIntegralsBy);
	hPhotosIntegralsBz->SetResource(photosIntegralsBz);

	hBB->SetUnorderedAccessView(BB);
	hBBfraction->SetUnorderedAccessView(BBfraction);
	hMaxB->SetUnorderedAccessView(maxB);

	hWidthB->SetRawValue(&widthB, 0, sizeof(widthB));
	hHeightB->SetRawValue(&heightB, 0, sizeof(heightB));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	int radius = RADIUS_IN_CELLS;
	hRadius->SetRawValue(&radius, 0, sizeof(radius));

	auto context = GraphicsCore::instance()->context;
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(6)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthB / 16);
	uint32_t groups_y = std::ceil((float)heightB / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);

	hBB->SetUnorderedAccessView(nullptr);
	hBBfraction->SetUnorderedAccessView(nullptr);
	hMaxB->SetUnorderedAccessView(nullptr);
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(6)->Apply(0, context);
	unboundViews();
}

void OperationsOnGridIntegrals::calculateAB(
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
)
{
	hPhotosIntegralsAx->SetResource(photosIntegralsAx);
	hPhotosIntegralsAy->SetResource(photosIntegralsAy);
	hPhotosIntegralsAz->SetResource(photosIntegralsAz);

	hPhotosIntegralsBx->SetResource(photosIntegralsBx);
	hPhotosIntegralsBy->SetResource(photosIntegralsBy);
	hPhotosIntegralsBz->SetResource(photosIntegralsBz);

	hAB->SetUnorderedAccessView(AB);
	hABfraction->SetUnorderedAccessView(ABfraction);

	hWidthAreal->SetRawValue(&widthAreal, 0, sizeof(widthAreal));
	hHeightAreal->SetRawValue(&heightAreal, 0, sizeof(heightAreal));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	hWidthAB->SetRawValue(&widthAB, 0, sizeof(widthAB));
	hHeightAB->SetRawValue(&heightAB, 0, sizeof(heightAB));

	hWidthABreal->SetRawValue(&widthABreal, 0, sizeof(widthABreal));
	hHeightABreal->SetRawValue(&heightABreal, 0, sizeof(heightABreal));
	hIndexOfA->SetRawValue(&indexOfA, 0, sizeof(indexOfA));

	int radius = RADIUS_IN_CELLS;
	hRadius->SetRawValue(&radius, 0, sizeof(radius));

	Vec2d<int> cellDimension(CELL_DIMENSION_X, CELL_DIMENSION_Y);
	hCellDimension->SetRawValue(&cellDimension, 0, sizeof(cellDimension));

	Vec2d<int> offsetRange(OFFSET_RANGE_X, OFFSET_RANGE_Y);
	hOffsetRange->SetRawValue(&offsetRange, 0, sizeof(offsetRange));

	Vec2d<int> offset0(OFFSET0_X, OFFSET0_Y);
	hOffset0->SetRawValue(&offset0, 0, sizeof(offset0));

	auto context = GraphicsCore::instance()->context;
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(7)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthA / 16);
	uint32_t groups_y = std::ceil((float)heightA / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);

	hAB->SetUnorderedAccessView(nullptr);
	hABfraction->SetUnorderedAccessView(nullptr);
	hMakeOperationsOnGridIntegralsTech->GetPassByIndex(7)->Apply(0, context);
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

	hWidth = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("width");
	hWidthAA = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("widthAA");
	hWidthAB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("widthAB");
	hWidthABreal = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("widthABreal");
	hWidthBB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("widthBB");

	hHeight = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("height");
	hHeightAA = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("heightAA");
	hHeightAB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("heightAB");
	hHeightABreal = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("heightABreal");
	hHeightBB = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("heightBB");

	hTexturesCount = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("texturesCount");

	hAngle0 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("angle0");
	hScale0 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("scale0");

	hAngle1 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("angle1");
	hScale1 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("scale1");

	hRadius = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("radius");
	hCellDimension = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("cellDimension");
	hOffsetRange = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("offsetRange");
	hOffset0 = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("offset0");

	hIndexOfA = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("indexOfA");

	hError = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("error")->AsUnorderedAccessView();
	hErrorIn = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("errorIn")->AsShaderResource();
	hAtoBx = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("AtoBx")->AsUnorderedAccessView();
	hAtoBy = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("AtoBy")->AsUnorderedAccessView();
	hAtoBz = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("AtoBz")->AsUnorderedAccessView();
	hAtoBw = hLeastSquaresOfJacobianDeterminantFX->GetVariableByName("AtoBw")->AsUnorderedAccessView();
}

void LeastSquaresOfJacobianDeterminant::unboundViews()
{
	auto context = GraphicsCore::instance()->context;

	ID3D11ShaderResourceView* nullSRVs[9] = {
		nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr
	};
	context->CSSetShaderResources(0, 9, nullSRVs);
}

void LeastSquaresOfJacobianDeterminant::clear(
	ID3D11UnorderedAccessView* error,
	ID3D11UnorderedAccessView* AtoBx,
	ID3D11UnorderedAccessView* AtoBy,
	ID3D11UnorderedAccessView* AtoBz,
	ID3D11UnorderedAccessView* AtoBw,
	int widthBB, int heightBB, int texturesCount
)
{
	hError->SetUnorderedAccessView(error);
	hAtoBx->SetUnorderedAccessView(AtoBx);
	hAtoBy->SetUnorderedAccessView(AtoBy);
	hAtoBz->SetUnorderedAccessView(AtoBz);
	hAtoBw->SetUnorderedAccessView(AtoBw);

	hWidthBB->SetRawValue(&widthBB, 0, sizeof(widthBB));
	hHeightBB->SetRawValue(&heightBB, 0, sizeof(heightBB));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	auto context = GraphicsCore::instance()->context;
	hClearTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthBB / 16);
	uint32_t groups_y = std::ceil((float)heightBB / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);

	hError->SetUnorderedAccessView(nullptr);
	hAtoBx->SetUnorderedAccessView(nullptr);
	hAtoBy->SetUnorderedAccessView(nullptr);
	hAtoBz->SetUnorderedAccessView(nullptr);
	hAtoBw->SetUnorderedAccessView(nullptr);
	hClearTech->GetPassByIndex(0)->Apply(0, context);
	unboundViews();
}

void LeastSquaresOfJacobianDeterminant::calculateMapping(
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
)
{
	hAA->SetResource(AA);
	hAAfraction->SetResource(AAfraction);

	hAB->SetResource(AB);
	hABfraction->SetResource(ABfraction);

	hBB->SetResource(BB);
	hBBfraction->SetResource(BBfraction);

	hMaxA->SetResource(maxA);
	hMaxB->SetResource(maxB);

	hError->SetUnorderedAccessView(error);

	hWidth->SetRawValue(&width, 0, sizeof(width));
	hHeight->SetRawValue(&height, 0, sizeof(height));

	hAngle0->SetRawValue(&angle0, 0, sizeof(angle0));
	hScale0->SetRawValue(&scale0, 0, sizeof(scale0));

	hAngle1->SetRawValue(&angle1, 0, sizeof(angle1));
	hScale1->SetRawValue(&scale1, 0, sizeof(scale1));

	hWidthAB->SetRawValue(&widthAB, 0, sizeof(widthAB));
	hHeightAB->SetRawValue(&heightAB, 0, sizeof(heightAB));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	hIndexOfA->SetRawValue(&indexOfA, 0, sizeof(indexOfA));

	hWidthABreal->SetRawValue(&widthABreal, 0, sizeof(widthABreal));
	hHeightABreal->SetRawValue(&heightABreal, 0, sizeof(heightABreal));

	hWidthAA->SetRawValue(&widthAA, 0, sizeof(widthAA));
	hHeightAA->SetRawValue(&heightAA, 0, sizeof(heightAA));

	int radius = RADIUS_IN_CELLS;
	hRadius->SetRawValue(&radius, 0, sizeof(radius));

	Vec2d<int> cellDimension(CELL_DIMENSION_X, CELL_DIMENSION_Y);
	hCellDimension->SetRawValue(&cellDimension, 0, sizeof(cellDimension));

	Vec2d<int> offsetRange(OFFSET_RANGE_X, OFFSET_RANGE_Y);
	hOffsetRange->SetRawValue(&offsetRange, 0, sizeof(offsetRange));

	Vec2d<int> offset0(OFFSET0_X, OFFSET0_Y);
	hOffset0->SetRawValue(&offset0, 0, sizeof(offset0));

	auto context = GraphicsCore::instance()->context;
	hApplyLeastSquareMethodTech->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)widthAB / 16);
	uint32_t groups_y = std::ceil((float)heightAB / 16);
	uint32_t groups_z = std::ceil((float)texturesCount / 4);

	context->Dispatch(groups_x, groups_y, groups_z);
	
	hError->SetUnorderedAccessView(nullptr);
	hApplyLeastSquareMethodTech->GetPassByIndex(0)->Apply(0, context);
	unboundViews();

	hAA->SetResource(AA);
	hAAfraction->SetResource(AAfraction);

	hAB->SetResource(AB);
	hABfraction->SetResource(ABfraction);

	hBB->SetResource(BB);
	hBBfraction->SetResource(BBfraction);

	hMaxA->SetResource(maxA);
	hMaxB->SetResource(maxB);

	hErrorIn->SetResource(errorIn);

	hAtoBx->SetUnorderedAccessView(AtoBx);
	hAtoBy->SetUnorderedAccessView(AtoBy);
	hAtoBz->SetUnorderedAccessView(AtoBz);
	hAtoBw->SetUnorderedAccessView(AtoBw);

	hWidthAB->SetRawValue(&widthAB, 0, sizeof(widthAB));
	hHeightAB->SetRawValue(&heightAB, 0, sizeof(heightAB));
	hTexturesCount->SetRawValue(&texturesCount, 0, sizeof(texturesCount));

	hIndexOfA->SetRawValue(&indexOfA, 0, sizeof(indexOfA));

	hWidthABreal->SetRawValue(&widthABreal, 0, sizeof(widthABreal));
	hHeightABreal->SetRawValue(&heightABreal, 0, sizeof(heightABreal));

	hWidthAA->SetRawValue(&widthAA, 0, sizeof(widthAA));
	hHeightAA->SetRawValue(&heightAA, 0, sizeof(heightAA));

	hRadius->SetRawValue(&radius, 0, sizeof(radius));
	hCellDimension->SetRawValue(&cellDimension, 0, sizeof(cellDimension));
	hOffsetRange->SetRawValue(&offsetRange, 0, sizeof(offsetRange));
	hOffset0->SetRawValue(&offset0, 0, sizeof(offset0));

	hAngle0->SetRawValue(&angle0, 0, sizeof(angle0));
	hScale0->SetRawValue(&scale0, 0, sizeof(scale0));

	hAngle1->SetRawValue(&angle1, 0, sizeof(angle1));
	hScale1->SetRawValue(&scale1, 0, sizeof(scale1));

	hApplyLeastSquareMethodTech->GetPassByIndex(1)->Apply(0, context);
	context->Dispatch(groups_x, groups_y, groups_z);

	hAtoBx->SetUnorderedAccessView(nullptr);
	hAtoBy->SetUnorderedAccessView(nullptr);
	hAtoBz->SetUnorderedAccessView(nullptr);
	hAtoBw->SetUnorderedAccessView(nullptr);
	hApplyLeastSquareMethodTech->GetPassByIndex(1)->Apply(0, context);
	unboundViews();
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
	hAtoBx = hEffect->GetVariableByName("AtoBx")->AsShaderResource();
	hAtoBy = hEffect->GetVariableByName("AtoBy")->AsShaderResource();
	hAtoBz = hEffect->GetVariableByName("AtoBz")->AsShaderResource();
	hAtoBw = hEffect->GetVariableByName("AtoBw")->AsShaderResource();
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
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = sizeof(uint32_t);
	device->CreateBuffer(&bufferDesc, nullptr, &minDistance);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = 1;
	uavDesc.Buffer.Flags = 0;
	device->CreateUnorderedAccessView(minDistance, &uavDesc, &minDistanceUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 1;
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

Vec4d<int> FindNearestDefinedPoint::findNearestPoint(
	ID3D11ShaderResourceView* error,
	ID3D11ShaderResourceView* AtoBx,
	ID3D11ShaderResourceView* AtoBy,
	ID3D11ShaderResourceView* AtoBz,
	ID3D11ShaderResourceView* AtoBw,
	const Vec2d<int>& point
)
{
	auto context = GraphicsCore::instance()->context;

	hError->SetResource(error);
	hAtoBx->SetResource(AtoBx);
	hAtoBy->SetResource(AtoBy);
	hAtoBz->SetResource(AtoBz);
	hAtoBw->SetResource(AtoBw);

	uint32_t initialMinDistance = 0xffffffff;
	context->UpdateSubresource(minDistance, 0, nullptr, &initialMinDistance, 0, 0);

	hMinDistanceOut->SetUnorderedAccessView(minDistanceUAV);

	ID3D11Texture2D* errorTex = nullptr;
	error->GetResource((ID3D11Resource * *)(&errorTex));

	D3D11_TEXTURE2D_DESC texDesc;
	errorTex->GetDesc(&texDesc);

	hWidth->SetRawValue(&texDesc.Width, 0, sizeof(texDesc.Width));
	hHeight->SetRawValue(&texDesc.Height, 0, sizeof(texDesc.Height));
	
	int index = 1;
	hIndex->SetRawValue(&index, 0, sizeof(index));

	hPt->SetRawValue(&point, 0, sizeof(point));

	uint32_t threshold = 4000;
	hThreshold->SetRawValue(&threshold, 0, sizeof(threshold));

	hTechnique->GetPassByIndex(0)->Apply(0, context);

	uint32_t groups_x = std::ceil((float)(texDesc.Width) / 32);
	uint32_t groups_y = std::ceil((float)(texDesc.Height) / 32);
	uint32_t groups_z = 1;

	context->Dispatch(groups_x, groups_y, groups_z);

	ID3D11ShaderResourceView* nullSRVs[6] = {
		nullptr,nullptr,nullptr,
		nullptr,nullptr,nullptr
	};
	ID3D11UnorderedAccessView* nullUAVs[2] = { nullptr,nullptr };

	context->CSSetShaderResources(0, 6, nullSRVs);
	context->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);

	hError->SetResource(error);
	hAtoBx->SetResource(AtoBx);
	hAtoBy->SetResource(AtoBy);
	hAtoBz->SetResource(AtoBz);
	hAtoBw->SetResource(AtoBw);
	hMinDistanceIn->SetResource(minDistanceSRV);

	Vec4d<uint32_t> mappingOfPointInit;
	context->UpdateSubresource(mappingOfPoint, 0, nullptr, &mappingOfPointInit, 0, 0);
	hMappingOfPoint->SetUnorderedAccessView(mappingOfPointUAV);

	hWidth->SetRawValue(&texDesc.Width, 0, sizeof(texDesc.Width));
	hHeight->SetRawValue(&texDesc.Height, 0, sizeof(texDesc.Height));
	hIndex->SetRawValue(&index, 0, sizeof(index));
	hPt->SetRawValue(&point, 0, sizeof(point));
	hThreshold->SetRawValue(&threshold, 0, sizeof(threshold));

	hTechnique->GetPassByIndex(1)->Apply(0, context);
	context->Dispatch(groups_x, groups_y, groups_z);

	context->CSSetShaderResources(0, 6, nullSRVs);
	context->CSSetUnorderedAccessViews(0, 2, nullUAVs, nullptr);

	context->CopyResource(mappingOfPointCopy, mappingOfPoint);

	D3D11_MAPPED_SUBRESOURCE data;
	context->Map(mappingOfPointCopy, 0, D3D11_MAP_READ, 0, &data);

	Vec4d<uint32_t> mapping = *((Vec4d<uint32_t>*)data.pData);
	Vec4d<int> res{ mapping.x() >> 16, mapping.x() & 0xffff,
		mapping.z() >> 16, mapping.z() & 0xffff };

	context->Unmap(mappingOfPointCopy, 0);

	char buffer[256];
	std::sprintf(buffer, "\nerror = %llu\n", mapping.y());
	OutputDebugStringA(buffer);
	std::sprintf(buffer, "\nerror = %llu\n", mapping.w());
	OutputDebugStringA(buffer);

	return res;
}

void TransformTo3dVertices::init()
{
	char shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariableA("SPECIAL_PURPOSE_SHADERS", shadersFolder, sz);

	std::string sFindNearestDefinedPoint = std::string(shadersFolder) + "\\TransformationFromTheSamePointsTo3dVertices.hlsl";

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

	hTechnique = hEffect->GetTechniqueByName("TransformTo3dVertices");

	Rc_out = hEffect->GetVariableByName("Rc_out")->AsUnorderedAccessView();
	Ac_out = hEffect->GetVariableByName("Ac_out")->AsUnorderedAccessView();
	Bc_out = hEffect->GetVariableByName("Bc_out")->AsUnorderedAccessView();

	Ad_out = hEffect->GetVariableByName("Ad_out")->AsUnorderedAccessView();
	Bd_out = hEffect->GetVariableByName("Bd_out")->AsUnorderedAccessView();
	Cd_out = hEffect->GetVariableByName("Cd_out")->AsUnorderedAccessView();

	R_out = hEffect->GetVariableByName("R_out")->AsUnorderedAccessView();
	A_out = hEffect->GetVariableByName("A_out")->AsUnorderedAccessView();
	B_out = hEffect->GetVariableByName("B_out")->AsUnorderedAccessView();

	Rc = hEffect->GetVariableByName("Rc")->AsShaderResource();
	Ac = hEffect->GetVariableByName("Ac")->AsShaderResource();
	Bc = hEffect->GetVariableByName("Bc")->AsShaderResource();

	Ad = hEffect->GetVariableByName("Ad")->AsShaderResource();
	Bd = hEffect->GetVariableByName("Bd")->AsShaderResource();
	Cd = hEffect->GetVariableByName("Cd")->AsShaderResource();

	R = hEffect->GetVariableByName("R")->AsShaderResource();
	A = hEffect->GetVariableByName("A")->AsShaderResource();
	B = hEffect->GetVariableByName("B")->AsShaderResource();

	pointsOnPhotos = hEffect->GetVariableByName("pointsOnPhotos")->AsShaderResource();
	mapToVertexAndCamera = hEffect->GetVariableByName("mapToVertexAndCamera")->AsShaderResource();

	I = hEffect->GetVariableByName("I")->AsUnorderedAccessView();
	Iin = hEffect->GetVariableByName("Iin")->AsShaderResource();

	J = hEffect->GetVariableByName("J")->AsUnorderedAccessView();
	Jin = hEffect->GetVariableByName("Jin")->AsShaderResource();

	K = hEffect->GetVariableByName("K")->AsUnorderedAccessView();
	Kin = hEffect->GetVariableByName("Kin")->AsShaderResource();

	xyzc = hEffect->GetVariableByName("xyzc")->AsUnorderedAccessView();
	xyzc_in = hEffect->GetVariableByName("xyzc_in")->AsShaderResource();

	xyz = hEffect->GetVariableByName("xyz")->AsUnorderedAccessView();
	xyz_in = hEffect->GetVariableByName("xyz_in")->AsShaderResource();

	error = hEffect->GetVariableByName("error")->AsUnorderedAccessView();

	dIdA = hEffect->GetVariableByName("dIdA")->AsUnorderedAccessView();
	dIdAin = hEffect->GetVariableByName("dIdAin")->AsShaderResource();

	dIdB = hEffect->GetVariableByName("dIdB")->AsUnorderedAccessView();
	dIdBin = hEffect->GetVariableByName("dIdBin")->AsShaderResource();

	dIdC = hEffect->GetVariableByName("dIdC")->AsUnorderedAccessView();
	dIdCin = hEffect->GetVariableByName("dIdCin")->AsShaderResource();

	dJdA = hEffect->GetVariableByName("dJdA")->AsUnorderedAccessView();
	dJdAin = hEffect->GetVariableByName("dJdAin")->AsShaderResource();

	dJdB = hEffect->GetVariableByName("dJdB")->AsUnorderedAccessView();
	dJdBin = hEffect->GetVariableByName("dJdBin")->AsShaderResource();

	dJdC = hEffect->GetVariableByName("dJdC")->AsUnorderedAccessView();
	dJdCin = hEffect->GetVariableByName("dJdCin")->AsShaderResource();

	dKdA = hEffect->GetVariableByName("dKdA")->AsUnorderedAccessView();
	dKdAin = hEffect->GetVariableByName("dKdAin")->AsShaderResource();

	dKdB = hEffect->GetVariableByName("dKdB")->AsUnorderedAccessView();
	dKdBin = hEffect->GetVariableByName("dKdBin")->AsShaderResource();

	dKdC = hEffect->GetVariableByName("dKdC")->AsUnorderedAccessView();
	dKdCin = hEffect->GetVariableByName("dKdCin")->AsShaderResource();

	dXYZCdR = hEffect->GetVariableByName("dXYZCdR")->AsUnorderedAccessView();
	dXYZCdRin = hEffect->GetVariableByName("dXYZCdRin")->AsShaderResource();

	dXYZCdA = hEffect->GetVariableByName("dXYZCdA")->AsUnorderedAccessView();
	dXYZCdAin = hEffect->GetVariableByName("dXYZCdAin")->AsShaderResource();

	dXYZCdB = hEffect->GetVariableByName("dXYZCdB")->AsUnorderedAccessView();
	dXYZCdBin = hEffect->GetVariableByName("dXYZCdBin")->AsShaderResource();

	dXYZdR = hEffect->GetVariableByName("dXYZdR")->AsUnorderedAccessView();
	dXYZdRin = hEffect->GetVariableByName("dXYZdRin")->AsShaderResource();

	dXYZdA = hEffect->GetVariableByName("dXYZdA")->AsUnorderedAccessView();
	dXYZdAin = hEffect->GetVariableByName("dXYZdAin")->AsShaderResource();

	dXYZdB = hEffect->GetVariableByName("dXYZdB")->AsUnorderedAccessView();
	dXYZdBin = hEffect->GetVariableByName("dXYZdBin")->AsShaderResource();

	gradError_r = hEffect->GetVariableByName("gradError_r")->AsUnorderedAccessView();
	gradError_r_in = hEffect->GetVariableByName("gradError_r_in")->AsShaderResource();

	gradError_a = hEffect->GetVariableByName("gradError_a")->AsUnorderedAccessView();
	gradError_a_in = hEffect->GetVariableByName("gradError_a_in")->AsShaderResource();

	minGradComponent_a = hEffect->GetVariableByName("minGradComponent_a")->AsUnorderedAccessView();

	amountOfCameras = hEffect->GetVariableByName("amountOfCameras");
	amountOfVertices = hEffect->GetVariableByName("amountOfVertices");
	amountOfPointsOnPhotos = hEffect->GetVariableByName("amountOfPointsOnPhotos");

	t_r = hEffect->GetVariableByName("t_r");
	t_a = hEffect->GetVariableByName("t_a");
}

void TransformTo3dVertices::setPointsOnPhotos(
	flt2* points,
	Vec2d<uint32_t>* mapping,
	int count
)
{
	auto device = GraphicsCore::instance()->device;

	amountOfCameras_ = 0;
	amountOfVertices_ = 0;
	for (int i = 0; i < count; i++)
	{
		amountOfVertices_ = std::max<int>(amountOfVertices_, mapping[i].x());
		amountOfCameras_ = std::max<int>(amountOfCameras_, mapping[i].y());
	}

	{
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = amountOfCameras_ * sizeof(float);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc.StructureByteStride = sizeof(float);

		device->CreateBuffer(&buffer_desc, nullptr, &Rc_buf);
		device->CreateBuffer(&buffer_desc, nullptr, &Ac_buf);
		device->CreateBuffer(&buffer_desc, nullptr, &Bc_buf);

		device->CreateBuffer(&buffer_desc, nullptr, &Ad_buf);
		device->CreateBuffer(&buffer_desc, nullptr, &Bd_buf);
		device->CreateBuffer(&buffer_desc, nullptr, &Cd_buf);
	}
	{
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = amountOfVertices_ * sizeof(float);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc.StructureByteStride = sizeof(float);

		device->CreateBuffer(&buffer_desc, nullptr, &R_buf);
		device->CreateBuffer(&buffer_desc, nullptr, &A_buf);
		device->CreateBuffer(&buffer_desc, nullptr, &B_buf);
	}
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

	initSetOfPhotos(paths);
	initDimensionsOfTextures();
	initPhotosIntegralsA();
	initPhotosIntegralsB();
	initAAandMaxA();
	initAB();
	initBBandMaxB();
	initErrorsAndMapping();

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
		widthAreal, heightAreal, texturesCount,
		height);
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
		width, height, texturesCount,
		widthAreal, heightAreal
	);

	operationsOnGridIntegrals.clearAAandMaxA(
		AAuav, AAfractionUAV, maxAuav,
		AAhUAV, AAfractionHuav, maxAhUAV,
		widthAA, heightAA, texturesCount,
		heightAreal
	);
	operationsOnGridIntegrals.calculateAAandMaxA(
		photosIntegralsAxSRV,
		photosIntegralsAySRV,
		photosIntegralsAzSRV,
		AAhSRV, AAfractionHsrv, maxAhSRV,
		AAuav, AAfractionUAV, maxAuav,
		AAhUAV, AAfractionHuav, maxAhUAV,
		widthAreal, heightAreal, texturesCount,
		widthAA, heightAA
	);

	leastSquaresOfJacobianDeterminant.clear(
		errorUAV,
		AtoBxUAV, AtoByUAV, AtoBzUAV, AtoBwUAV,
		widthBB, heightBB, texturesCount
	);

	int N = 4;

	int countOfSteps = (N + 1) * (N + 1) * (N + 1) * (N + 1);
	int elapsedSteps = 0;
	int percent = 0;

	float scaleMin = 0.9f;
	float scaleMax = 1.1f;
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
						width, height, texturesCount
					);

					gridIntegralsB.calculatePhotosIntegrals(
						setOfPhotosSRV,
						photosIntegralsBxUAV,
						photosIntegralsByUAV,
						photosIntegralsBzUAV,
						width, height, texturesCount,
						widthB, heightB,
						angle0, scale0,
						angle1, scale1
					);
					
					operationsOnGridIntegrals.clearAB(
						ABuav,
						ABfractionUAV,
						widthAB, heightAB, texturesCount,
						widthABreal, heightABreal
					);
					operationsOnGridIntegrals.clearBBandMaxB(
						BBuav,
						BBfractionUAV,
						maxBuav,
						widthBB, heightBB, texturesCount
					);

					operationsOnGridIntegrals.calculateAB(
						photosIntegralsAxSRV,
						photosIntegralsAySRV,
						photosIntegralsAzSRV,
						photosIntegralsBxSRV,
						photosIntegralsBySRV,
						photosIntegralsBzSRV,
						ABuav,
						ABfractionUAV,
						widthA, heightA, texturesCount,
						widthAreal, heightAreal,
						widthAB, heightAB,
						widthABreal, heightABreal,
						0
					);

					operationsOnGridIntegrals.calculateBBandMaxB(
						photosIntegralsBxSRV,
						photosIntegralsBySRV,
						photosIntegralsBzSRV,
						BBuav,
						BBfractionUAV,
						maxBuav,
						widthB,
						heightB,
						texturesCount
					);

					leastSquaresOfJacobianDeterminant.calculateMapping(
						AAsrv, AAfractionSRV,
						ABsrv, ABfractionSRV,
						BBsrv, BBfractionSRV,
						maxAsrv, maxBsrv,
						errorSRV, errorUAV,
						AtoBxUAV, AtoByUAV, AtoBzUAV, AtoBwUAV,
						width, height,
						widthAB, heightAB, texturesCount,
						0,
						widthABreal, heightABreal,
						widthAA, heightAA,
						angle0, scale0,
						angle1, scale1
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

Vec4d<int> ModelMaker::findTheSamePoint(const Vec2d<int>& pt)
{
	return findNearestDefinedPoint.findNearestPoint(
		errorSRV,
		AtoBxSRV,
		AtoBySRV,
		AtoBzSRV,
		AtoBwSRV,
		pt
	);
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

		if (freeResult)
		{
			error->Release();
			error = nullptr;

			errorUAV->Release();
			errorUAV = nullptr;

			errorSRV->Release();
			errorSRV = nullptr;

			AtoBx->Release();
			AtoBx = nullptr;

			AtoBy->Release();
			AtoBy = nullptr;

			AtoBz->Release();
			AtoBz = nullptr;

			AtoBw->Release();
			AtoBw = nullptr;

			AtoBxUAV->Release();
			AtoBxUAV = nullptr;

			AtoByUAV->Release();
			AtoByUAV = nullptr;

			AtoBzUAV->Release();
			AtoBzUAV = nullptr;

			AtoBwUAV->Release();
			AtoBwUAV = nullptr;

			AtoBxSRV->Release();
			AtoBxSRV = nullptr;

			AtoBySRV->Release();
			AtoBySRV = nullptr;

			AtoBzSRV->Release();
			AtoBzSRV = nullptr;

			AtoBwSRV->Release();
			AtoBwSRV = nullptr;
		}

		bSetOfPhotosIsLoaded = false;
	}
}

void ModelMaker::initSetOfPhotos(const std::vector<std::string>& paths)
{
	auto device = GraphicsCore::instance()->device;
	auto context = GraphicsCore::instance()->context;

	texturesCount = paths.size();
	std::vector<ID3D11Texture2D*> photos(texturesCount);
	for (int i = 0; i < texturesCount; i++)
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
	texArrayDesc.ArraySize = texturesCount;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;
	device->CreateTexture2D(&texArrayDesc, 0, &setOfPhotos);

	for (int texElement = 0; texElement < texturesCount; ++texElement)
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

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = texElementDesc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = texturesCount;
	device->CreateShaderResourceView(setOfPhotos, &srv_desc, &setOfPhotosSRV);

	width = texElementDesc.Width;
	height = texElementDesc.Height;
}

void ModelMaker::initPhotosIntegralsA()
{
	auto device = GraphicsCore::instance()->device;

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = widthAreal;
	texArrayDesc.Height = height;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = texturesCount;
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

	texArrayDesc.Height = heightAreal;

	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAx);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAy);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsAz);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateUnorderedAccessView(photosIntegralsAxh, &uav_desc, &photosIntegralsAxhUAV);
	device->CreateUnorderedAccessView(photosIntegralsAyh, &uav_desc, &photosIntegralsAyhUAV);
	device->CreateUnorderedAccessView(photosIntegralsAzh, &uav_desc, &photosIntegralsAzhUAV);

	device->CreateUnorderedAccessView(photosIntegralsAx, &uav_desc, &photosIntegralsAxUAV);
	device->CreateUnorderedAccessView(photosIntegralsAy, &uav_desc, &photosIntegralsAyUAV);
	device->CreateUnorderedAccessView(photosIntegralsAz, &uav_desc, &photosIntegralsAzUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateShaderResourceView(photosIntegralsAxh, &srv_desc, &photosIntegralsAxhSRV);
	device->CreateShaderResourceView(photosIntegralsAyh, &srv_desc, &photosIntegralsAyhSRV);
	device->CreateShaderResourceView(photosIntegralsAzh, &srv_desc, &photosIntegralsAzhSRV);

	device->CreateShaderResourceView(photosIntegralsAx, &srv_desc, &photosIntegralsAxSRV);
	device->CreateShaderResourceView(photosIntegralsAy, &srv_desc, &photosIntegralsAySRV);
	device->CreateShaderResourceView(photosIntegralsAz, &srv_desc, &photosIntegralsAzSRV);
}

void ModelMaker::initPhotosIntegralsB()
{
	auto device = GraphicsCore::instance()->device;

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = widthB;
	texArrayDesc.Height = heightB;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = texturesCount;
	texArrayDesc.Format = DXGI_FORMAT_R32_UINT;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsBx);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsBy);
	device->CreateTexture2D(&texArrayDesc, nullptr, &photosIntegralsBz);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateUnorderedAccessView(photosIntegralsBx, &uav_desc, &photosIntegralsBxUAV);
	device->CreateUnorderedAccessView(photosIntegralsBy, &uav_desc, &photosIntegralsByUAV);
	device->CreateUnorderedAccessView(photosIntegralsBz, &uav_desc, &photosIntegralsBzUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateShaderResourceView(photosIntegralsBx, &srv_desc, &photosIntegralsBxSRV);
	device->CreateShaderResourceView(photosIntegralsBy, &srv_desc, &photosIntegralsBySRV);
	device->CreateShaderResourceView(photosIntegralsBz, &srv_desc, &photosIntegralsBzSRV);
}

void ModelMaker::initAAandMaxA()
{
	auto device = GraphicsCore::instance()->device;

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = widthAA;
	texArrayDesc.Height = heightAA;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = texturesCount;
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

	texArrayDesc.Height = heightAreal;
	device->CreateTexture2D(&texArrayDesc, nullptr, &AAh);
	device->CreateTexture2D(&texArrayDesc, nullptr, &AAfractionH);
	device->CreateTexture2D(&texArrayDesc, nullptr, &maxAh);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateUnorderedAccessView(AA, &uav_desc, &AAuav);
	device->CreateUnorderedAccessView(AAfraction, &uav_desc, &AAfractionUAV);
	device->CreateUnorderedAccessView(maxA, &uav_desc, &maxAuav);

	device->CreateUnorderedAccessView(AAh, &uav_desc, &AAhUAV);
	device->CreateUnorderedAccessView(AAfractionH, &uav_desc, &AAfractionHuav);
	device->CreateUnorderedAccessView(maxAh, &uav_desc, &maxAhUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateShaderResourceView(AA, &srv_desc, &AAsrv);
	device->CreateShaderResourceView(AAfraction, &srv_desc, &AAfractionSRV);
	device->CreateShaderResourceView(maxA, &srv_desc, &maxAsrv);

	device->CreateShaderResourceView(AAh, &srv_desc, &AAhSRV);
	device->CreateShaderResourceView(AAfractionH, &srv_desc, &AAfractionHsrv);
	device->CreateShaderResourceView(maxAh, &srv_desc, &maxAhSRV);
}

void ModelMaker::initAB()
{
	auto device = GraphicsCore::instance()->device;

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = widthABreal;
	texArrayDesc.Height = heightABreal;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = countOfABtextures;
	texArrayDesc.Format = DXGI_FORMAT_R32_UINT;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	device->CreateTexture2D(&texArrayDesc, nullptr, &AB);
	device->CreateTexture2D(&texArrayDesc, nullptr, &ABfraction);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = countOfABtextures;

	device->CreateUnorderedAccessView(AB, &uav_desc, &ABuav);
	device->CreateUnorderedAccessView(ABfraction, &uav_desc, &ABfractionUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = countOfABtextures;

	device->CreateShaderResourceView(AB, &srv_desc, &ABsrv);
	device->CreateShaderResourceView(ABfraction, &srv_desc, &ABfractionSRV);
}

void ModelMaker::initBBandMaxB()
{
	auto device = GraphicsCore::instance()->device;

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = widthBB;
	texArrayDesc.Height = heightBB;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = texturesCount;
	texArrayDesc.Format = DXGI_FORMAT_R32_UINT;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	device->CreateTexture2D(&texArrayDesc, nullptr, &BB);
	device->CreateTexture2D(&texArrayDesc, nullptr, &BBfraction);
	device->CreateTexture2D(&texArrayDesc, nullptr, &maxB);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateUnorderedAccessView(BB, &uav_desc, &BBuav);
	device->CreateUnorderedAccessView(BBfraction, &uav_desc, &BBfractionUAV);
	device->CreateUnorderedAccessView(maxB, &uav_desc, &maxBuav);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateShaderResourceView(BB, &srv_desc, &BBsrv);
	device->CreateShaderResourceView(BBfraction, &srv_desc, &BBfractionSRV);
	device->CreateShaderResourceView(maxB, &srv_desc, &maxBsrv);
}

void ModelMaker::initErrorsAndMapping()
{
	auto device = GraphicsCore::instance()->device;

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = widthBB;
	texArrayDesc.Height = heightBB;
	texArrayDesc.MipLevels = 1;
	texArrayDesc.ArraySize = texturesCount;
	texArrayDesc.Format = DXGI_FORMAT_R32_UINT;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	device->CreateTexture2D(&texArrayDesc, nullptr, &error);
	device->CreateTexture2D(&texArrayDesc, nullptr, &AtoBx);
	device->CreateTexture2D(&texArrayDesc, nullptr, &AtoBy);
	device->CreateTexture2D(&texArrayDesc, nullptr, &AtoBz);
	device->CreateTexture2D(&texArrayDesc, nullptr, &AtoBw);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;
	uav_desc.Format = DXGI_FORMAT_R32_UINT;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
	uav_desc.Texture2DArray.MipSlice = 0;
	uav_desc.Texture2DArray.FirstArraySlice = 0;
	uav_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateUnorderedAccessView(error, &uav_desc, &errorUAV);
	device->CreateUnorderedAccessView(AtoBx, &uav_desc, &AtoBxUAV);
	device->CreateUnorderedAccessView(AtoBy, &uav_desc, &AtoByUAV);
	device->CreateUnorderedAccessView(AtoBz, &uav_desc, &AtoBzUAV);
	device->CreateUnorderedAccessView(AtoBw, &uav_desc, &AtoBwUAV);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R32_UINT;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srv_desc.Texture2DArray.MostDetailedMip = 0;
	srv_desc.Texture2DArray.MipLevels = 1;
	srv_desc.Texture2DArray.FirstArraySlice = 0;
	srv_desc.Texture2DArray.ArraySize = texturesCount;

	device->CreateShaderResourceView(error, &srv_desc, &errorSRV);
	device->CreateShaderResourceView(AtoBx, &srv_desc, &AtoBxSRV);
	device->CreateShaderResourceView(AtoBy, &srv_desc, &AtoBySRV);
	device->CreateShaderResourceView(AtoBz, &srv_desc, &AtoBzSRV);
	device->CreateShaderResourceView(AtoBw, &srv_desc, &AtoBwSRV);
}

void ModelMaker::initDimensionsOfTextures()
{
	int radius = RADIUS_IN_CELLS;
	int diameter = 2 * radius + 1;

	// Dimensions of BB
	widthBB = width / (diameter * CELL_DIMENSION_X);
	heightBB = height / (diameter * CELL_DIMENSION_Y);

	// Dimensions of B
	widthB = widthBB * diameter;
	heightB = heightBB * diameter;

	// Dimensions of real A
	widthAreal = width - CELL_DIMENSION_X + 1;
	heightAreal = height - CELL_DIMENSION_Y + 1; 

	// Dimensions of A
	widthA = (widthB - 1) * OFFSET_RANGE_X;
	widthA += std::min<int>(OFFSET_RANGE_X, OFFSET0_X + widthAreal - (widthB - 1) * CELL_DIMENSION_X);
	widthA -= OFFSET0_X;

	heightA = (heightB - 1) * OFFSET_RANGE_Y;
	heightA += std::min<int>(OFFSET_RANGE_Y, OFFSET0_Y + heightAreal - (heightB - 1) * CELL_DIMENSION_Y);
	heightA -= OFFSET0_Y;

	// Dimensions of AA
	widthAA = widthAreal - (CELL_DIMENSION_X * diameter) + 1;
	heightAA = heightAreal - (CELL_DIMENSION_Y * diameter) + 1;

	// Dimensions of AB
	widthAB = (widthBB - 1) * OFFSET_RANGE_X;
	widthAB += std::min<int>(OFFSET_RANGE_X, OFFSET0_X + widthAA - (widthBB - 1) * CELL_DIMENSION_X * diameter);
	widthAB -= OFFSET0_X;

	heightAB = (heightBB - 1) * OFFSET_RANGE_Y;
	heightAB += std::min<int>(OFFSET_RANGE_Y, OFFSET0_Y + heightAA - (heightBB - 1) * CELL_DIMENSION_Y * diameter);
	heightAB -= OFFSET0_Y;

	// count of AB textures
	widthABreal = 2048;
	heightABreal = 2048;

	int pixelsInAB = widthAB * heightAB * texturesCount;
	countOfABtextures = std::ceil((float)pixelsInAB / (widthABreal * heightABreal));
}

int ModelMaker::getWidth()
{
	return width;
}

int ModelMaker::getHeight()
{
	return height;
}