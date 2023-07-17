#pragma once
#include "Envelope.h"

struct SelectedObjectBox
{
	flt3 posW;
	flt3 axis0;
	flt3 axis1;
	flt3 axis2;
	flt3 color;
	float size;
};

class Selector
{
	Selector() {}
public:
	static Selector* instance();
	void selectObjects(
		float mousePosX0, float mousePosY0,
		float mousePosX1, float mousePosY1
	);
	void processRoughlySelectedObject(uint32_t objectId);
	void draw();
	void turnOn();
	void turnOff();
private:
	static Selector* pSelector;
	uint32_t selectedObjects[MaxEnvelopesCount];

	SelectedObjectBox selectedObjectsBoxes[MaxEnvelopesCount];
};