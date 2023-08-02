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
		MeshInScene(const Mesh* mesh, NodeID meshNode) :mesh(mesh) 
		{
			--instanceNumber;
			id = mesh->id;
			nodeID = meshNode;
		}

		std::string getTechnique() const;
		std::string getPass() const;
		
		const void* getStream(const std::string& name, StreamType type) const;
		const std::vector<uint32_t>* getIndicies() const;

		std::string getName() const;

		uint32_t getVerticesCount() const;
		uint32_t getIndicesCount() const;

		std::string getParam(const std::string& param) const;
		flt4x4 getPosition() const;
	};
public:
	void startVisit(const Scene::MeshNode* node);
};