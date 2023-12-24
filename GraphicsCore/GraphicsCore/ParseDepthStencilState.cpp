#include "ParseDepthStencilState.h"
#include <algorithm>
#include <string>
#include <map>

int readParam(const char* depthStencilState, int off)
{
	int len = std::strlen(depthStencilState);
	auto endOfParam = std::find(depthStencilState + off, depthStencilState + len, '=');
	return endOfParam - depthStencilState;
}

int readValue(const char* depthStencilState, int off)
{
	int len = std::strlen(depthStencilState);
	auto endOfValue = std::find(depthStencilState + off, depthStencilState + len, ';');
	return endOfValue - depthStencilState;
}

void parseDepthStencilState(const char* szDepthStencilState, D3D11_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = false;
	depthStencilDesc.StencilReadMask = 0;
	depthStencilDesc.StencilWriteMask = 0;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

	int pos = 0;
	int len = std::strlen(szDepthStencilState);
	while (pos != len)
	{
		int p = readParam(szDepthStencilState, pos);
		std::string param(szDepthStencilState + pos, p - pos);

		pos = p + 1;
		int p = readValue(szDepthStencilState, pos);
		std::string value(szDepthStencilState + pos, p - pos);

		pos = p + 1;

		std::map<std::string, bool> toBool;
		toBool["true"] = true;
		toBool["false"] = false;

		std::map<std::string, D3D11_DEPTH_WRITE_MASK> toDepthWriteMask;
		toDepthWriteMask["D3D11_DEPTH_WRITE_MASK_ZERO"] = D3D11_DEPTH_WRITE_MASK_ZERO;
		toDepthWriteMask["D3D11_DEPTH_WRITE_MASK_ALL"] = D3D11_DEPTH_WRITE_MASK_ALL;

		std::map<std::string, D3D11_COMPARISON_FUNC> toComparisonFunc;
		toComparisonFunc["D3D11_COMPARISON_NEVER"] = D3D11_COMPARISON_NEVER;
		toComparisonFunc["D3D11_COMPARISON_LESS"] = D3D11_COMPARISON_LESS;
		toComparisonFunc["D3D11_COMPARISON_EQUAL"] = D3D11_COMPARISON_EQUAL;
		toComparisonFunc["D3D11_COMPARISON_LESS_EQUAL"] = D3D11_COMPARISON_LESS_EQUAL;
		toComparisonFunc["D3D11_COMPARISON_GREATER"] = D3D11_COMPARISON_GREATER;
		toComparisonFunc["D3D11_COMPARISON_NOT_EQUAL"] = D3D11_COMPARISON_NOT_EQUAL;
		toComparisonFunc["D3D11_COMPARISON_GREATER_EQUAL"] = D3D11_COMPARISON_GREATER_EQUAL;
		toComparisonFunc["D3D11_COMPARISON_ALWAYS"] = D3D11_COMPARISON_ALWAYS;

		if (param == "DepthEnable")
		{
			depthStencilDesc.DepthEnable = toBool[value];
			continue;
		}
		if (param == "DepthWriteMask")
		{
			depthStencilDesc.DepthWriteMask = toDepthWriteMask[value];
			continue;
		}
		if (param == "DepthFunc")
		{
			depthStencilDesc.DepthFunc = toComparisonFunc[value];
			continue;
		}
	}
}