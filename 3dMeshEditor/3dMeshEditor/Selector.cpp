#include "Selector.h"

Selector* Selector::pSelector = nullptr;

Selector* Selector::instance()
{
	if (pSelector == nullptr)
		pSelector = new Selector();
	return pSelector;
}

void Selector::selectObjects(
	int mousePosX0, int mousePosY0,
	int mousePosX1, int mousePosY1
)
{

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