#include "Shader.h"
#include "ShaderReader.h"
#include "HLSLConverter.h"
#include <d3dx11async.h>
#include <fstream>
#include <cstddef>
#include <winbase.h>
#include <tchar.h>

#include <boost/filesystem.hpp>

#include "Visitors.h"
#include "ResourceManager.h"

#include "preprocessor.h"

using namespace boost::filesystem;

void loadVariableLocationsFromFile(const std::string& path, std::map<std::string, std::string>& variableLocations)
{
	std::ifstream config(path);

	config.seekg(0, config.end);
	int length = config.tellg();
	config.seekg(0, config.beg);

	char* szConfig = new char[length + 1];
	config.read(szConfig, length);
	szConfig[length] = 0;

	std::string sConfig(szConfig);
	char name[] = "locations_of_variables";

	size_t beg = 0;
	beg = sConfig.find(name, beg);
	if (beg == std::string::npos)
		return;

	beg += sizeof name / sizeof * name;
	beg = sConfig.find("{", beg);
	beg += 1;

	const char skipSymbols[5] =
	{
		32, // space
		9,  // tab
		10, // line feed
		13, // carriage return
		0
	};
	const char stopSymbols[7] =
	{
		32, // space
		9,  // tab
		10, // line feed
		13, // carriage return
		'=',
		';',
		0
	};

	for (;;)
	{
		beg = sConfig.find_first_not_of(skipSymbols, beg);
		if (sConfig[beg] == '}')
			break;

		size_t end = sConfig.find_first_of(stopSymbols, beg);
		std::string variable(sConfig, beg, end - beg);

		beg = end;
		beg = sConfig.find('=', beg);
		beg += 1;

		beg = sConfig.find_first_not_of(skipSymbols, beg);

		end = sConfig.find_first_of(stopSymbols, beg);
		std::string location(sConfig, beg, end - beg);

		variableLocations[variable] = location;

		beg = end;
		beg = sConfig.find(';', beg);
		beg += 1;
	}
}

ID3DX11Effect* createEffect(ID3D11Device* device, LPCTSTR shader_path)
{
	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3DX11Effect* mShader = nullptr;
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT res = D3DX11CompileFromFile(shader_path, 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	if (res != S_OK)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, MB_OK);
		return nullptr;
	}
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &mShader);

	return mShader;
}

void registerResources(HLSLShader& shader, ID3D11Device* device, ID3DX11Effect* mShader, LPCSTR config_path)
{
	ShadersNamesVisitor shadersNamesVisitor;
	shader.query(&shadersNamesVisitor);

	ElementsOfCbufferVisitor elementsOfCbufferVisitor;
	shader.query(&elementsOfCbufferVisitor);

	ElementOfCbuffer* elementsOfCbuffers = nullptr;
	int countOfCbufferElements = 0;
	elementsOfCbufferVisitor.getElements(elementsOfCbuffers, countOfCbufferElements);

	for (int i = 0; i < countOfCbufferElements; ++i)
		elementsOfCbuffers[i].v = mShader->GetVariableByName(elementsOfCbuffers[i].name.c_str());

	GlobalVariablesVisitor globalVariablesVisitor;
	shader.query(&globalVariablesVisitor);

	std::map<std::string, std::string> variableLocations;
	loadVariableLocationsFromFile(config_path, variableLocations);

	std::vector<ShadersNamesVisitor::ShadersNames> shadersNames = shadersNamesVisitor.getShadersNames();
	for (auto& sn : shadersNames)
	{
		ID3DX11EffectTechnique* technique = mShader->GetTechniqueByName(sn.technique.c_str());
		ResourceManager::instance()->registerTechnique(sn.technique, technique);
		for (int i = 0; i < sn.passes.size(); ++i)
		{
			ID3DX11EffectPass* pass = technique->GetPassByName(sn.passes[i].c_str());
			ResourceManager::instance()->registerPass(sn.technique, sn.passes[i], pass);

			InputLayoutVisitor inputLayoutVisitor;
			inputLayoutVisitor.setShaderName(sn.shaders[i]);

			shader.query(&inputLayoutVisitor);

			D3DX11_PASS_DESC passDesc;
			pass->GetDesc(&passDesc);

			ID3D11InputLayout* inputLayout = inputLayoutVisitor.getInputLayout(device, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize);
			ResourceManager::instance()->registerInputLayout(sn.technique, sn.passes[i], inputLayout);

			std::vector<InputLayoutResource::StreamInfo> streamsInfo;
			inputLayoutVisitor.getStreamsInfo(streamsInfo);
			ResourceManager::instance()->registerStreamsInfo(sn.technique, sn.passes[i], streamsInfo);

			GeometryShaderInfoVisitor geometryShaderInfoVisitor;
			geometryShaderInfoVisitor.technique = sn.technique;
			geometryShaderInfoVisitor.pass = sn.passes[i];

			shader.query(&geometryShaderInfoVisitor);
			if (geometryShaderInfoVisitor.geometryShaderIsPresented)
			{
				ResourceManager::instance()->registerPresenceOfGeometryShader(sn.technique, sn.passes[i]);
				ResourceManager::instance()->registerPrimitiveType(sn.technique, sn.passes[i], geometryShaderInfoVisitor.primType);
			}
		}

		for (int i = 0; i < countOfCbufferElements; ++i)
		{
			if (elementsOfCbuffers[i].type == std::string("float4x4"))
				ResourceManager::instance()->registerFloat4x4(sn.technique, elementsOfCbuffers[i].name, elementsOfCbuffers[i].v->AsMatrix(), elementsOfCbuffers[i].elementsCount);
			if (elementsOfCbuffers[i].type == std::string("float3"))
				ResourceManager::instance()->registerFloat3(sn.technique, elementsOfCbuffers[i].name, elementsOfCbuffers[i].v, elementsOfCbuffers[i].elementsCount);
			if (variableLocations.count(elementsOfCbuffers[i].name) == 1)
			{
				ResourceManager::instance()->registerVariableLocation(
					sn.technique,
					elementsOfCbuffers[i].name,
					variableLocations[elementsOfCbuffers[i].name]
				);
			}

			if (elementsOfCbuffers[i].type.find("struct ") != std::string::npos)
			{
				StructVisitor structVisitor;
				structVisitor.structName = elementsOfCbuffers[i].type.substr(7);
				shader.query(&structVisitor);

				auto structInfo = structVisitor.structInfo;
				structInfo.ptr = elementsOfCbuffers[i].v;
				structInfo.elementsCount = elementsOfCbuffers[i].elementsCount;
				ResourceManager::instance()->registerStruct(sn.technique, elementsOfCbuffers[i].name, structInfo);
			}
		}

		for (int i = 0; i < globalVariablesVisitor.globalVariablesCount; ++i)
		{
			auto& gv = globalVariablesVisitor.globalVariables[i];
			if (gv.type == std::string("Texture2D"))
			{
				Texture2dResource t2dres;
				t2dres.tex = mShader->GetVariableByName(gv.name.c_str())->AsShaderResource();
				ResourceManager::instance()->registerTexture(sn.technique, gv.name, t2dres);
			}
			if (gv.type == std::string("Texture2DArray"))
			{
				Texture2dArrayResource t2dArrRes;
				t2dArrRes.texArr = mShader->GetVariableByName(gv.name.c_str())->AsShaderResource();
				ResourceManager::instance()->registerTexturesArray(sn.technique, gv.name, t2dArrRes);
			}
		}
	}
}

