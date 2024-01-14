#pragma once

enum class UpdateType 
{
	Translation,
	Rotation,
	Scaling,
	SetFrameOfReference
};

class FrameOfReferenceStateObserver
{
public:
	virtual void update(UpdateType updateType) = 0;
};