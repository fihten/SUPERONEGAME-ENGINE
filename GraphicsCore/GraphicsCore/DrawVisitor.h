#pragma once

#include "Scene.h"
#include "GraphicsCore.h"
#include "Mesh.h"

class DrawVisitor :public Scene::Visitor
{
	class MeshInScene :public Mesh
	{
		Mesh* mesh = nullptr;
	public:
		MeshInScene(Mesh* mesh, NodeID meshNode) :mesh(mesh) 
		{
			--instanceNumber;
			id = mesh->id;
			nodeID = meshNode;
		}

		string_id getTechnique() const;
		string_id getPass() const;
		
		const void* getStream(string_id name, StreamType type) const;
		const std::vector<uint32_t>* getIndicies() const;

		std::string getName() const;

		uint32_t getVerticesCount() const;
		uint32_t getIndicesCount() const;

		virtual bool getParam(int param_index, string_id& s) const;
		virtual bool getParam(int param_index, float& f) const;
		virtual bool getParam(int param_index, flt2& f2) const;
		virtual bool getParam(int param_index, flt3& f3) const;
		virtual bool getParam(int param_index, flt4& f4) const;
		virtual bool getParam(int param_index, flt4x4& f4x4) const;

		virtual bool getParam(const ParamKey& param, string_id& s) const;
		virtual bool getParam(const ParamKey& param, float& f) const;
		virtual bool getParam(const ParamKey& param, flt2& f2) const;
		virtual bool getParam(const ParamKey& param, flt3& f3) const;
		virtual bool getParam(const ParamKey& param, flt4& f4) const;
		virtual bool getParam(const ParamKey& param, flt4x4& f4x4) const;


		flt4x4 getPosition() const;
	};
public:
	void startVisit(const Scene::MeshNode* node);
};