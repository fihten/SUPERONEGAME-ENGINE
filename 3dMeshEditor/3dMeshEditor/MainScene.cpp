#include "MainScene.h"
#include "GraphicsCore.h"

MainScene* MainScene::pMainScene = nullptr;

MainScene::MainScene()
{
	std::memset(meshIDtoStartVertex, 0xff, sizeof meshIDtoStartVertex);
	std::memset(meshIDtoStartIndex, 0xff, sizeof meshIDtoStartIndex);
}

MainScene* MainScene::instance()
{
	if (pMainScene == nullptr)
		pMainScene = new MainScene();
	return pMainScene;
}

NodeID MainScene::addTransformNode(const flt4x4& pos, NodeID id)
{
	return scene.addTransformNode(pos, id);
}

NodeID MainScene::addMeshNode(Mesh* mesh, NodeID id)
{
	NodeID meshNode = scene.addMeshNode(mesh, id);

	auto& selectedObjectBox = selectedObjectsBoxes[spheresCount];

	flt3 min = mesh->getBottomBorder();
	flt3 max = mesh->getTopBorder();

	flt4 posL(0.5f * (min + max), 1.0f);
	
	flt4x4& pos = scene.getNodePosition4x4(id);
	selectedObjectBox.posW = (posL * pos).xyz();

	selectedObjectBox.axis0 = 0.5f * (max.x() - min.x()) * flt3(1.1f, 0, 0);
	selectedObjectBox.axis0 = selectedObjectBox.axis0 * pos;

	selectedObjectBox.axis1 = 0.5f * (max.y() - min.y()) * flt3(0, 1.1f, 0);
	selectedObjectBox.axis1 = selectedObjectBox.axis1 * pos;

	selectedObjectBox.axis2 = 0.5f * (max.z() - min.z()) * flt3(0, 0, 1.1f);
	selectedObjectBox.axis2 = selectedObjectBox.axis2 * pos;

	selectedObjectBox.color = flt3(1, 0, 0);
	selectedObjectBox.size = 0.5f;

	mesh->getBoundingSphere(boundingSpheres[spheresCount], pos);
	boundingSphereToNode[spheresCount] = meshNode;
	
	if (meshIDtoStartIndex[mesh->id] == uint32_t(-1))
	{
		const std::vector<flt3>& verts = *(const std::vector<flt3>*)mesh->getStream(StringManager::toStringId("POSITION"), StreamType::FLT3);
		std::memcpy(vertices + verticesCount, verts.data(), verts.size() * sizeof(flt3));
		meshIDtoStartVertex[mesh->id] = verticesCount;
		verticesCount += verts.size();

		const std::vector<uint32_t>& inds = *mesh->getIndicies();
		std::memcpy(indices + indicesCount, inds.data(), inds.size() * sizeof(uint32_t));
		meshIDtoStartIndex[mesh->id] = indicesCount;
		indicesCount += inds.size();
	}

	objectsInfo[spheresCount].verticesOffset = meshIDtoStartVertex[mesh->id];
	objectsInfo[spheresCount].indicesOffset = meshIDtoStartIndex[mesh->id];
	objectsInfo[spheresCount].trianglesCount = mesh->getIndicesCount() / 3;
	objectsInfo[spheresCount].world = pos.transpose();

	++spheresCount;

	GraphicsCore::instance()->updateBoundingSpheres(boundingSpheres);
	GraphicsCore::instance()->setSpheresCount(spheresCount);

	GraphicsCore::instance()->updateVertices(vertices);
	GraphicsCore::instance()->updateIndices(indices);
	GraphicsCore::instance()->updateObjectsInfo(objectsInfo, spheresCount);
	GraphicsCore::instance()->setObjectsCount(spheresCount);

	return meshNode;
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const string_id& s)
{
	scene.setNodeParam(id, paramName, s);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const float& f)
{
	scene.setNodeParam(id, paramName, f);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const flt2& f2)
{
	scene.setNodeParam(id, paramName, f2);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const flt3& f3)
{
	scene.setNodeParam(id, paramName, f3);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const flt4& f4)
{
	scene.setNodeParam(id, paramName, f4);
}

void MainScene::setNodeParam(NodeID id, const ParamKey& paramName, const flt4x4& f4x4)
{
	scene.setNodeParam(id, paramName, f4x4);
}

void MainScene::createNodeParamReference(NodeID reference, NodeID referenced, const ParamKey& param)
{
	scene.createNodeParamReference(reference, referenced, param);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, string_id& s) const
{
	return scene.getNodeParam(id, paramName, s);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, float& f) const
{
	return scene.getNodeParam(id, paramName, f);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, flt2& f2) const
{
	return scene.getNodeParam(id, paramName, f2);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, flt3& f3) const
{
	return scene.getNodeParam(id, paramName, f3);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, flt4& f4) const
{
	return scene.getNodeParam(id, paramName, f4);
}

bool MainScene::getNodeParam(NodeID id, const ParamKey& paramName, flt4x4& f4x4) const
{
	return scene.getNodeParam(id, paramName, f4x4);
}

flt4x4& MainScene::getNodePosition(NodeID id)
{
	return scene.getNodePosition4x4(id);
}

void MainScene::accept(Scene::Visitor* visitor) const
{
	scene.accept(visitor);
}

void* MainScene::getNode(NodeID id)
{
	return scene.getNode(id);
}