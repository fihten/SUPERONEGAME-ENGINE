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
		std::list<const Scene::Node*> childs;
	public:
		Node(NodeID id);
		virtual ~Node();

		void addChild(const Scene::Node* n);
	};

	class RootNode : public Node
	{

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
	
};
