#pragma once

#include "Creator.h"
#include "Mesh.h"
#include "ConeFramework.h"

class ConeCreator :public Creator
{
	Mesh coneFrameworkGeo;
	ConeFramework coneFramework;

	flt3 axis0;
	flt3 axis1;
	flt3 axis2;

	enum class State :uint32_t
	{
		Initial,
		DeterminingOfOxyProjectionOfBottom,
		DeterminingOfHeight,
		DeterminingOfOxyProjectionOfTop,
		Insertion
	};
	State currentState = State::Initial;
	bool bInitialized = false;

public:
	Behaviour processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam);
	void draw();
	uint32_t getIcon();
	void init();
};