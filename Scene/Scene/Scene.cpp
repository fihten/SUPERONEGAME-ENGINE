
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

NodeID Scene::addTransformNode(const flt4x4& pos, NodeID id)
{
	Scene::Node* node = root->findNodeByID(id);
	if (node == nullptr)
		return NodeID(-1);

	id = nextId;
	node->addChild(new Scene::TransformNode(nextId++, pos));

	return id;
}

NodeID Scene::addMeshNode(Mesh* mesh, NodeID id)
{
	Scene::Node* node = root->findNodeByID(id);
	if (node == nullptr)
		return NodeID(-1);

	id = nextId;
	node->addChild(new Scene::MeshNode(nextId++, mesh));

	mesh->scene = (void*)this;
	mesh->nodeID = (int)id;

	return id;
}