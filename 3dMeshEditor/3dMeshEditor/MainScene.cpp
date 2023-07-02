#include "MainScene.h"

Scene* MainScene::pScene = nullptr;

Scene* MainScene::instance()
{
	if (pScene == nullptr)
		pScene = new Scene();
	return pScene;
}