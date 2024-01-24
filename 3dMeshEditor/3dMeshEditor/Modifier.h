#pragma once
#include <windows.h>

class Modifier
{
protected:
	HWND hWnd;
public:
	void setWindow(HWND hWnd) { this->hWnd = hWnd; }
	virtual void processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam) {};
};