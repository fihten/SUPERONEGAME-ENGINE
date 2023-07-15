#pragma once

#include "Scene.h"
#include "Envelope.h"

class MainScene
{
	MainScene() {}
public:
	static MainScene* instance();
	NodeID addTransformNode(const flt4x4& pos, NodeID id = 0);
	NodeID addMeshNode(Mesh* mesh, NodeID id = 0);

private:
	Scene scene;
	static MainScene* pMainScene;
	Envelope envelopes[MaxEnvelopesCount];
	uint32_t envelopesCount = 0;
};