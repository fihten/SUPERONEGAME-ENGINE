#include "Selector.h"
#include "GraphicsCore.h"
#include "Cameras.h"
#include "MainScene.h"
#include <algorithm>

#undef min
#undef max

Selector* Selector::pSelector = nullptr;

Selector::Selector()
{
	// Initialization of selected objects' boxes 	
	selectedObjectsBoxesMesh.setTechnique(selected_objects_box_id);
	selectedObjectsBoxesMesh.setPass(p0_pass_id);

	selectedObjectsBoxesMesh.gpuReadyData = selectedObjectsBoxes;
	selectedObjectsBoxesMesh.elementSize = sizeof SelectedObjectBox;

	// Initialization of frames of references
	framesOfReferences.setTechnique(frame_of_reference_id);
	framesOfReferences.setPass(p0_pass_id);

	framesOfReferences.gpuReadyData = &frameOfReference;
	framesOfReferences.elementSize = sizeof FrameOfReference;

	const char* septh_stencil_state = "DepthEnable=false;DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;DepthFunc=D3D11_COMPARISON_ALWAYS;StencilEnable=false;StencilReadMask=0;StencilWriteMask=0;FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;";
	framesOfReferences.setDepthStencilState(StringManager::toStringId(septh_stencil_state));

	areaOfSelection = createAreaOfSelection();
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

	float minX = std::min<float>(mousePosX0, mousePosX1);
	float maxX = std::max<float>(mousePosX0, mousePosX1);

	float minY = std::min<float>(mousePosY0, mousePosY1);
	float maxY = std::max<float>(mousePosY0, mousePosY1);

	flt4x4 areaOfSelectionPos(
		maxX - minX, 0, 0, 0,
		0, maxY - minY, 0, 0,
		0, 0, 1, 0,
		minX, minY, 0, 1
	);

	ParamKey areaTransform_key{ StringManager::toStringId("areaTransform"),-1,string_id(-1) };
	areaOfSelection.setParam(areaTransform_key, areaOfSelectionPos);

	minX *= 0.5 * w;
	maxX *= 0.5 * w;

	minY *= 0.5 * h;
	maxY *= 0.5 * h;

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

	GraphicsCore::instance()->setV(camera.getView());
	GraphicsCore::instance()->setSelectedObjectsForWritingRoughByFrustum();
	GraphicsCore::instance()->setBoundingSpheresRoughByFrustum();

	GraphicsCore::instance()->findRoughlySelectedObjects();

	GraphicsCore::instance()->setSelectorFrustumFine(selectorFrustum);
	GraphicsCore::instance()->updateSelectorFrustumDiagonals(selectorFrustumDiagonals);
	GraphicsCore::instance()->setSelectorFrustumDiagonals();
	GraphicsCore::instance()->setVerticesFineByFrustum();
	GraphicsCore::instance()->setIndicesFineByFrustum();
	GraphicsCore::instance()->setObjectsInfoFineByFrustum();
	GraphicsCore::instance()->setVFine(camera.getView());
	GraphicsCore::instance()->setThreshold(0.0);
	GraphicsCore::instance()->setRoughlySelectedObjects();
	GraphicsCore::instance()->initSelectedTrianglesWithZeros();
	GraphicsCore::instance()->setSelectedTrianglesWriteFineByFrustum();

	GraphicsCore::instance()->applyContextForFineSelection();
	GraphicsCore::instance()->checkIntersection();

	selectedObjectsCount = 0;
	class VisitFineSelectedObjects : public SelectedObjectVisitor
	{
		void updateMinMax(const SelectedObjectBox& sob)
		{
			flt3 corners[8] = {
				sob.posW - sob.axis0 - sob.axis1 - sob.axis2,
				sob.posW - sob.axis0 - sob.axis1 + sob.axis2,
				sob.posW - sob.axis0 + sob.axis1 - sob.axis2,
				sob.posW - sob.axis0 + sob.axis1 + sob.axis2,
				sob.posW + sob.axis0 - sob.axis1 - sob.axis2,
				sob.posW + sob.axis0 - sob.axis1 + sob.axis2,
				sob.posW + sob.axis0 + sob.axis1 - sob.axis2,
				sob.posW + sob.axis0 + sob.axis1 + sob.axis2
			};
			for (int i = 0; i < 8; i++)
			{
				min.x() = std::min<float>(min.x(), corners[i].x());
				min.y() = std::min<float>(min.y(), corners[i].y());
				min.z() = std::min<float>(min.z(), corners[i].z());

				max.x() = std::max<float>(max.x(), corners[i].x());
				max.y() = std::max<float>(max.y(), corners[i].y());
				max.z() = std::max<float>(max.z(), corners[i].z());
			}
		}
	public:
		VisitFineSelectedObjects(): min(FLT_MAX,FLT_MAX,FLT_MAX),max(-FLT_MAX,-FLT_MAX,-FLT_MAX)
		{
		}

		void operator()(uint32_t objectID)
		{
			auto& boxes = Selector::instance()->selectedObjectsBoxes;
			auto& count = Selector::instance()->selectedObjectsCount;
			auto& objects = Selector::instance()->selectedObjects;
			boxes[count] = MainScene::instance()->selectedObjectsBoxes[objectID];
			objects[count] = objectID;

			updateMinMax(boxes[count]);

			count++;
		}
		flt3 min;
		flt3 max;
	};
	VisitFineSelectedObjects fineVisitor;
	GraphicsCore::instance()->traverseFineSelectedObjects(&fineVisitor);

	selectedObjectsBoxesMesh.verticesCount = selectedObjectsCount;

	frameOfReference.posW = 0.5f * (fineVisitor.min + fineVisitor.max);
	frameOfReference.axis0 = 0.5f * (fineVisitor.max - fineVisitor.min).x() * flt3(1, 0, 0);
	frameOfReference.axis1 = 0.5f * (fineVisitor.max - fineVisitor.min).y() * flt3(0, 1, 0);
	frameOfReference.axis2 = 0.5f * (fineVisitor.max - fineVisitor.min).z() * flt3(0, 0, 1);
	frameOfReference.scale = 1;
	if (selectedObjectsCount == 1)
	{
		frameOfReference.posW = selectedObjectsBoxes[0].posW;
		frameOfReference.axis0 = selectedObjectsBoxes[0].axis0;
		frameOfReference.axis1 = selectedObjectsBoxes[0].axis1;
		frameOfReference.axis2 = selectedObjectsBoxes[0].axis2;
		frameOfReference.scale = 1;
	}
	framesOfReferences.verticesCount = selectedObjectsCount > 0 ? 1 : 0;
}

