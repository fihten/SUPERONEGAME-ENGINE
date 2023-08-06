// StringManager.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "StringManager.h"
#include <cmath>
#include <Windows.h>

string_id float_id;
string_id float2_id;
string_id float3_id;
string_id float4_id;

string_id position_id;

string_id technique_id;
string_id pass_id;

string_id node_position_id;
string_id node_axis_z_id;

string_id cameras_id;
string_id wvp_id;
string_id vp_id;
string_id v_id;
string_id p_id;
string_id eye_pos_id;
string_id fwd_id;

string_id position_in_scene_id;
string_id inverse_transpose_of_position_in_scene_id;

string_id selected_object_technique_id;
string_id p0_pass_id;

StringManager* StringManager::stringManager = nullptr;

StringManager::StringManager()
{
	hash = new char* [hashSize];
	std::memset(hash, 0, hashSize);

	float_id = toStringId("float");
	float2_id = toStringId("float2");
	float3_id = toStringId("float3");
	float4_id = toStringId("float4");

	position_id = toStringId("POSITION");

	technique_id = toStringId("technique");
	pass_id = toStringId("pass");

	node_position_id = toStringId("node_position");
	node_axis_z_id = toStringId("node_axis_z");

	cameras_id = toStringId("cameras");
	wvp_id = toStringId("WVP");
	vp_id = toStringId("VP");
	v_id = toStringId("V");
	p_id = toStringId("P");
	eye_pos_id = toStringId("eye_pos");
	fwd_id = toStringId("fwd");

	position_in_scene_id = toStringId("position_in_scene");
	inverse_transpose_of_position_in_scene_id = toStringId("inverse_transpose_of_position_in_scene");

	selected_object_technique_id = toStringId("SelectedObject");
	p0_pass_id = toStringId("P0");
}

void StringManager::init()
{
	if (stringManager != nullptr)
		return;
	stringManager = new StringManager();
}

string_id StringManager::toStringId(const char* str)
{
	string_id strID = 0;

	int n = std::strlen(str);
	int u = sizeof(string_id);
	int n0 = n / u;
	string_id* str0 = (string_id*)str;
	for (int ci = 0; ci < n0; ci += u)
	{
		string_id strID0 = str0[ci];
		strID = (strID & (~strID0)) | ((~strID) & strID0);
	}

	string_id strID0 = 0;
	for (int ci = 0; ci < n % u; ci++)
	{
		char ch = *(str + n - u + ci);
		strID0 |= ((string_id)(ch) << ci);
	}
	strID = (strID & (~strID0)) | ((~strID) & strID0);

	strID %= stringManager->hashSize;

	if (stringManager->hash[strID] != nullptr)
	{
		if (std::strcmp(stringManager->hash[strID], str) == 0)
		{
			return strID;
		}
		else
		{
			char buff[256] = { 0 };
			sprintf(buff, "Hashing of string \"%s\" is failed");
			MessageBoxA(0, buff, 0, MB_OK);
			return string_id(-1);
		}
	}

	stringManager->hash[strID] = new char[n + 1];
	std::memcpy(stringManager->hash[strID], str, n);
	stringManager->hash[strID][n] = 0;

	return strID;
}

string_id StringManager::toStringId(const std::string& str)
{
	return toStringId(str.c_str());
}

const char* StringManager::toString(string_id str_id)
{
	return stringManager->hash[str_id];
}