#include "pch.h"
#include "Param.h"

bool operator<(const ParamKey& p0, const ParamKey& p1)
{
	if (p0.name > p1.name)
		return false;
	if (p0.name < p1.name)
		return true;
	if (p0.index > p1.index)
		return false;
	if (p0.index < p1.index)
		return true;
	if (p0.field < p1.field)
		return true;
	return false;
}

bool operator==(const ParamKey& p0, const ParamKey& p1)
{
	return p0.field == p1.field && p0.index == p1.index && p0.name == p1.name;
}
