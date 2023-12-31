#pragma once
#include "FrameOfReferenceStateObserver.h"
#include "FrameOfReference.h"
#include <list>

class FrameOfReferenceState
{
public:
	void attach(FrameOfReferenceStateObserver* observer);
	void detach(const FrameOfReferenceStateObserver* observer);
	void notify();
private:
	std::list<FrameOfReferenceStateObserver*> observers;
	FrameOfReference state;
};