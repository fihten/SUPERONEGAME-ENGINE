#pragma once

#include "StringManager.h"

#define MAX_COUNT_OF_MULTIPLIERS 3
struct VariableLocation
{
	string_id name[MAX_COUNT_OF_MULTIPLIERS] = { string_id(-1) };
	int index[MAX_COUNT_OF_MULTIPLIERS] = { -1 };
	string_id field[MAX_COUNT_OF_MULTIPLIERS] = { string_id(-1) };
	int countOfMultipliers = -1;
};