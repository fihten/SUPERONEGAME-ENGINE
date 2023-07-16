#include "Selector.h"
#include "GraphicsCore.h"
#include "Cameras.h"
#include <algorithm>

Selector* Selector::pSelector = nullptr;

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
	GraphicsCore::instance()->getRoughlySelectedObjects(selectedObjects);
}

void Selector::draw()
{

}

void Selector::turnOn()
{

}

void Selector::turnOff()
{

}