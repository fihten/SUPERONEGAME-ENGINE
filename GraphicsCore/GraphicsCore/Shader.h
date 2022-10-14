#pragma once
#include <string>
#include <map>
#include "All.h"

void processShader(ID3D11Device* device, LPCTSTR path);
void processShaders(ID3D11Device* device);

std::map<std::string, std::string>&& loadVariableLocationsFromFile(const std::string& path);