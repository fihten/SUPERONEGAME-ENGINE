#include "DrawVisitor.h"

void DrawVisitor::startVisit(const Scene::MeshNode* node)
{
	MeshInScene m(const_cast<Mesh*>(node->mesh), node->ID);
	GraphicsCore::instance()->draw(m);
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

string_id DrawVisitor::MeshInScene::getBlendState() const
{
	if (mesh == nullptr)
		return string_id(-1);

	return mesh->getBlendState();
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

string_id* DrawVisitor::MeshInScene::getStringID(int param_index)
{
	if (mesh == nullptr)
		return nullptr;

	string_id* s = mesh->getStringID(param_index);
	if (s)
		return s;

	s = static_cast<Scene*>(mesh->scene)->getStringID(nodeID, param_index);

	return s;
}

float* DrawVisitor::MeshInScene::getFloat(int param_index)
{
	if (mesh == nullptr)
		return nullptr;

	float* f = mesh->getFloat(param_index);
	if (f)
		return f;

	f = static_cast<Scene*>(mesh->scene)->getFloat(nodeID, param_index);

	return f;
}

flt2* DrawVisitor::MeshInScene::getFlt2(int param_index)
{
	if (mesh == nullptr)
		return nullptr;

	flt2* f2 = mesh->getFlt2(param_index);
	if (f2)
		return f2;

	f2 = static_cast<Scene*>(mesh->scene)->getFlt2(nodeID, param_index);

	return f2;
}

flt3* DrawVisitor::MeshInScene::getFlt3(int param_index)
{
	if (mesh == nullptr)
		return nullptr;

	flt3* f3 = mesh->getFlt3(param_index);
	if (f3)
		return f3;

	f3 = static_cast<Scene*>(mesh->scene)->getFlt3(nodeID, param_index);

	return f3;
}

flt4* DrawVisitor::MeshInScene::getFlt4(int param_index)
{
	if (mesh == nullptr)
		return nullptr;

	flt4* f4 = mesh->getFlt4(param_index);
	if (f4)
		return f4;

	f4 = static_cast<Scene*>(mesh->scene)->getFlt4(nodeID, param_index);

	return f4;
}

flt4x4* DrawVisitor::MeshInScene::getFlt4x4(int param_index)
{
	if (mesh == nullptr)
		return nullptr;

	flt4x4* f4x4 = mesh->getFlt4x4(param_index);
	if (f4x4)
		return f4x4;

	f4x4 = static_cast<Scene*>(mesh->scene)->getFlt4x4(nodeID, param_index);

	return f4x4;
}

bool DrawVisitor::MeshInScene::getParam(int param_index, string_id& s) const
{
	s = string_id(-1);
	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param_index, s))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param_index, s);
}

bool DrawVisitor::MeshInScene::getParam(int param_index, float& f) const
{
	f = 0;
	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param_index, f))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param_index, f);
}

bool DrawVisitor::MeshInScene::getParam(int param_index, flt2& f2) const
{
	f2 = flt2();
	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param_index, f2))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param_index, f2);
}

bool DrawVisitor::MeshInScene::getParam(int param_index, flt3& f3) const
{
	f3 = flt3();
	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param_index, f3))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param_index, f3);
}

bool DrawVisitor::MeshInScene::getParam(int param_index, flt4& f4) const
{
	f4 = flt4();
	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param_index, f4))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param_index, f4);
}

bool DrawVisitor::MeshInScene::getParam(int param_index, flt4x4& f4x4) const
{
	f4x4 = flt4x4();
	if (mesh == nullptr)
		return false;

	if (mesh->getParam(param_index, f4x4))
		return true;

	return static_cast<Scene*>(mesh->scene)->getNodeParam(nodeID, param_index, f4x4);
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

flt4x4& DrawVisitor::MeshInScene::getPos4x4()
{
	static flt4x4 pos;
	if (mesh == nullptr)
		return pos;

	return static_cast<Scene*>(mesh->scene)->getNodePosition4x4(nodeID);
}

flt3& DrawVisitor::MeshInScene::getPos3()
{
	static flt3 pos;
	if (mesh == nullptr)
		return pos;

	return static_cast<Scene*>(mesh->scene)->getNodePosition3(nodeID);
}

flt4& DrawVisitor::MeshInScene::getPos4()
{
	static flt4 pos;
	if (mesh == nullptr)
		return pos;

	return static_cast<Scene*>(mesh->scene)->getNodePosition4(nodeID);
}

flt3& DrawVisitor::MeshInScene::getAxisZ()
{
	static flt3 axis;
	if (mesh == nullptr)
		return axis;

	return static_cast<Scene*>(mesh->scene)->getNodeAxisZ(nodeID);
}

flt4x4& DrawVisitor::MeshInScene::getPosInvTr()
{
	static flt4x4 posInvTr;
	if (mesh == nullptr)
		return posInvTr;

	return static_cast<Scene*>(mesh->scene)->getNodeInvTrPosition(nodeID);
}