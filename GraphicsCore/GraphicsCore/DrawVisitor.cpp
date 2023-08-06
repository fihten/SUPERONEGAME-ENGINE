#include "DrawVisitor.h"

void DrawVisitor::startVisit(const Scene::MeshNode* node)
{
	GraphicsCore::instance()->draw(MeshInScene(node->mesh, node->ID));
}

string_id DrawVisitor::MeshInScene::getTechnique() const
{
	if (mesh == nullptr)
		return string_id(-1);

	return mesh->getTechnique();
}

string_id DrawVisitor::MeshInScene::getPass() const
{
	if (mesh == nullptr)
		return string_id(-1);

	return mesh->getPass();
}

const void* DrawVisitor::MeshInScene::getStream(string_id name, StreamType type) const
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

bool DrawVisitor::MeshInScene::getParam(const ParamKey& param, string_id& s) const
{
	s = string_id(-1);
	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param, s))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param, s);
}

bool DrawVisitor::MeshInScene::getParam(const ParamKey& param, float& f) const
{
	f = 0;

	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param, f))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param, f);
}

bool DrawVisitor::MeshInScene::getParam(const ParamKey& param, flt2& f2) const
{
	f2 = flt2();

	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param, f2))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param, f2);
}

bool DrawVisitor::MeshInScene::getParam(const ParamKey& param, flt3& f3) const
{
	f3 = flt3();

	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param, f3))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param, f3);
}

bool DrawVisitor::MeshInScene::getParam(const ParamKey& param, flt4& f4) const
{
	f4 = flt4();

	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param, f4))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param, f4);
}

bool DrawVisitor::MeshInScene::getParam(const ParamKey& param, flt4x4& f4x4) const
{
	f4x4 = flt4x4();

	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param, f4x4))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param, f4x4);
}

flt4x4 DrawVisitor::MeshInScene::getPosition() const
{
	if (mesh == nullptr)
		return flt4x4();

	return static_cast<Scene*>(mesh->scene)->getNodePosition(nodeID);
}