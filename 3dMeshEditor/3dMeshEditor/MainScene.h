#pragma once

#include "Scene.h"
#include "Envelope.h"

class Selector;
class MainScene
{
	MainScene() {}
	friend Selector;
public:
	static MainScene* instance();
	NodeID addTransformNode(const flt4x4& pos, NodeID id = 0);
	NodeID addMeshNode(Mesh* mesh, NodeID id = 0);
	void setNodeParam(NodeID id, const std::string& paramName, const std::string& paramVal);
	void createNodeParamReference(NodeID reference, NodeID referenced, const std::string& param);
	std::string getNodeParam(NodeID id, const std::string& paramName) const;
	flt4x4 getNodePosition(NodeID id) const;
	void accept(Scene::Visitor* visitor) const;

private:
	Scene scene;
	static MainScene* pMainScene;
	Envelope envelopes[MaxEnvelopesCount];
	NodeID envelopeToNode[MaxEnvelopesCount];
	uint32_t envelopesCount = 0;
};