void Selector::selectObject(float mousePosX, float mousePosY)
{
	if (bProcessOfMultipleSelection)
		return;

	auto& camera = cameras()[MAIN_CAMERA];

	float nearZ = camera.getNear();
	float fovY = camera.getFov() * M_PI / 180;
	float h = 2 * nearZ * std::tan(0.5 * fovY);

	float ar = camera.getAspectRatio();
	float w = ar * h;

	mousePosX *= 0.5 * w;
	mousePosY *= 0.5 * h;

	Segment selectingSegments[MAX_SELECTING_SEGMENTS_COUNT];
	auto& seg = selectingSegments[0];

	seg.v0 = flt3(mousePosX, mousePosY, nearZ);

	float farZ = camera.getFar();
	seg.v1 = flt3(farZ * mousePosX / nearZ, farZ * mousePosY / nearZ, farZ);

	flt4x4 viewInv = camera.getView().inverse();

	flt4 v0(seg.v0, 1);
	seg.v0 = (v0 * viewInv).xyz();

	flt4 v1(seg.v1, 1);
	seg.v1 = (v1 * viewInv).xyz();

	GraphicsCore::instance()->updateSelectingSegments(selectingSegments);
	GraphicsCore::instance()->setSelectingSegmentsRoughBySegments();

	GraphicsCore::instance()->setSelectingSegmentsCountRoughBySegments(1);
	GraphicsCore::instance()->setBoundingSpheresRoughBySegments();
	GraphicsCore::instance()->setSelectedObjectsWriteRoughBySegments();

	GraphicsCore::instance()->findRoughlySelectedObjectsBySegments();

	GraphicsCore::instance()->setSelectingSegmentsFineBySegments();
	GraphicsCore::instance()->setSelectedObjectsFineBySegments();

	GraphicsCore::instance()->setSelectingSegmentsCountFineBySegments(1);

	GraphicsCore::instance()->setVerticesFineBySegments();
	GraphicsCore::instance()->setIndicesFineBySegments();
	GraphicsCore::instance()->setObjectsInfoFineBySegments();

	GraphicsCore::instance()->setDistancesToClosestObjects();
	GraphicsCore::instance()->setClosestObjects();

	GraphicsCore::instance()->initDistancesToClosestObjects();
	GraphicsCore::instance()->initClosestObjects();

	GraphicsCore::instance()->findSelectedObjectsFineBySegments();
	
	if (bPopSelectedObject)
	{
		selectedObjectsCount--;
		selectedObject = uint32_t(-1);
		selectedObjectsBoxesMesh.verticesCount = selectedObjectsCount;
		bPopSelectedObject = false;
	}
	if (selectedObject != uint32_t(-1))
	{
		auto pSelectedObject = std::find(selectedObjects, selectedObjects + selectedObjectsCount, selectedObject);
		uint32_t selectedObjectBox = pSelectedObject - selectedObjects;
		selectedObjectsBoxes[selectedObjectBox].color = flt3(1, 0, 0);
	}
	
	GraphicsCore::instance()->getSelectedObjectsFineBySegments(&selectedObject, 1);

	if (selectedObject == uint32_t(-1))
		return;

	auto pSelectedObject = std::find(selectedObjects, selectedObjects + selectedObjectsCount, selectedObject);
	uint32_t selectedObjectBox = pSelectedObject - selectedObjects;
	if (selectedObjectBox == selectedObjectsCount)
	{
		selectedObjectsBoxes[selectedObjectBox] = MainScene::instance()->selectedObjectsBoxes[selectedObject];
		selectedObjectsCount++;
		selectedObjectsBoxesMesh.verticesCount = selectedObjectsCount;
		bPopSelectedObject = true;
	}
	selectedObjectsBoxes[selectedObjectBox].color = flt3(0, 0, 1);
}

void Selector::draw()
{
	if (selectedObjectsCount > 0)
	{
		GraphicsCore::instance()->draw(selectedObjectsBoxesMesh);
		GraphicsCore::instance()->draw(framesOfReferences);
	}
	if (bProcessOfMultipleSelection)
		GraphicsCore::instance()->draw(areaOfSelection);
}

bool Selector::turnOnMultipleSelection()
{
	if (selectedObject == uint32_t(-1))
		bProcessOfMultipleSelection = true;
	return bProcessOfMultipleSelection;
}

void Selector::turnOffMultipleSelection()
{
	bProcessOfMultipleSelection = false;
}
