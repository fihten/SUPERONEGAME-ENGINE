#pragma once

#include "Scene.h"
#include "GraphicsCore.h"

class DrawVisitor :public Scene::Visitor
{
public:
	void startVisit(const Scene::MeshNode* node);

	GraphicsCore* graphicsCore = nullptr;
};