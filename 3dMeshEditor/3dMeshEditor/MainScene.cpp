#include "MainScene.h"
#include "GraphicsCore.h"

MainScene* MainScene::pMainScene = nullptr;

MainScene* MainScene::instance()
{
	if (pMainScene == nullptr)
		pMainScene = new MainScene();
	return pMainScene;
}

NodeID MainScene::addTransformNode(const flt4x4& pos, NodeID id = 0)
{
	return scene.addTransformNode(pos, id);
}

NodeID MainScene::addMeshNode(Mesh* mesh, NodeID id = 0)
{
	NodeID meshNode = scene.addMeshNode(mesh, id);

	
	envelopes[envelopesCount].min = mesh->getBottomBorder();
	envelopes[envelopesCount].max = mesh->getTopBorder();
	envelopes[envelopesCount].transform = scene.getNodePosition(meshNode);
	++envelopesCount;

	GraphicsCore::instance()->updateEnvelopes(envelopes, envelopesCount);

	return meshNode;
}