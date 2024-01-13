#pragma once
#include "Matrix4x4.h"
#include "Mesh.h"
#include <vector>
#include <string>

typedef int NodeID;
class Scene
{
public:
	class Visitor;
	class Node
	{
		std::vector<Node*> childs;
		friend Scene;
	protected:
		enum class type :uint32_t
		{
			ROOT,
			TRANSFORM,
			MESH
		};
		type nodeType;

		Node* parent = nullptr;
		Scene* scene = nullptr;

		std::vector<std::pair<ParamKey, ParamValue>> params;

		flt4x4 cachedPosition4x4;
		flt3 cachedPosition3;
		flt4 cachedPosition4;
		flt3 cachedAxisZ;
		flt4x4 cachedInverseTransposePosition;
		bool bCached;

	public:
		Node(NodeID id, type nodeType);
		virtual ~Node();

		virtual void addChild(Node* n);
		virtual Node* findNodeByID(NodeID id);
		
		virtual flt4x4& getPos4x4();
		virtual flt3& getPos3();
		virtual flt4& getPos4();
		virtual flt3& getAxisZ();
		virtual flt4x4& getPosInvTr();

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

		virtual bool getParam(const ParamKey& paramName, string_id& s) const;
		virtual bool getParam(const ParamKey& paramName, float& f) const;
		virtual bool getParam(const ParamKey& paramName, flt2& f2) const;
		virtual bool getParam(const ParamKey& paramName, flt3& f3) const;
		virtual bool getParam(const ParamKey& paramName, flt4& f4) const;
		virtual bool getParam(const ParamKey& paramName, flt4x4& f4x4) const;
		virtual bool getParam(const ParamKey& paramName, int& i) const;
		virtual bool getParam(const ParamKey& paramName, Vec2d<int>& i2) const;

		virtual void setParam(const ParamKey& paramName, const string_id& s);
		virtual void setParam(const ParamKey& paramName, const float& f);
		virtual void setParam(const ParamKey& paramName, const flt2& f2);
		virtual void setParam(const ParamKey& paramName, const flt3& f3);
		virtual void setParam(const ParamKey& paramName, const flt4& f4);
		virtual void setParam(const ParamKey& paramName, const flt4x4& f4x4);
		virtual void setParam(const ParamKey& paramName, const int& i);
		virtual void setParam(const ParamKey& paramName, const Vec2d<int>& i2);

		virtual void accept(Visitor* visitor) const;

	public:
		NodeID ID = -1;
	};

	class RootNode : public Node
	{
	public:
		RootNode(NodeID id) :Node(id, type::ROOT) {}
		virtual void accept(Visitor* visitor) const;
	};

	class TransformNode : public Node
	{
		friend Scene;
		flt4x4 pos;
	public:
		TransformNode(NodeID id, const flt4x4& pos) :Node(id, type::TRANSFORM), pos(pos) {}
		
		virtual flt4x4& getPos4x4();
		virtual flt3& getPos3();
		virtual flt4& getPos4();
		virtual flt3& getAxisZ();
		virtual flt4x4& getPosInvTr();

		virtual void accept(Visitor* visitor) const;
	};

	class MeshNode : public Node
	{
	public:
		MeshNode(NodeID id, const Mesh* mesh) :Node(id, type::MESH), mesh(mesh) {}
		virtual void accept(Visitor* visitor) const;

		const Mesh* mesh = nullptr;
	};

	class Visitor
	{
	public:
		virtual void startVisit(const Node* node) {}
		virtual void startVisit(const RootNode* node) {}
		virtual void startVisit(const TransformNode* node) {}
		virtual void startVisit(const MeshNode* node) {}

		virtual void finishVisit(const Node* node) {}
		virtual void finishVisit(const RootNode* node) {}
		virtual void finishVisit(const TransformNode* node) {}
		virtual void finishVisit(const MeshNode* node) {}
	};

private:
	NodeID nextId = 0;
	RootNode* root = nullptr;

private:
	struct ParamsLocations
	{
		std::vector<std::pair<ParamKey, NodeID>> location;
	};
	std::vector<ParamsLocations> paramsLocations;

private:
	std::vector<Node*> nodes;

public:
	Scene();
	~Scene();

	NodeID addTransformNode(const flt4x4& pos, NodeID id = 0);
	NodeID addMeshNode(Mesh* mesh, NodeID id = 0);

	string_id* getStringID(NodeID id, int param_index);
	float* getFloat(NodeID id, int param_index);
	flt2* getFlt2(NodeID id, int param_index);
	flt3* getFlt3(NodeID id, int param_index);
	flt4* getFlt4(NodeID id, int param_index);
	flt4x4* getFlt4x4(NodeID id, int param_index);
	int* getInt(NodeID id, int param_index);
	Vec2d<int>* getInt2(NodeID id, int param_index);

	bool getNodeParam(NodeID id, int param_index, string_id& s) const;
	bool getNodeParam(NodeID id, int param_index, float& f) const;
	bool getNodeParam(NodeID id, int param_index, flt2& f2) const;
	bool getNodeParam(NodeID id, int param_index, flt3& f3) const;
	bool getNodeParam(NodeID id, int param_index, flt4& f4) const;
	bool getNodeParam(NodeID id, int param_index, flt4x4& f4x4) const;
	bool getNodeParam(NodeID id, int param_index, int& i) const;
	bool getNodeParam(NodeID id, int param_index, Vec2d<int>& i2) const;

	void setNodeParam(NodeID id, const ParamKey& paramName, const string_id& s);
	void setNodeParam(NodeID id, const ParamKey& paramName, const float& f);
	void setNodeParam(NodeID id, const ParamKey& paramName, const flt2& f2);
	void setNodeParam(NodeID id, const ParamKey& paramName, const flt3& f3);
	void setNodeParam(NodeID id, const ParamKey& paramName, const flt4& f4);
	void setNodeParam(NodeID id, const ParamKey& paramName, const flt4x4& f4x4);
	void setNodeParam(NodeID id, const ParamKey& paramName, const int& i);
	void setNodeParam(NodeID id, const ParamKey& paramName, const Vec2d<int>& i2);

	void createNodeParamReference(NodeID reference, NodeID referenced, const ParamKey& param);

	bool getNodeParam(NodeID id, const ParamKey& paramName, string_id& s) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, float& f) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, flt2& f2) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, flt3& f3) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, flt4& f4) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, flt4x4& f4x4) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, int& i) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, Vec2d<int>& i2) const;

	flt4x4& getNodePosition4x4(NodeID id);
	flt4& getNodePosition4(NodeID id);
	flt3& getNodePosition3(NodeID id);
	flt3& getNodeAxisZ(NodeID id);
	flt4x4& getNodeInvTrPosition(NodeID id);

	void accept(Visitor* visitor) const;
	void* getNode(NodeID id);

	void updateTransformWithTranslation(NodeID id, flt4x4& translation);
};
