#include "Creator.h"
#include "GraphicsCore.h"

void Creator::init()
{
	baseOfFrameworkGeo.setTechnique(StringManager::toStringId("BaseOfFramework"));
	baseOfFrameworkGeo.setPass(p0_pass_id);

	const char* depth_stencil_state = "DepthEnable=false;DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;DepthFunc=D3D11_COMPARISON_ALWAYS;StencilEnable=false;StencilReadMask=0;StencilWriteMask=0;FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;";
	baseOfFrameworkGeo.setDepthStencilState(StringManager::toStringId(depth_stencil_state));

	const char* blend_state = "RenderTarget[0].BlendEnable=TRUE;RenderTarget[0].SrcBlend=D3D11_BLEND_INV_SRC_ALPHA;RenderTarget[0].DestBlend=D3D11_BLEND_SRC_ALPHA;RenderTarget[0].BlendOp=D3D11_BLEND_OP_ADD;RenderTarget[0].RenderTargetWriteMask=15";
	baseOfFrameworkGeo.setBlendState(StringManager::toStringId(blend_state));

	baseOfFrameworkGeo.gpuReadyData = &baseOfFramework;
	baseOfFrameworkGeo.elementSize = sizeof BaseOfFramework;
	baseOfFrameworkGeo.verticesCount = 1;
}

void Creator::draw()
{
	if (bDrawBase)
		GraphicsCore::instance()->draw(baseOfFrameworkGeo);
}