void processShader(ID3D11Device* device, LPCTSTR shader_path, LPCSTR config_path)
{
	ID3DX11Effect* mShader = createEffect(device, shader_path);
	if (mShader == nullptr)
		return;

	ShaderReader reader;
	reader.openShader(shader_path);

	HLSLConverter converter;
	reader.parseShader(converter);

	HLSLShader shader;
	converter.getShader(shader);

	registerResources(shader, device, mShader, config_path);
}

void getPathsToShaders(std::vector<std::basic_string<TCHAR>>& pathsToShaders)
{
	pathsToShaders.clear();

	TCHAR shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariable(_T("SHADERS"), shadersFolder, sz);
	
	path p(shadersFolder);
	for (auto i = directory_iterator(p); i != directory_iterator(); i++)
	{
		if (!is_directory(i->path()))
			pathsToShaders.push_back(std::basic_string<TCHAR>(shadersFolder) + _T("\\") + i->path().filename().c_str());
		else
			continue;
	}
}

void getPathsToShadersConfigs(std::vector<std::string>& pathsToShadersConfigs)
{
	pathsToShadersConfigs.clear();

	TCHAR shadersFolder[200];
	int sz = sizeof shadersFolder / sizeof * shadersFolder;
	GetEnvironmentVariable(_T("SHADERS"), shadersFolder, sz);

	char shadersConfigsFolder[200];
	sz = sizeof shadersConfigsFolder / sizeof * shadersConfigsFolder;
	GetEnvironmentVariableA("CONFIGS", shadersConfigsFolder, sz);

	path p(shadersFolder);
	for (auto i = directory_iterator(p); i != directory_iterator(); i++)
	{
		if (!is_directory(i->path()))
			pathsToShadersConfigs.push_back(std::string(shadersConfigsFolder) + "\\" + i->path().filename().string() + ".cfg");
		else
			continue;
	}
}

void processShaders(ID3D11Device* device)
{
	std::vector<std::basic_string<TCHAR>> pathsToShaders;
	getPathsToShaders(pathsToShaders);

	std::vector<std::string> pathsToShadersConfigs;
	getPathsToShadersConfigs(pathsToShadersConfigs);

	for (int i = 0; i < pathsToShaders.size(); ++i)
		processShader(device, pathsToShaders[i].c_str(), pathsToShadersConfigs[i].c_str());
}
