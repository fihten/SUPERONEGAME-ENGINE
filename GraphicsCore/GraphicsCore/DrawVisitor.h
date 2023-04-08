#pragma once

#include "Scene.h"
#include "GraphicsCore.h"
#include "Mesh.h"

class DrawVisitor :public Scene::Visitor
{
	class MeshInScene :public Mesh
	{
		const Mesh* mesh = nullptr;
	public:
		MeshInScene(const Mesh* mesh) :mesh(mesh) {}

		std::string getParam(const std::string& param) const;
		flt4x4 getPosition() const;
	};
public:
	void startVisit(const Scene::MeshNode* node);

	GraphicsCore* graphicsCore = nullptr;
};