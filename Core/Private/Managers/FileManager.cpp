// Copyright (c) Eric Jeker 2025.

#include "Core/Managers/FileManager.h"

#include <fstream>
#include <nlohmann/json.hpp>

json FileManager::LoadJSON(const std::string& filePath)
{
    std::ifstream f(filePath);
    return json::parse(f);
}
