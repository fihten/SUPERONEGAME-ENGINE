#pragma once
#include <windows.h>
#include <stdint.h>

class Modifier
{
protected:
	HWND hWnd;
public:
	enum class Behaviour :uint32_t
	{
		CONTINUE = 0,
		FINISH = 1
	};

	void setWindow(HWND hWnd) { this->hWnd = hWnd; }
	virtual Behaviour processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam) { return Behaviour::CONTINUE; };
	virtual void draw() {};
};