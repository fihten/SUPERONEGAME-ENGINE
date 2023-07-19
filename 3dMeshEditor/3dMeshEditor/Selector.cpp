#include "Selector.h"
#include "GraphicsCore.h"
#include "Cameras.h"
#include "MainScene.h"
#include <algorithm>

Selector* Selector::pSelector = nullptr;

Selector::Selector()
{
	selectedObjectsBoxesMesh.setParam("technique", "SelectedObjectsBox");
	selectedObjectsBoxesMesh.setParam("pass", "P0");
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
	Envelope selectorEnvelope;

	selectorEnvelope.min.x() = std::min<float>(mousePosX0, mousePosX1);
	selectorEnvelope.min.y() = std::min<float>(mousePosY0, mousePosY1);
	selectorEnvelope.min.z() = 0;

	selectorEnvelope.max.x() = std::max<float>(mousePosX0, mousePosX1);
	selectorEnvelope.max.y() = std::max<float>(mousePosY0, mousePosY1);
	selectorEnvelope.max.z() = 1;

	GraphicsCore::instance()->setSelectorEnvelopeRough(selectorEnvelope);
	
	flt4x4 view = cameras()[MAIN_CAMERA].getView();
	flt4x4 proj = cameras()[MAIN_CAMERA].getProj();
	flt4x4 vp = view * proj;
	GraphicsCore::instance()->setVP(vp);

	GraphicsCore::instance()->findRoughlySelectedObjects();

	GraphicsCore::instance()->setSelectorEnvelopeFine(selectorEnvelope);
	GraphicsCore::instance()->setThreshold(0.001);

	selectedObjectsCount = 0;
	class VisitSelectedObjects : public RoughlySelectedObjectVisitor
	{
	public:
		void operator()(uint32_t objectID)
		{
			class MeshVisitor : public Scene::Visitor
			{
				uint32_t objectID = -1;
			public:
				MeshVisitor(uint32_t objectID) :objectID(objectID) {}

				void startVisit(const Scene::MeshNode* node)
				{
					NodeID meshID = MainScene::instance()->envelopeToNode[objectID];
					if (node->ID != meshID)
						return;
					
					flt4x4 world = MainScene::instance()->getNodePosition(meshID);
					flt4x4 view = cameras()[MAIN_CAMERA].getView();
					flt4x4 proj = cameras()[MAIN_CAMERA].getProj();

					flt4x4 wvp = world * view * proj;
					GraphicsCore::instance()->setWVP(wvp);
					GraphicsCore::instance()->setGeometryForFineSelection(*node->mesh);
					GraphicsCore::instance()->setTrianglesCount(node->mesh->getIndicesCount() / 3);

					GraphicsCore::instance()->checkIntersection();
					if (GraphicsCore::instance()->isObjectIntersected())
					{
						auto& boxes = Selector::instance()->selectedObjectsBoxes;
						auto& count = Selector::instance()->selectedObjectsCount;
						auto& selectedObjectBox = boxes[count++];

						flt3& min = MainScene::instance()->envelopes[objectID].min;
						flt3& max = MainScene::instance()->envelopes[objectID].max;
						
						flt4x4& w = MainScene::instance()->envelopes[objectID].transform;

						selectedObjectBox.posW = 0.5f * (min + max);

						selectedObjectBox.axis0 = 0.5f * (max.x() - min.x()) * flt3(1.1f, 0, 0);
						selectedObjectBox.axis0 = selectedObjectBox.axis0 * w;

						selectedObjectBox.axis1 = 0.5f * (max.y() - min.y()) * flt3(0, 1.1f, 0);
						selectedObjectBox.axis1 = selectedObjectBox.axis1 * w;

						selectedObjectBox.axis2 = 0.5f * (max.z() - min.z()) * flt3(0, 0, 1.1f);
						selectedObjectBox.axis2 = selectedObjectBox.axis2 * w;

						selectedObjectBox.color = flt3(1, 0, 0);
						selectedObjectBox.size = 0.5f;
					}
				}
			};

			MeshVisitor visitor(objectID);
			MainScene::instance()->accept(&visitor);
		}
	};
	VisitSelectedObjects visitor;
	GraphicsCore::instance()->traverseRoughlySelectedObjects(&visitor);
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