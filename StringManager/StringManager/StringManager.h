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