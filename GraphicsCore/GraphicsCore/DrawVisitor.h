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
		string_id getBlendState() const;
		string_id getDepthStencilState() const;

		const void* getStream(string_id name, StreamType type) const;
		const std::vector<uint32_t>* getIndicies() const;

		std::string getName() const;

		uint32_t getVerticesCount() const;
		uint32_t getIndicesCount() const;

		virtual string_id* getStringID(int param_index);
		virtual float* getFloat(int param_index);
		virtual flt2* getFlt2(int param_index);
		virtual flt3* getFlt3(int param_index);
		virtual flt4* getFlt4(int param_index);
		virtual flt4x4* getFlt4x4(int param_index);
		virtual int* getInt(int param_index);
		virtual Vec2d<int>* getInt2(int param_index);

		virtual bool getParam(int param_index, string_id& s) const;
		virtual bool getParam(int param_index, float& f) const;
		virtual bool getParam(int param_index, flt2& f2) const;
		virtual bool getParam(int param_index, flt3& f3) const;
		virtual bool getParam(int param_index, flt4& f4) const;
		virtual bool getParam(int param_index, flt4x4& f4x4) const;
		virtual bool getParam(int param_index, int& i) const;
		virtual bool getParam(int param_index, Vec2d<int>& i2) const;

		virtual bool getParam(const ParamKey& param, string_id& s) const;
		virtual bool getParam(const ParamKey& param, float& f) const;
		virtual bool getParam(const ParamKey& param, flt2& f2) const;
		virtual bool getParam(const ParamKey& param, flt3& f3) const;
		virtual bool getParam(const ParamKey& param, flt4& f4) const;
		virtual bool getParam(const ParamKey& param, flt4x4& f4x4) const;
		virtual bool getParam(const ParamKey& param, int& i) const;
		virtual bool getParam(const ParamKey& param, Vec2d<int>& i2) const;

		flt4x4& getPos4x4();
		flt3& getPos3();
		flt4& getPos4();
		flt3& getAxisZ();
		flt4x4& getPosInvTr();
	};
public:
	void startVisit(const Scene::MeshNode* node);
};