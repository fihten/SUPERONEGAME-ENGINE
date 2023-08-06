#pragma once
#include <string>

typedef unsigned int string_id;

extern string_id float_id;
extern string_id float2_id;
extern string_id float3_id;
extern string_id float4_id;

extern string_id position_id;

extern string_id technique_id;
extern string_id pass_id;

extern string_id node_position_id;
extern string_id node_axis_z_id;

extern string_id cameras_id;
extern string_id wvp_id;
extern string_id vp_id;
extern string_id v_id;
extern string_id p_id;
extern string_id eye_pos_id;
extern string_id fwd_id;

extern string_id position_in_scene_id;
extern string_id inverse_transpose_of_position_in_scene_id;

struct StringManager
{
	static void init();

	static string_id toStringId(const char* str);
	static string_id toStringId(const std::string& str);
private:
	StringManager();
	
	static StringManager* stringManager;
	char** hash = nullptr;
	const int hashSize = 1048576;
};