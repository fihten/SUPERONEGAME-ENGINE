#pragma once

#include "Creator.h"
#include "Mesh.h"
#include "SphereFramework.h"

class SphereCreator :public Creator
{
	Mesh sphereFrameworkGeo;
	SphereFramework sphereFramework;

	flt3 axis0;
	flt3 axis1;
	flt3 axis2;

	enum class State :uint32_t
	{
		Initial,
		DeterminingOfOxyProjection,
		DeterminingOfHeight,
		Insertion
	};
	State currentState = State::Initial;
	bool bInitialized = false;

public:
	Behaviour processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam);
	void draw();
	void init();
};