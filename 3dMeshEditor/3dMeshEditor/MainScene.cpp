#include "MainScene.h"

MainScene* MainScene::pMainScene = nullptr;

MainScene* MainScene::instance()
{
	if (pMainScene == nullptr)
		pMainScene = new MainScene();
	return pMainScene;
}