#pragma once
#include "Mesh.h"
#include "SelectedObjectBox.h"
#include "FrameOfReference.h"
#include "GraphicsCore.h"
#include "Frustum.h"
#include "Segment.h"

class Selector
{
	Selector();
public:
	static Selector* instance();
	void selectObjects(
		float mousePosX0, float mousePosY0,
		float mousePosX1, float mousePosY1
	);
	void selectObject(float mousePosX, float mousePosY);
	void draw();
	bool turnOnMultipleSelection();
	void turnOffMultipleSelection();

	SelectedObjectBox selectedObjectsBoxes[MAX_BOUNDING_SPHERES_COUNT];
	uint32_t selectedObjectsCount = 0;
	
	uint32_t selectedObjects[MAX_BOUNDING_SPHERES_COUNT];
	uint32_t selectedObject = uint32_t(-1);

	bool bPopSelectedObject = false;

	Frustum selectorFrustum;
	Segment selectorFrustumDiagonals[4];

	FrameOfReference frameOfReference;

private:
	static Selector* pSelector;
	Mesh selectedObjectsBoxesMesh;
	Mesh framesOfReferences;
	Mesh areaOfSelection;
	bool bProcessOfMultipleSelection = false;
};