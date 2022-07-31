#include "Shader.h"
#include <d3dx11async.h>

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
}
