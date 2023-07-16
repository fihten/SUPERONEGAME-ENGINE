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
	envelopeToNode[envelopesCount] = meshNode;
	++envelopesCount;

	GraphicsCore::instance()->updateEnvelopes(envelopes, envelopesCount);
	GraphicsCore::instance()->setEnvelopesCount(envelopesCount);

	return meshNode;
}

void MainScene::setNodeParam(NodeID id, const std::string& paramName, const std::string& paramVal)
{
	scene.setNodeParam(id, paramName, paramVal);
}

void MainScene::createNodeParamReference(NodeID reference, NodeID referenced, const std::string& param)
{
	scene.createNodeParamReference(reference, referenced, param);
}

std::string MainScene::getNodeParam(NodeID id, const std::string& paramName) const
{
	return scene.getNodeParam(id, paramName);
}

flt4x4 MainScene::getNodePosition(NodeID id) const
{
	return scene.getNodePosition(id);
}

void MainScene::accept(Scene::Visitor* visitor) const
{
	scene.accept(visitor);
}