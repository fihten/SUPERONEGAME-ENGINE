#include "DrawVisitor.h"

void DrawVisitor::startVisit(const Scene::MeshNode* node)
{
	if (graphicsCore == nullptr)
		return;
	graphicsCore->draw(*node->mesh);
}