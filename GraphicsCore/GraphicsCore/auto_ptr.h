#pragma once
#include <malloc.h>

struct auto_ptr
{
	void* ptr;
public:
	auto_ptr(void* ptr) :ptr(ptr) {}
	~auto_ptr() { free(ptr); }
};