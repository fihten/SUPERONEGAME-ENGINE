#pragma once
#include <d3dx11effect.h>
#include <d3d11.h>

class Shader
{
public:
	void init(ID3D11Device* device, LPCTSTR path);
private:
	ID3DX11Effect* mShader = nullptr;
};