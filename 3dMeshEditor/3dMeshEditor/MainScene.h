#pragma once

#include "Scene.h"
#include "Envelope.h"

class MainScene
{
	MainScene() {}
public:
	static MainScene* instance();

private:
	Scene scene;
	static MainScene* pMainScene;
	Envelope envelopes[MaxEnvelopesCount];
};