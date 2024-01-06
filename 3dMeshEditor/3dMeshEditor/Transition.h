#pragma once
#include "Modifier.h"

class Transition :public Modifier
{
public:
	void processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam);
};