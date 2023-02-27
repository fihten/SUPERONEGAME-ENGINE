// Scene.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

#include "Scene.h"

Scene::Node::Node(int id)
{
	ID = id;
}

Scene::Node::~Node()
{
	for (auto c : childs)
		delete c;
	childs.clear();
}
