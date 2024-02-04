#include "FrameOfReferenceState.h"
#include "FrameOfReferenceStateObserver.h"
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

	const char* depth_stencil_state = "DepthEnable=false;DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;DepthFunc=D3D11_COMPARISON_ALWAYS;StencilEnable=false;StencilReadMask=0;StencilWriteMask=0;FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;";
	framesOfReferences.setDepthStencilState(StringManager::toStringId(depth_stencil_state));

	framesOfReferences.verticesCount = 1;

	// Initialization of spheric frames of references
	sphericFramesOfReferences.setTechnique(spheric_frame_of_reference_id);
	sphericFramesOfReferences.setPass(p0_pass_id);

	sphericFramesOfReferences.gpuReadyData = &spheric_state;
	sphericFramesOfReferences.elementSize = sizeof SphericFrameOfReference;

	sphericFramesOfReferences.setDepthStencilState(StringManager::toStringId(depth_stencil_state));

	sphericFramesOfReferences.verticesCount = 1;

	// Initialization of scale frames of references
	scaleFramesOfReferences.setTechnique(scale_frame_of_reference_id);
	scaleFramesOfReferences.setPass(p0_pass_id);

	scaleFramesOfReferences.gpuReadyData = &scale_state;
	scaleFramesOfReferences.elementSize = sizeof ScaleFrameOfReference;

	scaleFramesOfReferences.setDepthStencilState(StringManager::toStringId(depth_stencil_state));

	scaleFramesOfReferences.verticesCount = 1;
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
	if (x == 0)
		return;

	flt3 shift = state.axis0;
	shift.normalize();
	shift *= x;

	state.posW += shift;
	scale_state.posW = state.posW;
	spheric_state.posW = state.posW;
	dPos = makeTranslation(shift);

	notify(UpdateType::Translation);
}

void FrameOfReferenceState::moveAlongAxisY(float y)
{
	if (y == 0)
		return;

	flt3 shift = state.axis1;
	shift.normalize();
	shift *= y;

	state.posW += shift;
	scale_state.posW = state.posW;
	spheric_state.posW = state.posW;
	dPos = makeTranslation(shift);

	notify(UpdateType::Translation);
}

void FrameOfReferenceState::moveAlongAxisZ(float z)
{
	if (z == 0)
		return;

	flt3 shift = state.axis2;
	shift.normalize();
	shift *= z;

	state.posW += shift;
	scale_state.posW = state.posW;
	spheric_state.posW = state.posW;
	dPos = makeTranslation(shift);

	notify(UpdateType::Translation);
}

void FrameOfReferenceState::rotateAlongAxisX(float x)
{
	dPos = makeRotate<float>(state.axis0, x);
	
	state.axis1 = state.axis1 * dPos;
	state.axis2 = state.axis2 * dPos;

	scale_state.axis1 = scale_state.axis1 * dPos;
	scale_state.axis2 = scale_state.axis2 * dPos;

	spheric_state.axis1 = state.axis1;
	spheric_state.axis2 = state.axis2;

	spheric_state.axis1.normalize();
	spheric_state.axis2.normalize();

	flt3 pt = state.posW;
	pt *= -1;
	auto t0 = makeTranslation(pt);
	pt *= -1;
	auto t1 = makeTranslation(pt);

	dPos = t0 * dPos * t1;

	notify(UpdateType::Rotation);
}

void FrameOfReferenceState::rotateAlongAxisY(float y)
{
	dPos = makeRotate<float>(state.axis1, y);
	
	state.axis0 = state.axis0 * dPos;
	state.axis2 = state.axis2 * dPos;

	scale_state.axis0 = scale_state.axis0 * dPos;
	scale_state.axis2 = scale_state.axis2 * dPos;

	spheric_state.axis0 = state.axis0;
	spheric_state.axis2 = state.axis2;

	spheric_state.axis0.normalize();
	spheric_state.axis2.normalize();

	flt3 pt = state.posW;
	pt *= -1;
	auto t0 = makeTranslation(pt);
	pt *= -1;
	auto t1 = makeTranslation(pt);

	dPos = t0 * dPos * t1;

	notify(UpdateType::Rotation);
}

void FrameOfReferenceState::rotateAlongAxisZ(float z)
{
	dPos = makeRotate<float>(state.axis2, z);
	
	state.axis0 = state.axis0 * dPos;
	state.axis1 = state.axis1 * dPos;

	scale_state.axis0 = scale_state.axis0 * dPos;
	scale_state.axis1 = scale_state.axis1 * dPos;

	spheric_state.axis0 = state.axis0;
	spheric_state.axis1 = state.axis1;

	spheric_state.axis0.normalize();
	spheric_state.axis1.normalize();

	flt3 pt = state.posW;
	pt *= -1;
	auto t0 = makeTranslation(pt);
	pt *= -1;
	auto t1 = makeTranslation(pt);

	dPos = t0 * dPos * t1;

	notify(UpdateType::Rotation);
}

