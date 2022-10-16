#pragma once
#include <string>
#include <map>
#include <vector>
#include "All.h"

void processShader(ID3D11Device* device, LPCTSTR shader_ath, LPCSTR config_path);
void processShaders(ID3D11Device* device);
