#pragma once

#include "Scene.h"

class MainScene
{
	MainScene() {}
public:
	static Scene* instance();

private:
	static Scene* pScene;
};