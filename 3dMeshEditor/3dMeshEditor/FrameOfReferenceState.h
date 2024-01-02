#pragma once
#include "FrameOfReferenceStateObserver.h"
#include "FrameOfReference.h"
#include "Mesh.h"
#include <list>
#include <memory>

enum class IntersectedAxis :uint32_t
{
	NONE,
	AXIS_X,
	AXIS_Y,
	AXIS_Z
};

class FrameOfReferenceState
{
	FrameOfReferenceState();
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

	void setFrameOfReference(const FrameOfReference& frameOfReference);
	void turnOn();
	void turnOff();

public:
	void draw();
	IntersectedAxis checkIntersection();

public:
	static auto instance()->std::shared_ptr<FrameOfReferenceState>;

private:
	std::list<FrameOfReferenceStateObserver*> observers;
	FrameOfReference state;

	Mesh framesOfReferences;

	static std::shared_ptr<FrameOfReferenceState> ptr;

	bool bTurnedOn = false;
};