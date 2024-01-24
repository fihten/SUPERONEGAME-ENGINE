#pragma once
#include "FrameOfReferenceState.h"

enum class UpdateType 
{
	Translation,
	Rotation,
	Scaling,
	SetFrameOfReference,
	Release
};

class FrameOfReferenceStateObserver
{
public:
	FrameOfReferenceStateObserver() 
	{ 
		FrameOfReferenceState::instance()->attach(this); 
	}

	virtual void update(UpdateType updateType) {};
};