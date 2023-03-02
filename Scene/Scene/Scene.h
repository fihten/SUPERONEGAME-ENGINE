#pragma once
#include <list>
#include "Matrix4x4.h"
#include "Mesh.h"

typedef int NodeID;
class Scene
{
	class Node
	{
		NodeID ID = -1;
		Node* parent = nullptr;
		std::list<Node*> childs;
	public:
		Node(NodeID id);
		virtual ~Node();

		virtual void addChild(Node* n);
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
	};

	class MeshNode : public Node
	{
		const Mesh* mesh = nullptr;
	public:
		MeshNode(NodeID id, const Mesh* mesh) :Node(id), mesh(mesh) {}
	};

	NodeID nextId = 0;
	RootNode* root = nullptr;

public:
	Scene();
	~Scene();
};
