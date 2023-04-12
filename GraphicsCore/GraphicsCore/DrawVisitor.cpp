#include "DrawVisitor.h"

void DrawVisitor::startVisit(const Scene::MeshNode* node)
{
	if (graphicsCore == nullptr)
		return;

	graphicsCore->draw(MeshInScene(node->mesh));
}

std::string DrawVisitor::MeshInScene::getTechnique() const
{
	if (mesh == nullptr)
		return std::string("");

	return mesh->getTechnique();
}

std::string DrawVisitor::MeshInScene::getPass() const
{
	if (mesh == nullptr)
		return std::string("");

	return mesh->getPass();
}

const void* DrawVisitor::MeshInScene::getStream(const std::string& name, StreamType type) const
{
	if (mesh == nullptr)
		return nullptr;

	return mesh->getStream(name, type);
}

const std::vector<uint32_t>* DrawVisitor::MeshInScene::getIndicies() const
{
	if (mesh == nullptr)
		return nullptr;

	return mesh->getIndicies();
}

std::string DrawVisitor::MeshInScene::getName() const
{
	if (mesh == nullptr)
		return std::string("");

	return mesh->getName();
}

uint32_t DrawVisitor::MeshInScene::getVerticesCount() const
{
	if (mesh == nullptr)
		return 0;

	return mesh->getVerticesCount();
}

uint32_t DrawVisitor::MeshInScene::getIndicesCount() const
{
	if (mesh == nullptr)
		return 0;

	return mesh->getIndicesCount();
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