void FrameOfReferenceState::scaleAlongAxisX(float x)
{
	state.axis0 *= x;
	scale_state.axis0 *= x;
	spheric_state.radius = std::max<float>(spheric_state.radius, state.axis0.length());

	flt3 pt = scale_state.posW;
	pt *= -1;
	auto t0 = makeTranslation(pt);
	pt *= -1;
	auto t1 = makeTranslation(pt);

	flt3 scale(x, 1, 1);
	dPos = makeScale<float>(scale);
	dPos = t0 * dPos * t1;

	notify(UpdateType::Scaling);
}

void FrameOfReferenceState::scaleAlongAxisY(float y)
{
	state.axis1 *= y;
	scale_state.axis1 *= y;
	spheric_state.radius = std::max<float>(spheric_state.radius, state.axis1.length());

	flt3 pt = scale_state.posW;
	pt *= -1;
	auto t0 = makeTranslation(pt);
	pt *= -1;
	auto t1 = makeTranslation(pt);

	flt3 scale(1, y, 1);
	dPos = makeScale<float>(scale);
	dPos = t0 * dPos * t1;

	notify(UpdateType::Scaling);
}

void FrameOfReferenceState::scaleAlongAxisZ(float z)
{
	state.axis2 *= z;
	scale_state.axis2 *= z;
	spheric_state.radius = std::max<float>(spheric_state.radius, state.axis2.length());

	flt3 pt = scale_state.posW;
	pt *= -1;
	auto t0 = makeTranslation(pt);
	pt *= -1;
	auto t1 = makeTranslation(pt);

	flt3 scale(1, 1, z);
	dPos = makeScale<float>(scale);
	dPos = t0 * dPos * t1;

	notify(UpdateType::Scaling);
}

void FrameOfReferenceState::release()
{
	notify(UpdateType::Release);
}

void FrameOfReferenceState::setFrameOfReference(const FrameOfReference& frameOfReference)
{
	state = frameOfReference;

	spheric_state.posW = state.posW;

	spheric_state.axis0 = state.axis0;
	spheric_state.axis0.normalize();

	spheric_state.axis1 = state.axis1;
	spheric_state.axis1.normalize();

	spheric_state.axis2 = state.axis2;
	spheric_state.axis2.normalize();

	spheric_state.radius = std::max<float>(
		std::max<float>(state.axis0.length(), state.axis1.length()), state.axis2.length());

	scale_state.posW = state.posW;

	scale_state.axis0 = state.axis0;
	scale_state.axis1 = state.axis1;
	scale_state.axis2 = state.axis2;

	notify(UpdateType::SetFrameOfReference);
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
		ptr = std::shared_ptr<FrameOfReferenceState>(new FrameOfReferenceState());
	return ptr;
}

void FrameOfReferenceState::draw()
{
	if (bTurnedOn)
		GraphicsCore::instance()->draw(framesOfReferences);
}

void FrameOfReferenceState::drawSpheric()
{
	if (bTurnedOn)
		GraphicsCore::instance()->draw(sphericFramesOfReferences);
}

void FrameOfReferenceState::drawScale()
{
	if (bTurnedOn)
		GraphicsCore::instance()->draw(scaleFramesOfReferences);
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
	float distance = distanceBetweenLineAndSegment(pt, dir, origin - axisX, origin + axisX);
	if (distance < threshold)
		return IntersectedAxis::AXIS_X;

	// check axis y
	flt3 axisY = state.axis1;
	axisY = axisY * v;
	distance = distanceBetweenLineAndSegment(pt, dir, origin - axisY, origin + axisY);
	if (distance < threshold)
		return IntersectedAxis::AXIS_Y;

	// check axis z
	flt3 axisZ = state.axis2;
	axisZ = axisZ * v;
	distance = distanceBetweenLineAndSegment(pt, dir, origin - axisZ, origin + axisZ);
	if (distance < threshold)
		return IntersectedAxis::AXIS_Z;

	return IntersectedAxis::NONE;
}

