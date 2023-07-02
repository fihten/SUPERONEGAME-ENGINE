#pragma once

class Selector
{
	Selector() {}
public:
	static Selector* instance();
	void selectObjects(
		int mousePosX0, int mousePosY0,
		int mousePosX1, int mousePosY1
	);
	void draw();
	void turnOn();
	void turnOff();
private:
	static Selector* pSelector;
};