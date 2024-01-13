#include "FrameOfReferenceState.h"
#include "Matrix4x4.h"
#include "GraphicsCore.h"
#include "Cameras.h"
#include "MathUtils.h"
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

	framesOfReferences.verticesCount = 1;
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

void FrameOfReferenceState::notify(UpdateType updateType)
{
	for (auto it : observers)
		it->update(updateType);
}

void FrameOfReferenceState::moveAlongAxisX(float x)
{
	flt3 axis = state.axis0;
	axis.normalize();

	state.posW += x * axis;
	dPos = makeTranslation(x * axis);

	notify(UpdateType::Translation);
}

void FrameOfReferenceState::moveAlongAxisY(float y)
{
	flt3 axis = state.axis1;
	axis.normalize();

	state.posW += y * axis;
	dPos = makeTranslation(y * axis);

	notify(UpdateType::Translation);
}

void FrameOfReferenceState::moveAlongAxisZ(float z)
{
	flt3 axis = state.axis2;
	axis.normalize();

	state.posW += z * axis;
	dPos = makeTranslation(z * axis);

	notify(UpdateType::Translation;
}

void FrameOfReferenceState::rotateAlongAxisX(float x)
{
	dPos = makeRotate<float>(state.axis0, x);
	state.axis1 = state.axis1 * dPos;
	state.axis2 = state.axis2 * dPos;
	notify(UpdateType::Rotation);
}

void FrameOfReferenceState::rotateAlongAxisY(float y)
{
	dPos = makeRotate<float>(state.axis1, y);
	state.axis0 = state.axis0 * dPos;
	state.axis2 = state.axis2 * dPos;
	notify(UpdateType::Rotation);
}

void FrameOfReferenceState::rotateAlongAxisZ(float z)
{
	dPos = makeRotate<float>(state.axis2, z);
	state.axis0 = state.axis0 * dPos;
	state.axis1 = state.axis1 * dPos;
	notify(UpdateType::Rotation);
}

void FrameOfReferenceState::scaleAlongAxisX(float x)
{
	state.axis0 *= x;
	notify(UpdateType::Scaling);
}

void FrameOfReferenceState::scaleAlongAxisY(float y)
{
	state.axis1 *= y;
	notify(UpdateType::Scaling);
}

void FrameOfReferenceState::scaleAlongAxisZ(float z)
{
	state.axis2 *= z;
	notify(UpdateType::Scaling);
}

void FrameOfReferenceState::setFrameOfReference(const FrameOfReference& frameOfReference)
{
	state = frameOfReference;
	notify();
}

void FrameOfReferenceState::turnOn()
{
	bTurnedOn = true;
}

void FrameOfReferenceState::turnOff()
{
	bTurnedOn = false;
}

auto FrameOfReferenceState::instance()->std::shared_ptr<FrameOfReferenceState>
{
	if (!ptr)
		ptr = std::make_shared<FrameOfReferenceState>(new FrameOfReferenceState());
	return ptr;
}

void FrameOfReferenceState::draw()
{
	if (bTurnedOn)
		GraphicsCore::instance()->draw(framesOfReferences);
}

IntersectedAxis FrameOfReferenceState::checkIntersection(float mousePosX, float mousePosY)
{
	auto& camera = cameras()[MAIN_CAMERA];

	float nearZ = camera.getNear();
	float fovY = camera.getFov() * M_PI / 180;
	float h = 2 * nearZ * std::tan(0.5 * fovY);

	float ar = camera.getAspectRatio();
	float w = ar * h;

	mousePosX *= 0.5 * w;
	mousePosY *= 0.5 * h;

	flt3 pt(mousePosX, mousePosY, nearZ);

	float farZ = camera.getFar();
	flt3 dir(mousePosX * (farZ / nearZ - 1), mousePosY * (farZ / nearZ - 1), farZ - nearZ);

	flt4 posW4(state.posW, 1.0f);
	auto v = camera.getView();
	flt3 origin = (posW4 * v).xyz();

	float threshold = 1e-2;

	// check axis x
	flt3 axisX = state.axis0;
	axisX = axisX * v;
	float distance = distanceBetweenLineAndSegment(pt, dir, origin, origin + axisX);
	if (distance < threshold)
		return IntersectedAxis::AXIS_X;

	// check axis y
	flt3 axisY = state.axis1;
	axisY = axisY * v;
	float distance = distanceBetweenLineAndSegment(pt, dir, origin, origin + axisY);
	if (distance < threshold)
		return IntersectedAxis::AXIS_Y;

	// check axis z
	flt3 axisZ = state.axis2;
	axisZ = axisZ * v;
	float distance = distanceBetweenLineAndSegment(pt, dir, origin, origin + axisZ);
	if (distance < threshold)
		return IntersectedAxis::AXIS_Z;

	return IntersectedAxis::NONE;
}

float FrameOfReferenceState::projectOnXaxis(float mousePosX, float mousePosY)
{
	auto& camera = cameras()[MAIN_CAMERA];

	float nearZ = camera.getNear();
	float fovY = camera.getFov() * M_PI / 180;
	float h = 2 * nearZ * std::tan(0.5 * fovY);

	float ar = camera.getAspectRatio();
	float w = ar * h;

	mousePosX *= 0.5 * w;
	mousePosY *= 0.5 * h;

	flt3 pt(mousePosX, mousePosY, nearZ);

	float farZ = camera.getFar();
	flt3 dir(mousePosX * (farZ / nearZ - 1), mousePosY * (farZ / nearZ - 1), farZ - nearZ);

	flt4 posW4(state.posW, 1.0f);
	auto v = camera.getView();
	flt3 origin = (posW4 * v).xyz();

	flt3 axisX = state.axis0;
	axisX = axisX * v;
	axisX.normalize();

	float t0 = 0;
	float t1 = 0;
	distanceBetweenLines(pt, dir, origin, origin + axisX, &t0, &t1);

	return t1;
}

float FrameOfReferenceState::projectOnYaxis(float mousePosX, float mousePosY)
{
	auto& camera = cameras()[MAIN_CAMERA];

	float nearZ = camera.getNear();
	float fovY = camera.getFov() * M_PI / 180;
	float h = 2 * nearZ * std::tan(0.5 * fovY);

	float ar = camera.getAspectRatio();
	float w = ar * h;

	mousePosX *= 0.5 * w;
	mousePosY *= 0.5 * h;

	flt3 pt(mousePosX, mousePosY, nearZ);

	float farZ = camera.getFar();
	flt3 dir(mousePosX * (farZ / nearZ - 1), mousePosY * (farZ / nearZ - 1), farZ - nearZ);

	flt4 posW4(state.posW, 1.0f);
	auto v = camera.getView();
	flt3 origin = (posW4 * v).xyz();

	flt3 axisY = state.axis1;
	axisY = axisY * v;
	axisY.normalize();

	float t0 = 0;
	float t1 = 0;
	distanceBetweenLines(pt, dir, origin, origin + axisY, &t0, &t1);

	return t1;
}

float FrameOfReferenceState::projectOnZaxis(float mousePosX, float mousePosY)
{
	auto& camera = cameras()[MAIN_CAMERA];

	float nearZ = camera.getNear();
	float fovY = camera.getFov() * M_PI / 180;
	float h = 2 * nearZ * std::tan(0.5 * fovY);

	float ar = camera.getAspectRatio();
	float w = ar * h;

	mousePosX *= 0.5 * w;
	mousePosY *= 0.5 * h;

	flt3 pt(mousePosX, mousePosY, nearZ);

	float farZ = camera.getFar();
	flt3 dir(mousePosX * (farZ / nearZ - 1), mousePosY * (farZ / nearZ - 1), farZ - nearZ);

	flt4 posW4(state.posW, 1.0f);
	auto v = camera.getView();
	flt3 origin = (posW4 * v).xyz();

	flt3 axisZ = state.axis2;
	axisZ = axisZ * v;
	axisZ.normalize();

	float t0 = 0;
	float t1 = 0;
	distanceBetweenLines(pt, dir, origin, origin + axisZ, &t0, &t1);

	return t1;
}

flt4x4& FrameOfReferenceState::getDiffPosition()
{
	return dPos;
}