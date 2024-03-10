#pragma once

#include "Modifier.h"
#include "Mesh.h"
#include "BaseOfFramework.h"
#include "resource.h"

class Creator :public Modifier
{
	Mesh baseOfFrameworkGeo;
protected:
	BaseOfFramework baseOfFramework;
	bool bDrawBase;

	std::vector<Mesh> meshes;

public:
	virtual void init();
	virtual uint32_t getIcon();
	void draw();
};