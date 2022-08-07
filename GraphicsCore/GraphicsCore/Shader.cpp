#include "Shader.h"
#include "ShaderInterpreter.h"
#include <d3dx11async.h>
#include <fstream>
#include <cstddef>

#include "Visitors.h"
#include "ResourceManager.h"

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
		}
	}
}
