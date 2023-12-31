#include "FrameOfReferenceState.h"
#include <algorithm>

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