#pragma once

#include "Modifier.h"
#include "Mesh.h"
#include "BaseOfFramework.h"

class Creator :public Modifier
{
	Mesh baseOfFrameworkGeo;
protected:
	BaseOfFramework baseOfFramework;
	bool bDrawBase;

	std::vector<Mesh> meshes;

public:
	virtual void init();
	void draw();
};