#pragma once
#include <list>
#include "Matrix4x4.h"
#include "Mesh.h"
#include <map>
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
		Node* parent = nullptr;
		Scene* scene = nullptr;
		std::map<ParamKey, ParamValue> params;
	public:
		Node(NodeID id);
		virtual ~Node();

		virtual void addChild(Node* n);
		virtual Node* findNodeByID(NodeID id);
		virtual flt4x4 getPos() const;

		virtual bool getParam(const ParamKey& paramName, string_id& s) const;
		virtual bool getParam(const ParamKey& paramName, float& f) const;
		virtual bool getParam(const ParamKey& paramName, flt2& f2) const;
		virtual bool getParam(const ParamKey& paramName, flt3& f3) const;
		virtual bool getParam(const ParamKey& paramName, flt4& f4) const;
		virtual bool getParam(const ParamKey& paramName, flt4x4& f4x4) const;
		
		virtual void setParam(const ParamKey& paramName, const string_id& s);
		virtual void setParam(const ParamKey& paramName, const float& f);
		virtual void setParam(const ParamKey& paramName, const flt2& f2);
		virtual void setParam(const ParamKey& paramName, const flt3& f3);
		virtual void setParam(const ParamKey& paramName, const flt4& f4);
		virtual void setParam(const ParamKey& paramName, const flt4x4& f4x4);
		
		virtual void accept(Visitor* visitor) const;

	public:
		NodeID ID = -1;
	};

	class RootNode : public Node
	{
	public:
		RootNode(NodeID id) :Node(id) {}
		virtual void accept(Visitor* visitor) const;
	};

	class TransformNode : public Node
	{
		flt4x4 pos;
	public:
		TransformNode(NodeID id, const flt4x4& pos) :Node(id), pos(pos) {}
		flt4x4 getPos() const;
		virtual void accept(Visitor* visitor) const;
	};

	class MeshNode : public Node
	{
	public:
		MeshNode(NodeID id, const Mesh* mesh) :Node(id), mesh(mesh) {}
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
		std::map<ParamKey, NodeID> location;
	};
	std::map<NodeID, ParamsLocations> paramsLocations;

private:
	std::vector<Node*> nodes;

public:
	Scene();
	~Scene();

	NodeID addTransformNode(const flt4x4& pos, NodeID id = 0);
	NodeID addMeshNode(Mesh* mesh, NodeID id = 0);

	void setNodeParam(NodeID id, const ParamKey& paramName, const string_id& s);
	void setNodeParam(NodeID id, const ParamKey& paramName, const float& f);
	void setNodeParam(NodeID id, const ParamKey& paramName, const flt2& f2);
	void setNodeParam(NodeID id, const ParamKey& paramName, const flt3& f3);
	void setNodeParam(NodeID id, const ParamKey& paramName, const flt4& f4);
	void setNodeParam(NodeID id, const ParamKey& paramName, const flt4x4& f4x4);

	void createNodeParamReference(NodeID reference, NodeID referenced, const ParamKey& param);

	bool getNodeParam(NodeID id, const ParamKey& paramName, string_id& s) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, float& f) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, flt2& f2) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, flt3& f3) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, flt4& f4) const;
	bool getNodeParam(NodeID id, const ParamKey& paramName, flt4x4& f4x4) const;

	flt4x4 getNodePosition(NodeID id) const;
	void accept(Visitor* visitor) const;
};
