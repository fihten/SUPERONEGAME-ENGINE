#pragma once
#include <windows.h>

class Modifier
{
public:
	virtual void processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};