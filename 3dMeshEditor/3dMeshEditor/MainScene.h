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
	void accept(Scene::Visitor* visitor) const;

	SelectedObjectBox selectedObjectsBoxes[MAX_BOUNDING_SPHERES_COUNT];
	flt4 boundingSpheres[MAX_BOUNDING_SPHERES_COUNT];
	NodeID boundingSphereToNode[MAX_BOUNDING_SPHERES_COUNT];
	uint32_t spheresCount = 0;

private:
	Scene scene;
	static MainScene* pMainScene;
};