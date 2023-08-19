#include "Selector.h"
#include "GraphicsCore.h"
#include "Cameras.h"
#include "MainScene.h"
#include <algorithm>

Selector* Selector::pSelector = nullptr;

Selector::Selector()
{
	selectedObjectsBoxesMesh.setTechnique(selected_objects_box_id);
	selectedObjectsBoxesMesh.setPass(p0_pass_id);

	selectedObjectsBoxesMesh.gpuReadyData = selectedObjectsBoxes;
	selectedObjectsBoxesMesh.elementSize = sizeof SelectedObjectBox;
}

Selector* Selector::instance()
{
	if (pSelector == nullptr)
		pSelector = new Selector();
	return pSelector;
}

void Selector::selectObjects(
	float mousePosX0, float mousePosY0,
	float mousePosX1, float mousePosY1
)
{
	auto& camera = cameras()[MAIN_CAMERA];
	
	float nearZ = camera.getNear();
	float fovY = camera.getFov() * M_PI / 180;
	float h = 2 * nearZ * std::tan(0.5 * fovY);
	
	float ar = camera.getAspectRatio();
	float w = ar * h;

	float minX = 0.5 * w * std::min<float>(mousePosX0, mousePosX1);
	float maxX = 0.5 * w * std::max<float>(mousePosX0, mousePosX1);

	float minY = 0.5 * h * std::min<float>(mousePosY0, mousePosY1);
	float maxY = 0.5 * h * std::max<float>(mousePosY0, mousePosY1);

	flt3 frontNormal(0, 0, -1);
	flt3 frontPoint(0, 0, nearZ);
	selectorFrustum.plane[FRONT_PLANE] = flt4(frontNormal, -dot(frontNormal, frontPoint));

	flt3 backNormal(0, 0, 1);
	float farZ = camera.getFar();
	flt3 backPoint(0, 0, farZ);
	selectorFrustum.plane[BACK_PLANE] = flt4(backNormal, -dot(backNormal, backPoint));

	flt3 leftNormal(-nearZ, 0, minX);
	leftNormal.normalize();
	flt3 leftPoint(minX, minY, nearZ);
	selectorFrustum.plane[LEFT_PLANE] = flt4(leftNormal, -dot(leftNormal, leftPoint));

	flt3 rightNormal(nearZ, 0, -maxX);
	rightNormal.normalize();
	flt3 rightPoint(maxX, maxY, nearZ);
	selectorFrustum.plane[RIGHT_PLANE] = flt4(rightNormal, -dot(rightNormal, rightPoint));

	flt3 bottomNormal(0, -nearZ, minY);
	bottomNormal.normalize();
	flt3 bottomPoint(maxX, minY, nearZ);
	selectorFrustum.plane[BOTTOM_PLANE] = flt4(bottomNormal, -dot(bottomNormal, bottomPoint));

	flt3 topNormal(0, nearZ, -maxY);
	topNormal.normalize();
	flt3 topPoint(maxX, maxY, nearZ);
	selectorFrustum.plane[TOP_PLANE] = flt4(topNormal, -dot(topNormal, topPoint));

	GraphicsCore::instance()->setSelectorFrustumRough(selectorFrustum);

	selectorFrustumDiagonals[0].v0 = flt3(minX, minY, nearZ);
	selectorFrustumDiagonals[0].v1 = (farZ / nearZ) * flt3(maxX, maxY, nearZ);

	selectorFrustumDiagonals[1].v0 = flt3(minX, maxY, nearZ);
	selectorFrustumDiagonals[1].v1 = (farZ / nearZ) * flt3(maxX, minY, nearZ);

	selectorFrustumDiagonals[2].v0 = flt3(maxX, minY, nearZ);
	selectorFrustumDiagonals[2].v1 = (farZ / nearZ) * flt3(minX, maxY, nearZ);

	selectorFrustumDiagonals[3].v0 = flt3(maxX, maxY, nearZ);
	selectorFrustumDiagonals[3].v1 = (farZ / nearZ) * flt3(minX, minY, nearZ);

	GraphicsCore::instance()->updateSelectorFrustumDiagonals(selectorFrustumDiagonals);

	GraphicsCore::instance()->setV(camera.getView());

	GraphicsCore::instance()->findRoughlySelectedObjects();

	GraphicsCore::instance()->setSelectorFrustumFine(selectorFrustum);
	GraphicsCore::instance()->setThreshold(0.0);

	GraphicsCore::instance()->initSelectedTrianglesWithZeros();

	class VisitRoughlySelectedObjects : public SelectedObjectVisitor
	{
	public:
		void operator()(uint32_t objectID)
		{
			NodeID meshID = MainScene::instance()->boundingSphereToNode[objectID];
			Scene::MeshNode* node = (Scene::MeshNode*)MainScene::instance()->getNode(meshID);

			flt4x4 world = MainScene::instance()->getNodePosition(meshID);
			flt4x4 view = cameras()[MAIN_CAMERA].getView();

			flt4x4 wv = world * view;
			GraphicsCore::instance()->setWV(wv);
			GraphicsCore::instance()->setGeometryForFineSelection(*node->mesh);
			GraphicsCore::instance()->setTrianglesCount(node->mesh->getIndicesCount() / 3);
			GraphicsCore::instance()->setMeshId(objectID);

			GraphicsCore::instance()->applyContextForFineSelection();
			GraphicsCore::instance()->checkIntersection();
		}
	};
	VisitRoughlySelectedObjects roughVisitor;
	GraphicsCore::instance()->traverseRoughlySelectedObjects(&roughVisitor);

	selectedObjectsCount = 0;
	class VisitFineSelectedObjects : public SelectedObjectVisitor
	{
	public:
		void operator()(uint32_t objectID)
		{
			auto& boxes = Selector::instance()->selectedObjectsBoxes;
			auto& count = Selector::instance()->selectedObjectsCount;
			boxes[count++] = MainScene::instance()->selectedObjectsBoxes[objectID];
		}
	};
	VisitFineSelectedObjects fineVisitor;
	GraphicsCore::instance()->traverseFineSelectedObjects(&fineVisitor);

	selectedObjectsBoxesMesh.verticesCount = selectedObjectsCount;
}

void Selector::draw()
{
	if (selectedObjectsCount == 0)
		return;
	GraphicsCore::instance()->draw(selectedObjectsBoxesMesh);
}

void Selector::turnOn()
{

}

void Selector::turnOff()
{

}

void Selector::processRoughlySelectedObject(uint32_t objectId)
{

}