#pragma once

enum class UpdateType 
{
	Translation,
	Rotation,
	Scaling
};

class FrameOfReferenceStateObserver
{
public:
	virtual void update(UpdateType updateType) = 0;
};