#pragma once
#include "FrameOfReference.h"
#include "Mesh.h"
#include <list>
#include <memory>

class FrameOfReferenceStateObserver;
enum class UpdateType;

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
	void notify(UpdateType updateType);

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

	void release();

	void setFrameOfReference(const FrameOfReference& frameOfReference);
	void turnOn();
	void turnOff();

public:
	void draw();
	IntersectedAxis checkIntersection(float mousePosX, float mousePosY);
	float projectOnXaxis(float mousePosX, float mousePosY);
	float projectOnYaxis(float mousePosX, float mousePosY);
	float projectOnZaxis(float mousePosX, float mousePosY);

	flt4x4& getDiffPosition();

public:
	static auto instance()->std::shared_ptr<FrameOfReferenceState>;

private:
	std::list<FrameOfReferenceStateObserver*> observers;
	FrameOfReference state;

	Mesh framesOfReferences;

	static std::shared_ptr<FrameOfReferenceState> ptr;

	bool bTurnedOn = false;
	flt4x4 dPos;
};