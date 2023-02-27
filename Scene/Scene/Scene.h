#pragma once
#include <list>

class Scene
{
	class Node
	{
		int ID = -1;
		std::list<Node*> childs;
	public:
		Node(int id);
		~Node();

		void addChild(const Node* n);
	};

	class RootNode : public Node
	{

	};

	class TransformNode : public Node
	{

	};

	class MeshNode : public Node
	{

	};
};