#include "Shader.h"
#include "ShaderInterpreter.h"
#include <d3dx11async.h>
#include <fstream>
#include <cstddef>
#include <winbase.h>
#include <tchar.h>

#include <boost/filesystem.hpp>

#include "Visitors.h"
#include "ResourceManager.h"

using namespace boost::filesystem;

void processShader(ID3D11Device* device, LPCTSTR path)
{
	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3DX11Effect* mShader = nullptr;
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	D3DX11CompileFromFile(path, 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, device, &mShader);

	// interpret shader
	std::ifstream shaderFile(path);

	shaderFile.seekg(0, shaderFile.end);
	int length = shaderFile.tellg();
	shaderFile.seekg(0, shaderFile.beg);

	char* shaderText = new char[length + 1];
	shaderFile.read(shaderText, length);
	shaderText[length] = 0;

	ShaderInterpreter interpreter;
	interpreter.setShaderText(shaderText);
	ShaderUnits::SHADER* shader = interpreter.build();

	ShadersNamesVisitor shadersNamesVisitor;
	shader->query(&shadersNamesVisitor);

	ElementsOfCbufferVisitor elementsOfCbufferVisitor;
	shader->query(&elementsOfCbufferVisitor);

	ElementOfCbuffer* elementsOfCbuffers = nullptr;
	int countOfCbufferElements = 0;
	elementsOfCbufferVisitor.getElements(elementsOfCbuffers, countOfCbufferElements);

	for (int i = 0; i < countOfCbufferElements; ++i)
		elementsOfCbuffers[i].v = mShader->GetVariableByName(elementsOfCbuffers[i].name.c_str());

	std::vector<ShadersNamesVisitor::ShadersNames> shadersNames = shadersNamesVisitor.getShadersNames();
	for (auto& sn : shadersNames)
	{
		ID3DX11EffectTechnique* technique = mShader->GetTechniqueByName(sn.technique.c_str());
		resourceManager.registerTechnique(sn.technique, technique);

		for (int i = 0; i < sn.passes.size(); ++i)
		{
			ID3DX11EffectPass* pass = technique->GetPassByName(sn.passes[i].c_str());
			resourceManager.registerPass(sn.technique, sn.passes[i], pass);

			InputLayoutVisitor inputLayoutVisitor;
			inputLayoutVisitor.setShaderName(sn.shaders[i]);

			shader->query(&inputLayoutVisitor);

			D3DX11_PASS_DESC passDesc;
			pass->GetDesc(&passDesc);

			ID3D11InputLayout* inputLayout = inputLayoutVisitor.getInputLayout(device, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize);
			resourceManager.registerInputLayout(sn.technique, sn.passes[i], inputLayout);
			resourceManager.registerStreamsInfo(sn.technique, sn.passes[i], inputLayoutVisitor.getStreamsInfo());
		}

		for (int i = 0; i < countOfCbufferElements; ++i)
		{
			if (elementsOfCbuffers[i].type == std::string("float4x4"))
				resourceManager.registerMatrix(sn.technique, elementsOfCbuffers[i].name, elementsOfCbuffers[i].v->AsMatrix());
		}
	}
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
			pathsToShaders.push_back(std::basic_string<TCHAR>(shadersFolder) + _T("//") + i->path().filename().c_str());
		else
			continue;
	}
}

void processShaders(ID3D11Device* device)
{
	std::vector<std::basic_string<TCHAR>> pathsToShaders;
	getPathsToShaders(pathsToShaders);
	for (const auto& pathToShader : pathsToShaders)
		processShader(device, pathToShader.c_str());
}

std::map<std::string, std::string>&& loadVariableLocationsFromFile(const std::string& path)
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
		return std::map<std::string, std::string>();

	beg += sizeof name / sizeof * name;
	beg = sConfig.find("{", beg);
	beg += 1;



	return std::map<std::string, std::string>();
}