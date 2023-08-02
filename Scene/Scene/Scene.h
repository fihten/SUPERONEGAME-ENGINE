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
		std::map<std::string, std::string> params;
	public:
		Node(NodeID id);
		virtual ~Node();

		virtual void addChild(Node* n);
		virtual Node* findNodeByID(NodeID id);
		virtual flt4x4 getPos() const;
		virtual std::string getParam(const std::string& paramName) const;
		virtual void setParam(const std::string& paramName, const std::string& paramVal);
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
		std::map<std::string, NodeID> location;
	};
	std::map<NodeID, ParamsLocations> paramsLocations;

private:
	std::vector<Node*> nodes;

public:
	Scene();
	~Scene();

	NodeID addTransformNode(const flt4x4& pos, NodeID id = 0);
	NodeID addMeshNode(Mesh* mesh, NodeID id = 0);

	void setNodeParam(NodeID id, const std::string& paramName, const std::string& paramVal);
	void createNodeParamReference(NodeID reference, NodeID referenced, const std::string& param);

	std::string getNodeParam(NodeID id, const std::string& paramName) const;
	flt4x4 getNodePosition(NodeID id) const;
	void accept(Visitor* visitor) const;
};
