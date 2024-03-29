#pragma once

#include "Scene.h"
#include "SelectedObjectBox.h"
#include "GraphicsCore.h"
#include "FrameOfReferenceStateObserver.h"

class MainSceneUpdatingVisitor;
class MainScene : public FrameOfReferenceStateObserver
{
	friend MainSceneUpdatingVisitor;
	MainScene();
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
	
	flt4x4& getNodePosition(NodeID id);
	void accept(Scene::Visitor* visitor) const;
	void* getNode(NodeID id);

	void getTriangle(
		uint32_t objectIndex,
		uint32_t triangleIndex,
		flt3& v0, flt3& v1, flt3& v2
	);

	SelectedObjectBox selectedObjectsBoxes[MAX_BOUNDING_SPHERES_COUNT];
	ObjectInfo objectsInfo[MAX_BOUNDING_SPHERES_COUNT];
	
	NodeID boundingSphereToNode[MAX_BOUNDING_SPHERES_COUNT];
	
	uint32_t spheresCount = 0;

	flt3 vertices[MAX_OBJECTS_COUNT * MAX_VERTICES_COUNT];
	uint32_t verticesCount = 0;

	uint32_t indices[MAX_OBJECTS_COUNT * MAX_TRIANGLES_COUNT * 3];
	uint32_t indicesCount = 0;

	uint32_t meshIDtoStartVertex[MAX_OBJECTS_COUNT];
	uint32_t meshIDtoStartIndex[MAX_OBJECTS_COUNT];

public:
	void update(UpdateType updateType);
	
	void save(const std::string& path) const;
	void load(const std::string& path);

	void clear();
	void updateGpu();

private:
	Scene scene;
	static MainScene* pMainScene;
};