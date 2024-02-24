#pragma once

#include "Modifier.h"
#include "Mesh.h"
#include "SphereFramework.h"

class SphereCreator :public Modifier
{
	Mesh sphereFrameworkGeo;
	SphereFramework sphereFramework;

public:
	SphereCreator();

	Behaviour processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam);
	void draw();
};