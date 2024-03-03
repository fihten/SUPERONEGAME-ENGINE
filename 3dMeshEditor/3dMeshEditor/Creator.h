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
public:
	virtual void init();
	void draw();
};