#include "SphereCreator.h"

SphereCreator::SphereCreator()
{
	sphereFrameworkGeo.setTechnique(StringManager::toStringId("SphereFramework"));
	sphereFrameworkGeo.setPass(p0_pass_id);

	const char* depth_stencil_state = "DepthEnable=false;DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;DepthFunc=D3D11_COMPARISON_ALWAYS;StencilEnable=false;StencilReadMask=0;StencilWriteMask=0;FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;";
	sphereFrameworkGeo.setDepthStencilState(StringManager::toStringId(depth_stencil_state));

	sphereFrameworkGeo.gpuReadyData = &sphereFramework;
	sphereFrameworkGeo.elementSize = sizeof SphereFramework;
	sphereFrameworkGeo.verticesCount = 1;

}