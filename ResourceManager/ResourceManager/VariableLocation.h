#pragma once

#include "StringManager.h"

struct VariableLocation
{
	string_id name = string_id(-1);
	int index = -1;
	string_id field = string_id(-1);
};