#pragma once
#include "FrameOfReference.h"
#include "SphericFrameOfReference.h"
#include "ScaleFrameOfReference.h"
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

enum class IntersectedCircleAxis :uint32_t
{
	NONE,
	PLAIN_XY,
	PLAIN_XZ,
	PLAIN_YZ
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
	void drawSpheric();
	void drawScale();

	IntersectedAxis checkIntersection(float mousePosX, float mousePosY);
	IntersectedCircleAxis checkCircleIntersection(float mousePosX, float mousePosY);
	IntersectedAxis checkHandleIntersection(float mousePosX, float mousePosY);

	float projectOnXaxis(float mousePosX, float mousePosY);
	float projectOnYaxis(float mousePosX, float mousePosY);
	float projectOnZaxis(float mousePosX, float mousePosY);

	float projectOnXYcircle(float mousePosX, float mousePosY);
	float projectOnXZcircle(float mousePosX, float mousePosY);
	float projectOnYZcircle(float mousePosX, float mousePosY);

	flt2 intersectWithOxy(float mousePosX, float mousePosY);
	flt2 intersectWithOxz(float mousePosX, float mousePosY);
	flt2 intersectWithOyz(float mousePosX, float mousePosY);

	flt4x4& getDiffPosition();

public:
	static auto instance()->std::shared_ptr<FrameOfReferenceState>;

private:
	std::list<FrameOfReferenceStateObserver*> observers;
	FrameOfReference state;
	SphericFrameOfReference spheric_state;
	ScaleFrameOfReference scale_state;

	Mesh framesOfReferences;
	Mesh sphericFramesOfReferences;
	Mesh scaleFramesOfReferences;

	static std::shared_ptr<FrameOfReferenceState> ptr;

	bool bTurnedOn = false;
	flt4x4 dPos;
};