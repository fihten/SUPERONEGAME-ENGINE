
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
	Scene::Node* node = root->findNodeByID(id);
	if (node == nullptr)
		return NodeID(-1);

	id = nextId;
	Node* addedNode = new Scene::TransformNode(nextId++, pos);
	addedNode->scene = this;
	node->addChild(addedNode);

	return id;
}

NodeID Scene::addMeshNode(Mesh* mesh, NodeID id)
{
	Scene::Node* node = root->findNodeByID(id);
	if (node == nullptr)
		return NodeID(-1);

	id = nextId;
	Node* addedNode = new Scene::MeshNode(nextId++, mesh);
	addedNode->scene = this;
	node->addChild(addedNode);

	mesh->scene = (void*)this;
	mesh->nodeID = (int)id;

	return id;
}

std::string Scene::Node::getParam(const std::string& paramName) const
{
	if (scene)
		return scene->getNodeParam(ID, paramName);

	std::string paramValue = "";
	if (params.count(paramName) == 1)
		paramValue = params.at(paramName);
	return paramValue;
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

std::string Scene::getNodeParam(NodeID id, const std::string& paramName) const
{
	if (paramsLocations.count(id) == 1)
	{
		auto& location = paramsLocations.at(id).location;
		if (location.count(paramName) == 1)
			id = location.at(paramName);
	}

	Node* node = root->findNodeByID(id);
	std::string paramValue = "";
	if (node->params.count(paramName) == 1)
		paramValue = node->params.at(paramName);

	if (paramValue == std::string("node_position"))
	{
		flt4x4 pos = node->getPos();
		return flt3(pos.m30(), pos.m31(), pos.m32());
	}
	if (paramValue == std::string("node_axis_z"))
	{
		flt4x4 pos = node->getPos();
		return flt3(pos.m20(), pos.m21(), pos.m22());
	}

	return paramValue;
}

flt4x4 Scene::getNodePosition(NodeID id) const
{
	Node* node = root->findNodeByID(id);
	if (node == nullptr)
		return flt4x4();
	return node->getPos();
}

void Scene::accept(Visitor* visitor) const
{
	root->accept(visitor);
}