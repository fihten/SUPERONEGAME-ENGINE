
#include "pch.h"
#include "framework.h"

#include "Scene.h"

Scene::Node::Node(NodeID id)
{
	ID = id;
}

Scene::Node::~Node()
{
	for (auto c : childs)
		delete c;
	childs.clear();
}

void Scene::Node::addChild(Node* n)
{
	childs.push_back(n);
	n->parent = this;
}

Scene::Scene()
{
	root = new Scene::RootNode(nextId++);
	root->scene = this;

	nodes.reserve(100);
	nodes.resize(1);
	nodes[0] = root;
}

Scene::~Scene()
{
	delete root;
}

Scene::Node* Scene::Node::findNodeByID(NodeID id)
{
	if (ID == id)
		return this;
	for (auto n : childs)
	{
		Scene::Node* node = n->findNodeByID(id);
		if (node)
			return node;
	}
	return nullptr;
}

flt4x4 Scene::Node::getPos() const
{
	return parent ? parent->getPos() : flt4x4();
}

flt4x4 Scene::TransformNode::getPos() const
{
	return parent ? parent->getPos() * pos : pos;
}

void Scene::TransformNode::accept(Visitor* visitor) const
{
	visitor->startVisit(this);

	for (auto& child : childs)
		child->accept(visitor);

	visitor->finishVisit(this);
}

void Scene::MeshNode::accept(Visitor* visitor) const
{
	visitor->startVisit(this);

	for (auto& child : childs)
		child->accept(visitor);

	visitor->finishVisit(this);
}

NodeID Scene::addTransformNode(const flt4x4& pos, NodeID id)
{
	Scene::Node* node = nodes[id];
	if (node == nullptr)
		return NodeID(-1);

	id = nextId;
	Node* addedNode = new Scene::TransformNode(nextId++, pos);
	addedNode->scene = this;
	node->addChild(addedNode);

	if (nodes.size() <= id)
		nodes.resize(id + 1, nullptr);
	nodes[id] = addedNode;

	return id;
}

NodeID Scene::addMeshNode(Mesh* mesh, NodeID id)
{
	Scene::Node* node = nodes[id];
	if (node == nullptr)
		return NodeID(-1);

	id = nextId;
	Node* addedNode = new Scene::MeshNode(nextId++, mesh);
	addedNode->scene = this;
	node->addChild(addedNode);

	mesh->scene = (void*)this;
	mesh->nodeID = (int)id;

	if (nodes.size() <= id)
		nodes.resize(id + 1, nullptr);
	nodes[id] = addedNode;

	return id;
}

bool Scene::Node::getParam(const ParamKey& paramName, string_id& s) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, s);

	if (params.count(paramName) == 0)
		return false;

	s = params.at(paramName).s;
	
	return true;
}

bool Scene::Node::getParam(const ParamKey& paramName, float& f) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f);

	if (params.count(paramName) == 0)
		return false;

	f = params.at(paramName).f;

	return true;
}

bool Scene::Node::getParam(const ParamKey& paramName, flt2& f2) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f2);

	if (params.count(paramName) == 0)
		return false;

	f2 = params.at(paramName).f2;

	return true;
}

bool Scene::Node::getParam(const ParamKey& paramName, flt3& f3) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f3);

	if (params.count(paramName) == 0)
		return false;

	f3 = params.at(paramName).f3;

	return true;
}

bool Scene::Node::getParam(const ParamKey& paramName, flt4& f4) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f4);

	if (params.count(paramName) == 0)
		return false;

	f4 = params.at(paramName).f4;

	return true;
}

bool Scene::Node::getParam(const ParamKey& paramName, flt4x4& f4x4) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName, f4x4);

	if (params.count(paramName) == 0)
		return false;

	f4x4 = params.at(paramName).f4x4;

	return true;
}

void Scene::Node::setParam(const ParamKey& paramName, const string_id& s)
{
	params[paramName].s = s;
}

void Scene::Node::setParam(const ParamKey& paramName, const float& f)
{
	params[paramName].f = f;
}

void Scene::Node::setParam(const ParamKey& paramName, const flt2& f2)
{
	params[paramName].f2 = f2;
}

