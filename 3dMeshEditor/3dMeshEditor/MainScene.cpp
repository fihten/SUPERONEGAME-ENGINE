#include "MainScene.h"
#include "GraphicsCore.h"

MainScene* MainScene::pMainScene = nullptr;

MainScene* MainScene::instance()
{
	if (pMainScene == nullptr)
		pMainScene = new MainScene();
	return pMainScene;
}

NodeID MainScene::addTransformNode(const flt4x4& pos, NodeID id)
{
	return scene.addTransformNode(pos, id);
}

NodeID MainScene::addMeshNode(Mesh* mesh, NodeID id)
{
	NodeID meshNode = scene.addMeshNode(mesh, id);

	auto& selectedObjectBox = selectedObjectsBoxes[spheresCount];

	flt3 min = mesh->getBottomBorder();
	flt3 max = mesh->getTopBorder();

	flt4 posL(0.5f * (min + max), 1.0f);
	
	flt4x4 pos = scene.getNodePosition(id);
	selectedObjectBox.posW = (posL * pos).xyz();

	selectedObjectBox.axis0 = 0.5f * (max.x() - min.x()) * flt3(1.1f, 0, 0);
	selectedObjectBox.axis0 = selectedObjectBox.axis0 * pos;

	selectedObjectBox.axis1 = 0.5f * (max.y() - min.y()) * flt3(0, 1.1f, 0);
	selectedObjectBox.axis1 = selectedObjectBox.axis1 * pos;

	selectedObjectBox.axis2 = 0.5f * (max.z() - min.z()) * flt3(0, 0, 1.1f);
	selectedObjectBox.axis2 = selectedObjectBox.axis2 * pos;

	selectedObjectBox.color = flt3(1, 0, 0);
	selectedObjectBox.size = 0.5f;

	mesh->getBoundingSphere(boundingSpheres[spheresCount], pos);
	boundingSphereToNode[spheresCount] = meshNode;
	++spheresCount;

	GraphicsCore::instance()->updateBoundingSpheres(boundingSpheres);
	GraphicsCore::instance()->setSpheresCount(spheresCount);

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