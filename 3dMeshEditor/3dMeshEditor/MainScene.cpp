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
	
	flt4x4& pos = scene.getNodePosition4x4(id);
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

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const string_id& s)
{
	scene.setNodeParam(id, paramName, s);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const float& f)
{
	scene.setNodeParam(id, paramName, f);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const flt2& f2)
{
	scene.setNodeParam(id, paramName, f2);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const flt3& f3)
{
	scene.setNodeParam(id, paramName, f3);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const flt4& f4)
{
	scene.setNodeParam(id, paramName, f4);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const flt4x4& f4x4)
{
	scene.setNodeParam(id, paramName, f4x4);
}

void MainScene::createNodeParamReference(NodeID reference, NodeID referenced, const ParamKey& param)
{
	scene.createNodeParamReference(reference, referenced, param);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, string_id& s) const
{
	return scene.getNodeParam(id, paramName, s);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, float& f) const
{
	return scene.getNodeParam(id, paramName, f);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, flt2& f2) const
{
	return scene.getNodeParam(id, paramName, f2);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, flt3& f3) const
{
	return scene.getNodeParam(id, paramName, f3);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, flt4& f4) const
{
	return scene.getNodeParam(id, paramName, f4);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, flt4x4& f4x4) const
{
	return scene.getNodeParam(id, paramName, f4x4);
}

flt4x4& MainScene::getNodePosition(NodeID id)
{
	return scene.getNodePosition4x4(id);
}

void MainScene::accept(Scene::Visitor* visitor) const
{
	scene.accept(visitor);
}