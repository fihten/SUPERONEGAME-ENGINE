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

public:
	void moveAlongAxisX(float x);
	void moveAlongAxisY(float y);
	void moveAlongAxisZ(float z);

	void rotateAlongAxisX(float x);
	void rotateAlongAxisY(float y);
	void rotateAlongAxisZ(float z);

	void scaleAlongAxisX(float x);
	void scaleAlongAxisY(float y);
	void scaleAlongAxisZ(float z);

private:
	std::list<FrameOfReferenceStateObserver*> observers;
	FrameOfReference state;
};