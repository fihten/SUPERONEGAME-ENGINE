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
		NodeID ID = -1;
		std::list<Node*> childs;
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
		virtual void accept(Visitor* visitor) const;
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
		const Mesh* mesh = nullptr;
	public:
		MeshNode(NodeID id, const Mesh* mesh) :Node(id), mesh(mesh) {}
		virtual void accept(Visitor* visitor) const;
	};

	class Visitor
	{
	public:
		virtual void startVisit(const Node* node) = 0;
		virtual void startVisit(const RootNode* node) = 0;
		virtual void startVisit(const TransformNode* node) = 0;
		virtual void startVisit(const MeshNode* node) = 0;

		virtual void finishVisit(const Node* node) = 0;
		virtual void finishVisit(const RootNode* node) = 0;
		virtual void finishVisit(const TransformNode* node) = 0;
		virtual void finishVisit(const MeshNode* node) = 0;
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

public:
	Scene();
	~Scene();

	NodeID addTransformNode(const flt4x4& pos, NodeID id);
	NodeID addMeshNode(Mesh* mesh, NodeID id);

	std::string getNodeParam(NodeID id, const std::string& paramName) const;
	void accept(Visitor* visitor) const;
};
