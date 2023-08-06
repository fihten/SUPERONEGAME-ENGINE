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
		
		string_id technique_id = StringManager::toStringId(sn.technique);
		ResourceManager::instance()->registerTechnique(technique_id, technique);
		for (int i = 0; i < sn.passes.size(); ++i)
		{
			ID3DX11EffectPass* pass = technique->GetPassByName(sn.passes[i].c_str());

			string_id pass_id = StringManager::toStringId(sn.passes[i]);
			ResourceManager::instance()->registerPass(technique_id, pass_id, pass);

			InputLayoutVisitor inputLayoutVisitor;
			inputLayoutVisitor.setShaderName(sn.shaders[i]);

			shader.query(&inputLayoutVisitor);

			D3DX11_PASS_DESC passDesc;
			pass->GetDesc(&passDesc);

			ID3D11InputLayout* inputLayout = inputLayoutVisitor.getInputLayout(device, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize);
			ResourceManager::instance()->registerInputLayout(technique_id, pass_id, inputLayout);

			std::vector<InputLayoutResource::StreamInfo> streamsInfo;
			inputLayoutVisitor.getStreamsInfo(streamsInfo);
			ResourceManager::instance()->registerStreamsInfo(technique_id, pass_id, streamsInfo);

			GeometryShaderInfoVisitor geometryShaderInfoVisitor;
			geometryShaderInfoVisitor.technique = sn.technique;
			geometryShaderInfoVisitor.pass = sn.passes[i];

			shader.query(&geometryShaderInfoVisitor);
			if (geometryShaderInfoVisitor.geometryShaderIsPresented)
			{
				ResourceManager::instance()->registerPresenceOfGeometryShader(technique_id, pass_id);
				ResourceManager::instance()->registerPrimitiveType(technique_id, pass_id, geometryShaderInfoVisitor.primType);
			}
		}

		for (int i = 0; i < countOfCbufferElements; ++i)
		{
			string_id name_id = StringManager::toStringId(elementsOfCbuffers[i].name);
			if (elementsOfCbuffers[i].type == std::string("float4x4"))
				ResourceManager::instance()->registerFloat4x4(technique_id, name_id, elementsOfCbuffers[i].v->AsMatrix(), elementsOfCbuffers[i].elementsCount);
			if (elementsOfCbuffers[i].type == std::string("float4"))
				ResourceManager::instance()->registerFloat4(technique_id, name_id, elementsOfCbuffers[i].v, elementsOfCbuffers[i].elementsCount);
			if (elementsOfCbuffers[i].type == std::string("float3"))
				ResourceManager::instance()->registerFloat3(technique_id, name_id, elementsOfCbuffers[i].v, elementsOfCbuffers[i].elementsCount);
			if (elementsOfCbuffers[i].type == std::string("float2"))
				ResourceManager::instance()->registerFloat2(technique_id, name_id, elementsOfCbuffers[i].v, elementsOfCbuffers[i].elementsCount);
			if (elementsOfCbuffers[i].type == std::string("float"))
				ResourceManager::instance()->registerFloat1(technique_id, name_id, elementsOfCbuffers[i].v, elementsOfCbuffers[i].elementsCount);
			if (variableLocations.count(elementsOfCbuffers[i].name) == 1)
			{
				VariableLocation location;

				std::string& place = variableLocations[elementsOfCbuffers[i].name];

				auto placeEnd = std::remove_if(place.begin(), place.end(), [&](char c)->bool {return c == ' '; });
				place = place.substr(0, placeEnd - place.begin());

				size_t startOfMultiplier = 0;
				int mi = -1;
				while (startOfMultiplier != place.length() + 1)
				{
					size_t endOfMultiplier = place.find_first_of('*', startOfMultiplier);
					if (endOfMultiplier == std::string::npos)
						endOfMultiplier = place.length();

					std::string multiplier = place.substr(startOfMultiplier, endOfMultiplier - startOfMultiplier);
					startOfMultiplier = endOfMultiplier + 1;

					if (std::strcmp(multiplier.c_str(), "") == 0)
						continue;

					mi++;

					size_t pos = multiplier.find("cameras[");
					if (pos == 0)
					{
						location.name[mi] = cameras_id;

						size_t beg = 7;
						size_t end = multiplier.find(']', beg);

						int index = std::atoi(std::string(multiplier, beg, end - beg).c_str());

						location.index[mi] = index;

						beg = end + 2;
						std::string what(multiplier, beg, std::string::npos);
						if (std::strcmp(what.c_str(), "WVP") == 0)
							location.field[mi] = wvp_id;
						if (std::strcmp(what.c_str(), "VP") == 0)
							location.field[mi] = vp_id;
						if (std::strcmp(what.c_str(), "V") == 0)
							location.field[mi] = v_id;
						if (std::strcmp(what.c_str(), "P") == 0)
							location.field[mi] = p_id;
						if (std::strcmp(what.c_str(), "eye_pos") == 0)
							location.field[mi] = eye_pos_id;
						if (std::strcmp(what.c_str(), "fwd") == 0)
							location.field[mi] = fwd_id;
						continue;
					}
					if (std::strcmp(multiplier.c_str(), "position_in_scene") == 0)
						location.name[mi] = position_in_scene_id;
					if (std::strcmp(multiplier.c_str(), "inverse_transpose_of_position_in_scene") == 0)
						location.name[mi] = inverse_transpose_of_position_in_scene_id;
				}
				location.countOfMultipliers = mi + 1;

				ResourceManager::instance()->registerVariableLocation(
					technique_id,
					name_id,
					location
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
				ResourceManager::instance()->registerStruct(technique_id, name_id, structInfo);
			}
		}

		for (int i = 0; i < globalVariablesVisitor.globalVariablesCount; ++i)
		{
			auto& gv = globalVariablesVisitor.globalVariables[i];
			string_id gv_name_id = StringManager::toStringId(gv.name);
			if (gv.type == std::string("Texture2D"))
			{
				Texture2dResource t2dres;
				t2dres.tex = mShader->GetVariableByName(gv.name.c_str())->AsShaderResource();
				ResourceManager::instance()->registerTexture(technique_id, gv_name_id, t2dres);
			}
			if (gv.type == std::string("Texture2DArray"))
			{
				Texture2dArrayResource t2dArrRes;
				t2dArrRes.texArr = mShader->GetVariableByName(gv.name.c_str())->AsShaderResource();
				ResourceManager::instance()->registerTexturesArray(technique_id, gv_name_id, t2dArrRes);
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
