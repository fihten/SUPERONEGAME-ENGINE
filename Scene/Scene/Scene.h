#pragma once
#include <list>
#include "Matrix4x4.h"
#include "Mesh.h"
#include <map>
#include <string>

typedef int NodeID;
class Scene
{
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
	};

	class RootNode : public Node
	{
	public:
		RootNode(NodeID id) :Node(id) {}
	};

	class TransformNode : public Node
	{
		flt4x4 pos;
	public:
		TransformNode(NodeID id, const flt4x4& pos) :Node(id), pos(pos) {}
		flt4x4 getPos() const;
	};

	class MeshNode : public Node
	{
		const Mesh* mesh = nullptr;
	public:
		MeshNode(NodeID id, const Mesh* mesh) :Node(id), mesh(mesh) {}
	};

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
};
