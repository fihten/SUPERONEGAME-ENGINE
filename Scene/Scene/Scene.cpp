
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

void Scene::Node::addChild(const Scene::Node* n)
{
	childs.push_back(n);
}