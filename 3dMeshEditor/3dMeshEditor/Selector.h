#pragma once
#include "Envelope.h"

class Selector
{
	Selector() {}
public:
	static Selector* instance();
	void selectObjects(
		float mousePosX0, float mousePosY0,
		float mousePosX1, float mousePosY1
	);
	void draw();
	void turnOn();
	void turnOff();
private:
	static Selector* pSelector;
	uint32_t selectedObjects[MaxEnvelopesCount];
};