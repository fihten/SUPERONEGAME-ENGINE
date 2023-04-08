#include "DrawVisitor.h"

void DrawVisitor::startVisit(const Scene::MeshNode* node)
{
	if (graphicsCore == nullptr)
		return;

	graphicsCore->draw(MeshInScene(node->mesh));
}

std::string DrawVisitor::MeshInScene::getParam(const std::string& param) const
{
	if (mesh == nullptr)
		return std::string("");

	std::string val = mesh->getParam(param);
	if (!val.empty())
		return val;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(mesh->nodeID, param);
}

flt4x4 DrawVisitor::MeshInScene::getPosition() const
{
	if (mesh == nullptr)
		return flt4x4();

	return static_cast<Scene*>(mesh->scene)->getNodePosition(mesh->nodeID);
}