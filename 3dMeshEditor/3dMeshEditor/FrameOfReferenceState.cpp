#include "FrameOfReferenceState.h"
#include "Matrix4x4.h"
#include "GraphicsCore.h"
#include <algorithm>

std::shared_ptr<FrameOfReferenceState> FrameOfReferenceState::ptr{ nullptr };

FrameOfReferenceState::FrameOfReferenceState()
{
	// Initialization of frames of references
	framesOfReferences.setTechnique(frame_of_reference_id);
	framesOfReferences.setPass(p0_pass_id);

	framesOfReferences.gpuReadyData = &state;
	framesOfReferences.elementSize = sizeof FrameOfReference;

	const char* septh_stencil_state = "DepthEnable=false;DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;DepthFunc=D3D11_COMPARISON_ALWAYS;StencilEnable=false;StencilReadMask=0;StencilWriteMask=0;FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;";
	framesOfReferences.setDepthStencilState(StringManager::toStringId(septh_stencil_state));
}

void FrameOfReferenceState::attach(FrameOfReferenceStateObserver* observer)
{
	observers.push_back(observer);
}

void FrameOfReferenceState::detach(const FrameOfReferenceStateObserver* observer)
{
	auto it = std::find(observers.begin(), observers.end(), observer);
	if (it == observers.end())
		return;
	observers.erase(it);
}

void FrameOfReferenceState::notify()
{
	for (auto it : observers)
		it->update();
}

void FrameOfReferenceState::moveAlongAxisX(float x)
{
	flt3 axis = state.axis0;
	axis.normalize();

	state.posW += x * axis;

	notify();
}

void FrameOfReferenceState::moveAlongAxisY(float y)
{
	flt3 axis = state.axis1;
	axis.normalize();

	state.posW += y * axis;

	notify();
}

void FrameOfReferenceState::moveAlongAxisZ(float z)
{
	flt3 axis = state.axis2;
	axis.normalize();

	state.posW += z * axis;

	notify();
}

void FrameOfReferenceState::rotateAlongAxisX(float x)
{
	auto m = makeRotate<float>(state.axis0, x);
	state.axis1 = state.axis1 * m;
	state.axis2 = state.axis2 * m;
	notify();
}

void FrameOfReferenceState::rotateAlongAxisY(float y)
{
	auto m = makeRotate<float>(state.axis1, y);
	state.axis0 = state.axis0 * m;
	state.axis2 = state.axis2 * m;
	notify();
}

void FrameOfReferenceState::rotateAlongAxisZ(float z)
{
	auto m = makeRotate<float>(state.axis2, z);
	state.axis0 = state.axis0 * m;
	state.axis1 = state.axis1 * m;
	notify();
}

void FrameOfReferenceState::scaleAlongAxisX(float x)
{
	state.axis0 *= x;
	notify();
}

void FrameOfReferenceState::scaleAlongAxisY(float y)
{
	state.axis1 *= y;
	notify();
}

void FrameOfReferenceState::scaleAlongAxisZ(float z)
{
	state.axis2 *= z;
	notify();
}

void FrameOfReferenceState::setFrameOfReference(const FrameOfReference& frameOfReference)
{
	state = frameOfReference;
	notify();
}

auto FrameOfReferenceState::instance()->std::shared_ptr<FrameOfReferenceState>
{
	if (!ptr)
		ptr = std::make_shared<FrameOfReferenceState>(new FrameOfReferenceState());
	return ptr;
}

void FrameOfReferenceState::draw()
{
	GraphicsCore::instance()->draw(framesOfReferences);
}