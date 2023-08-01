#pragma once

#include "Scene.h"
#include "SelectedObjectBox.h"
#include "GraphicsCore.h"

class MainScene
{
	MainScene() {}
public:
	static MainScene* instance();
	NodeID addTransformNode(const flt4x4& pos, NodeID id = 0);
	NodeID addMeshNode(Mesh* mesh, NodeID id = 0);
	void setNodeParam(NodeID id, const std::string& paramName, const std::string& paramVal);
	void createNodeParamReference(NodeID reference, NodeID referenced, const std::string& param);
	std::string getNodeParam(NodeID id, const std::string& paramName) const;
	flt4x4 getNodePosition(NodeID id) const;
	void accept(Scene::Visitor* visitor) const;

	SelectedObjectBox selectedObjectsBoxes[MAX_BOUNDING_SPHERES_COUNT];
	flt4 boundingSpheres[MAX_BOUNDING_SPHERES_COUNT];
	NodeID boundingSphereToNode[MAX_BOUNDING_SPHERES_COUNT];
	uint32_t spheresCount = 0;

private:
	Scene scene;
	static MainScene* pMainScene;
};