void Scene::Node::setParam(const ParamKey& paramName, const flt3& f3)
{
	params[paramName].f3 = f3;
}

void Scene::Node::setParam(const ParamKey& paramName, const flt4& f4)
{
	params[paramName].f4 = f4;
}

void Scene::Node::setParam(const ParamKey& paramName, const flt4x4& f4x4)
{
	params[paramName].f4x4 = f4x4;
}

void Scene::Node::accept(Visitor* visitor) const
{
	visitor->startVisit(this);

	for (auto& child : childs)
		child->accept(visitor);

	visitor->finishVisit(this);
}

void Scene::RootNode::accept(Visitor* visitor) const
{
	visitor->startVisit(this);

	for (auto& child : childs)
		child->accept(visitor);

	visitor->finishVisit(this);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const string_id& s)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, s);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const float& f)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const flt2& f2)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f2);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const flt3& f3)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f3);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const flt4& f4)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f4);
}

void Scene::setNodeParam(NodeID id, const ParamKey& paramName, const flt4x4& f4x4)
{
	Node* node = nodes[id];
	if (node == nullptr)
		return;
	node->setParam(paramName, f4x4);
}

void Scene::createNodeParamReference(NodeID reference, NodeID referenced, const ParamKey& param)
{
	paramsLocations[reference].location[param] = referenced;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, string_id& s) const
{
	if (paramsLocations.count(id) == 1)
	{
		auto& location = paramsLocations.at(id).location;
		if (location.count(paramName) == 1)
			id = location.at(paramName);
	}

	Node* node = nodes[id];
	if (node->params.count(paramName) == 0)
		return false;

	s = node->params.at(paramName).s;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, float& f) const
{
	if (paramsLocations.count(id) == 1)
	{
		auto& location = paramsLocations.at(id).location;
		if (location.count(paramName) == 1)
			id = location.at(paramName);
	}

	Node* node = nodes[id];
	if (node->params.count(paramName) == 0)
		return false;

	f = node->params.at(paramName).f;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, flt2& f2) const
{
	if (paramsLocations.count(id) == 1)
	{
		auto& location = paramsLocations.at(id).location;
		if (location.count(paramName) == 1)
			id = location.at(paramName);
	}

	Node* node = nodes[id];
	if (node->params.count(paramName) == 0)
		return false;

	f2 = node->params.at(paramName).f2;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, flt3& f3) const
{
	if (paramsLocations.count(id) == 1)
	{
		auto& location = paramsLocations.at(id).location;
		if (location.count(paramName) == 1)
			id = location.at(paramName);
	}

	Node* node = nodes[id];
	if (node->params.count(paramName) == 0)
		return false;

	auto& param = node->params.at(paramName);
	string_id s = param.s;

	if (s == node_position_id)
	{
		flt4x4 pos = node->getPos();
		f3.x() = pos.m30();
		f3.y() = pos.m31();
		f3.z() = pos.m32();
		return true;
	}
	if (s == node_axis_z_id)
	{
		flt4x4 pos = node->getPos();
		f3.x() = pos.m20();
		f3.y() = pos.m21();
		f3.z() = pos.m22();
		return true;
	}

	f3 = param.f3;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, flt4& f4) const
{
	if (paramsLocations.count(id) == 1)
	{
		auto& location = paramsLocations.at(id).location;
		if (location.count(paramName) == 1)
			id = location.at(paramName);
	}

	Node* node = nodes[id];
	if (node->params.count(paramName) == 0)
		return false;

	f4 = node->params.at(paramName).f4;

	return true;
}

bool Scene::getNodeParam(NodeID id, const ParamKey& paramName, flt4x4& f4x4) const
{
	if (paramsLocations.count(id) == 1)
	{
		auto& location = paramsLocations.at(id).location;
		if (location.count(paramName) == 1)
			id = location.at(paramName);
	}

	Node* node = nodes[id];
	if (node->params.count(paramName) == 0)
		return false;

	f4x4 = node->params.at(paramName).f4x4;

	return true;
}

flt4x4 Scene::getNodePosition(NodeID id) const
{
	Node* node = nodes[id];
	if (node == nullptr)
		return flt4x4();
	return node->getPos();
}

void Scene::accept(Visitor* visitor) const
{
	root->accept(visitor);
}