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