IntersectedCircleAxis FrameOfReferenceState::checkCircleIntersection(float mousePosX, float mousePosY)
{
	float threshold = 0.1;

	flt2 i = this->intersectWithOxy(mousePosX, mousePosY);
	float r = sqrt(i.x() * i.x() + i.y() * i.y());
	if (std::fabs(r - spheric_state.radius) < threshold)
		return IntersectedCircleAxis::PLAIN_XY;

	i = this->intersectWithOxz(mousePosX, mousePosY);
	r = sqrt(i.x() * i.x() + i.y() * i.y());
	if (std::fabs(r - spheric_state.radius) < threshold)
		return IntersectedCircleAxis::PLAIN_XZ;

	i = this->intersectWithOyz(mousePosX, mousePosY);
	r = sqrt(i.x() * i.x() + i.y() * i.y());
	if (std::fabs(r - spheric_state.radius) < threshold)
		return IntersectedCircleAxis::PLAIN_YZ;

	return IntersectedCircleAxis::NONE;
}

IntersectedAxis FrameOfReferenceState::checkHandleIntersection(float mousePosX, float mousePosY)
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

	/////////////////////////////////////////////////////////////////

	flt3 endOfAxis = scale_state.posW - scale_state.axis0;
	float distance = distanceBetweenLineAndPoint(
		pt, dir,
		endOfAxis
	);
	if (distance < scale_state.handleRadius)
		return IntersectedAxis::AXIS_X;

	endOfAxis = scale_state.posW + scale_state.axis0;
	distance = distanceBetweenLineAndPoint(
		pt, dir,
		endOfAxis
	);
	if (distance < scale_state.handleRadius)
		return IntersectedAxis::AXIS_X;

	/////////////////////////////////////////////////////////////////

	endOfAxis = scale_state.posW - scale_state.axis1;
	distance = distanceBetweenLineAndPoint(
		pt, dir,
		endOfAxis
	);
	if (distance < scale_state.handleRadius)
		return IntersectedAxis::AXIS_Y;

	endOfAxis = scale_state.posW + scale_state.axis1;
	distance = distanceBetweenLineAndPoint(
		pt, dir,
		endOfAxis
	);
	if (distance < scale_state.handleRadius)
		return IntersectedAxis::AXIS_Y;

	/////////////////////////////////////////////////////////////////

	endOfAxis = scale_state.posW - scale_state.axis2;
	distance = distanceBetweenLineAndPoint(
		pt, dir,
		endOfAxis
	);
	if (distance < scale_state.handleRadius)
		return IntersectedAxis::AXIS_Z;

	endOfAxis = scale_state.posW + scale_state.axis2;
	distance = distanceBetweenLineAndPoint(
		pt, dir,
		endOfAxis
	);
	if (distance < scale_state.handleRadius)
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
	distanceBetweenLines(pt, dir, origin, axisX, &t0, &t1);

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
	distanceBetweenLines(pt, dir, origin, axisY, &t0, &t1);

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
	distanceBetweenLines(pt, dir, origin, axisZ, &t0, &t1);

	return t1;
}

float FrameOfReferenceState::projectOnXYcircle(float mousePosX, float mousePosY)
{
	flt2 i = this->intersectWithOxy(mousePosX, mousePosY);
	float angle = atan2(i.y(), i.x()) * 180.0f / M_PI;
	return angle;
}

float FrameOfReferenceState::projectOnXZcircle(float mousePosX, float mousePosY)
{
	flt2 i = this->intersectWithOxz(mousePosX, mousePosY);
	float angle = atan2(i.y(), i.x()) * 180.0f / M_PI;
	return angle;
}

float FrameOfReferenceState::projectOnYZcircle(float mousePosX, float mousePosY)
{
	flt2 i = this->intersectWithOyz(mousePosX, mousePosY);
	float angle = atan2(i.y(), i.x()) * 180.0f / M_PI;
	return angle;
}

flt2 FrameOfReferenceState::intersectWithOxy(float mousePosX, float mousePosY)
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

	flt3 axisY = state.axis1;
	axisY = axisY * v;
	axisY.normalize();
	
	flt3 i = linePlaneIntersection(pt, dir, origin, axisX, axisY);

	return flt2(i.x(), i.y());
}

flt2 FrameOfReferenceState::intersectWithOxz(float mousePosX, float mousePosY)
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

	flt3 axisZ = state.axis2;
	axisZ = axisZ * v;
	axisZ.normalize();

	flt3 i = linePlaneIntersection(pt, dir, origin, axisX, axisZ);

	return flt2(i.x(), i.y());
}

flt2 FrameOfReferenceState::intersectWithOyz(float mousePosX, float mousePosY)
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

	flt3 axisZ = state.axis2;
	axisZ = axisZ * v;
	axisZ.normalize();

	flt3 i = linePlaneIntersection(pt, dir, origin, axisY, axisZ);

	return flt2(i.x(), i.y());
}

flt4x4& FrameOfReferenceState::getDiffPosition()
{
	return dPos;
}